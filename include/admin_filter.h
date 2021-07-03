#pragma once
#include <drogon/HttpFilter.h>
#include <libbase64.h>
#include "bank.h"

using namespace drogon;

class AdminFilter : public HttpFilter<AdminFilter, false>
{
private:
    Bank &bank;

public:
    AdminFilter(Bank &);

    virtual void doFilter(const HttpRequestPtr &,
                          FilterCallback &&,
                          FilterChainCallback &&) override;
};