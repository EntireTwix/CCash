#include "json_filter.h"

JsonFilter::JsonFilter() {}

__attribute__((always_inline)) inline bool Contains(std::string_view str, const std::string &val) { return str.find(val) != std::string::npos; }

void JsonFilter::doFilter(const HttpRequestPtr &req,
                          FilterCallback &&fcb,
                          FilterChainCallback &&fccb)
{
    std::string_view content_type = req->getHeader("content-type");
    std::string_view accept_header = req->getHeader("Accept");

    if (content_type == "applications/json" && (Contains(accept_header, "*/*") || Contains(accept_header, "application/json")))
    {
        fccb();
        return;
    }

    const auto &resp = HttpResponse::newHttpJsonResponse("Client must Accept JSON");
    resp->setStatusCode(k406NotAcceptable);
    fcb(resp);
}