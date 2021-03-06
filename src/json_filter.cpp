#include "json_filter.h"

inline bool Contains(std::string_view str, const std::string &val)
{
    return str.find(val) != std::string::npos;
}

template <bool check_content_type>
void JsonFilter<check_content_type>::doFilter(const HttpRequestPtr &req,
                                              FilterCallback &&fcb,
                                              FilterChainCallback &&fccb)
{
    std::string_view accept_header = req->getHeader("Accept");
    if constexpr (check_content_type)
    {
        std::string_view content_type = req->getHeader("content-type");
        if (content_type == "application/json" && (Contains(accept_header, "*/*") || Contains(accept_header, "application/json")))
        {
            fccb();
            return;
        }
        const auto &resp = HttpResponse::newCustomHttpResponse(BankResponse{k406NotAcceptable, "\"Client must Accept and have content-type of JSON\""});
        fcb(resp);
    }
    else
    {
        if ((Contains(accept_header, "*/*") || Contains(accept_header, "application/json")))
        {
            fccb();
            return;
        }
        const auto &resp = HttpResponse::newCustomHttpResponse(BankResponse{k406NotAcceptable, "\"Client must Accept JSON\""});
        fcb(resp);
    }
}

template class JsonFilter<true>;
template class JsonFilter<false>;