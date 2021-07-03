#include "user_filter.h"

UserFilter::UserFilter(Bank &b) : bank(b) {}

void UserFilter::doFilter(const HttpRequestPtr &req,
                          FilterCallback &&fcb,
                          FilterChainCallback &&fccb)
{
    const std::string &auth_header = req->getHeader("Authorization");
    if (auth_header.size() > 6)
    {
        if (substr_view(auth_header, 0, 6) == "Basic ")
        {
            //only one alloc for this entire thing!
            char base64_result[((auth_header.size() - 6) * 3) / 4];
            size_t new_sz;
            base64_decode(substr_view(auth_header, 6).data(), auth_header.size() - 6, base64_result, &new_sz, 0);

            std::size_t res = std::string_view(base64_result, new_sz).find(':');
            if (res != std::string::npos)
            {
                std::string_view username = substr_view(base64_result, 0, res).str_view();
                std::string_view password = substr_view(base64_result, res + 1, new_sz).str_view();
                //another alloc
                if (bank.VerifyPassword(username, password))
                {
                    fccb();
                    return;
                }
            }
        }
    }
    auto resp = HttpResponse::newHttpJsonResponse("Invalid Credentials");
    resp->setStatusCode(k401Unauthorized);
    fcb(resp);
}