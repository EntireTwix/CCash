#pragma once
#include <drogon/HttpFilter.h>
#include <libbase64.h>
#include "bank.h"

using namespace drogon;

template <bool set_body_flag>
class UserFilter : public HttpFilter<UserFilter<set_body_flag>, false>
{
private:
    Bank &bank;

public:
    UserFilter(Bank &);

    virtual void doFilter(const HttpRequestPtr &,
                          FilterCallback &&,
                          FilterChainCallback &&) override;
};

using UserFilterDefault = UserFilter<true>;
using UserFilterSparse = UserFilter<false>;