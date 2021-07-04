#pragma once
#include <drogon/HttpFilter.h>

using namespace drogon;

class JsonFilter : public HttpFilter<JsonFilter, false>
{
public:
    JsonFilter();

    virtual void doFilter(const HttpRequestPtr &,
                          FilterCallback &&,
                          FilterChainCallback &&) override;
};