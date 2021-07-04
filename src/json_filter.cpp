#include "json_filter.h"

JsonFilter::JsonFilter() {}

void JsonFilter::doFilter(const HttpRequestPtr &req,
                          FilterCallback &&fcb,
                          FilterChainCallback &&fccb)
{
    std::string_view content_type = req->getHeader("Content-Type");
    if ((content_type == "application/json" || content_type == "*/*") && req->getHeader("Accept") == "application/json") //probably will need to change to contains() type function rather then equality check
    {
        fccb();
        return;
    }
    const auto &resp = HttpResponse::newHttpJsonResponse("Client must Accept JSON");
    resp->setStatusCode(k406NotAcceptable);
    fcb(resp);
}