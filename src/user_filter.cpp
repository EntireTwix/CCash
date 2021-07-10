#include "user_filter.h"

template <bool set_body_flag, bool require_admin>
UserFilter<set_body_flag, require_admin>::UserFilter(Bank &b) : bank(b) {}

#define time_func_a(f, a, x)                                                                  \
    {                                                                                         \
        using namespace std::chrono;                                                          \
        uint32_t timer = 0;                                                                   \
        for (int i = 0; i < x; ++i)                                                           \
        {                                                                                     \
            auto t1 = high_resolution_clock::now().time_since_epoch();                        \
            f;                                                                                \
            auto t2 = high_resolution_clock::now().time_since_epoch();                        \
            a;                                                                                \
            timer += std::chrono::duration_cast<std::chrono::nanoseconds>((t2 - t1)).count(); \
        }                                                                                     \
        std::cout << timer / x << '\n';                                                       \
    }

#define time_func(f, x)                                                                       \
    {                                                                                         \
        using namespace std::chrono;                                                          \
        uint32_t timer = 0;                                                                   \
        for (int i = 0; i < x; ++i)                                                           \
        {                                                                                     \
            auto t1 = high_resolution_clock::now().time_since_epoch();                        \
            f;                                                                                \
            auto t2 = high_resolution_clock::now().time_since_epoch();                        \
            timer += std::chrono::duration_cast<std::chrono::nanoseconds>((t2 - t1)).count(); \
        }                                                                                     \
        std::cout << timer / x << '\n';                                                       \
    }

#define Op_a(v, name, x, a)   \
    {                         \
        std::cout << name;    \
        time_func_a(v, a, x); \
    }

#define Op(v, name, x)     \
    {                      \
        std::cout << name; \
        time_func(v, x);   \
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
            std::string_view base64_input = auth_header.substr(6);
            char result_buffer[max_name_size + 256]; //(username + ':' + 255 password)
            std::memset(result_buffer, 0, max_name_size + 256);
            size_t new_sz;
            base64_decode(base64_input.data(), base64_input.size(), result_buffer, &new_sz, 0);

            std::string_view results_view(result_buffer, new_sz);
            std::size_t middle = results_view.find(':');
            if (middle != std::string::npos)
            {
                StrFromSV_Wrapper username(results_view.substr(0, middle));
                if constexpr (require_admin)
                {
                    if (bank.AdminVerifyAccount(username.str))
                    {
                        StrFromSV_Wrapper password(results_view.substr(middle + 1));
                        if (bank.VerifyPassword(username.str, password.str))
                        {
                            fccb();
                            return;
                        }
                    }
                }
                else
                {
                    StrFromSV_Wrapper password(results_view.substr(middle + 1));
                    if (bank.VerifyPassword(username.str, results_view.substr(middle + 1)))
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
    const auto &resp = HttpResponse::newCustomHttpResponse(BankResponse(k401Unauthorized, "\"Invalid Credentials\""));
    fcb(resp);
}

template class UserFilter<true, false>;  //user default
template class UserFilter<false, false>; //user sparse
template class UserFilter<false, true>;  //admin