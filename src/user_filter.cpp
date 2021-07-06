#include "user_filter.h"

template <bool set_body_flag, bool require_admin>
UserFilter<set_body_flag, require_admin>::UserFilter(Bank &b) : bank(b) {}
template <>
UserFilter<true, false>::UserFilter(Bank &b) : bank(b) {}
template <>
UserFilter<false, false>::UserFilter(Bank &b) : bank(b) {}
template <>
UserFilter<false, true>::UserFilter(Bank &b) : bank(b) {}

template <bool set_body_flag, bool require_admin>
void UserFilter<set_body_flag, require_admin>::doFilter(const HttpRequestPtr &req,
                                                        FilterCallback &&fcb,
                                                        FilterChainCallback &&fccb)
{
    std::string_view auth_header = req->getHeader("Authorization");
    if (auth_header.size() > 6)
    {
        if (auth_header.substr(0, 6) == "Basic ")
        {
            std::string_view base64_input = auth_header.substr(6);
            char base64_result[(base64_input.size() * 3) / 4]; //only alloc
            size_t new_sz;
            base64_decode(base64_input.data(), base64_input.size(), base64_result, &new_sz, 0);

            std::string_view results_view(base64_result, new_sz);
            std::size_t middle = results_view.find(':');
            if (middle != std::string::npos)
            {
                base64_result[middle] = '\0';
                const std::string &username(results_view.substr(0, middle).data());
                if constexpr (require_admin)
                {
                    if (bank.AdminVerifyAccount(username))
                    {
                        base64_result[new_sz] = '\0';
                        if (bank.VerifyPassword(username, results_view.substr(middle + 1)))
                        {
                            fccb();
                            return;
                        }
                    }
                }
                else
                {
                    base64_result[new_sz] = '\0';
                    if (bank.VerifyPassword(username, results_view.substr(middle + 1)))
                    {
                        if constexpr (set_body_flag)
                        {
                            req->setBody(username); //feels sub optimal
                        }
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