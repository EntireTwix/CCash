#include "json_filter.h"

JsonFilter::JsonFilter() {}

void JsonFilter::doFilter(const HttpRequestPtr &req,
                          FilterCallback &&fcb,
                          FilterChainCallback &&fccb)
{
    if (req->getHeader("Content-Type") == "application/json" && req->getHeader("Accept") == "application/json")
    {
        fccb();
        return;
    }
    const auto &resp = HttpResponse::newHttpJsonResponse("Client must Accept JSON");
    resp->setStatusCode(k406NotAcceptable);
    fcb(resp);
}