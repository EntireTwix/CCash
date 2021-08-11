#include "user_filter.h"

constexpr bool ValidUsername(const std::string &name) noexcept
{
    if (name.size() < min_name_size || name.size() > max_name_size)
    {
        return false;
    }
    for (char c : name)
    {
        if (!((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_'))
        {
            return false;
        }
    }
    return true;
}

template <bool set_body_flag, bool require_admin>
void UserFilter<set_body_flag, require_admin>::doFilter(const HttpRequestPtr &req,
                                                        FilterCallback &&fcb,
                                                        FilterChainCallback &&fccb)
{
    std::string_view auth_header = req->getHeader("Authorization");
    if (auth_header.size() > 6 && auth_header.size() <= ((max_name_size + 256) * 4) / 3) //"Basic " + (username + ':' + password) * 4/3
    {
        if (auth_header.substr(0, 6) == "Basic ")
        {
            std::string_view input = auth_header.substr(6);
            char result_buffer[max_name_size + 256]; //(username + ':' + 255 password)
            size_t new_sz;
            base64_decode(input.data(), input.size(), result_buffer, &new_sz, 0);

            std::string_view results_view(result_buffer, new_sz);
            std::size_t middle = results_view.find(':');
            if (middle != std::string::npos && ((new_sz - middle) <= 256))
            {
                StrFromSV_Wrapper username(results_view.substr(0, middle));
                if (ValidUsername(username.str)) //check if username is a valid attempt to avoid hashing/grabbing shared lock
                {
                    if constexpr (require_admin)
                    {
                        if (Bank::admin_account == username.str)
                        {
                            StrFromSV_Wrapper password(results_view.substr(middle + 1));
                            if (Bank::VerifyPassword(username.str, password.str))
                            {
                                fccb();
                                return;
                            }
                        }
                    }
                    else
                    {
                        StrFromSV_Wrapper password(results_view.substr(middle + 1));
                        if (Bank::VerifyPassword(username.str, results_view.substr(middle + 1)))
                        {
                            if constexpr (set_body_flag)
                            {
                                req->setParameter("name", username.str);
                            }
                            fccb();
                            return;
                        }
                    }
                }
            }
        }
    }
    fcb(HttpResponse::newCustomHttpResponse(BankResponse{k401Unauthorized, "\"Invalid Credentials\""}));
}

template class UserFilter<true, false>;  //user default
template class UserFilter<false, false>; //user sparse
template class UserFilter<false, true>;  //admin
