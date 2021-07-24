#pragma once
#include <drogon/HttpFilter.h>
#include "bank_resp.h"

using namespace drogon;

template <bool check_content_type>
class JsonFilter : public HttpFilter<JsonFilter<check_content_type>>
{
public:
    virtual void doFilter(const HttpRequestPtr &,
                          FilterCallback &&,
                          FilterChainCallback &&) override;
};