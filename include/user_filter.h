#pragma once
#include <drogon/HttpFilter.h>
#include <libbase64.h>
#include "bank.h"

using namespace drogon;

class UserFilter : public HttpFilter<UserFilter, false>
{
private:
    Bank &bank;

public:
    UserFilter(Bank &);

    virtual void doFilter(const HttpRequestPtr &,
                          FilterCallback &&,
                          FilterChainCallback &&) override;
};