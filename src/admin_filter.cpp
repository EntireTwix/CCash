#include "admin_filter.h"

AdminFilter::AdminFilter(Bank &b) : bank(b) {}

void AdminFilter::doFilter(const HttpRequestPtr &req,
                           FilterCallback &&fcb,
                           FilterChainCallback &&fccb)
{
    std::string_view auth_header = req->getHeader("Authorization");
    if (auth_header.size() > 6)
    {
        if (auth_header.substr(0, 6) == "Basic ")
        {
            std::string_view base64_input = auth_header.substr(6);
            char base64_result[(base64_input.size() * 3) / 4];
            size_t new_sz;
            base64_decode(base64_input.data(), base64_input.size(), base64_result, &new_sz, 0);

            std::string_view results_view(base64_result, new_sz);
            std::size_t middle = results_view.find(':');
            if (middle != std::string::npos)
            {
                base64_result[middle] = '\0';
                std::string_view username = results_view.substr(0, middle);
                if (bank.AdminVerifyAccount(username))
                {
                    base64_result[new_sz] = '\0';
                    std::string_view password = results_view.substr(middle + 1);
                    if (bank.VerifyPassword(username, password))
                    {
                        fccb();
                        return;
                    }
                }
            }
        }
    }
    const auto &resp = HttpResponse::newHttpJsonResponse("Invalid Credentials");
    resp->setStatusCode(k401Unauthorized);
    fcb(resp);
}