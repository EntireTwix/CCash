#pragma once
#include <drogon/HttpFilter.h>
#include <libbase64.h>
#include "str_intrusion.h"
#include "bank.h"

using namespace drogon;

template <bool set_body_flag, bool require_admin>
class UserFilter : public HttpFilter<UserFilter<set_body_flag, require_admin>, false>
{
public:
    virtual void doFilter(const HttpRequestPtr &,
                          FilterCallback &&,
                          FilterChainCallback &&) override;
};