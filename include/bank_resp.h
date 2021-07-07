#pragma once
#include <string>
#include <drogon/HttpTypes.h>

struct BankResponse
{
    drogon::HttpStatusCode first = drogon::k200OK;
    std::string second;

    BankResponse() noexcept ;
    BankResponse(drogon::HttpStatusCode code, std::string &&str) noexcept ;
    BankResponse(drogon::HttpStatusCode code, const std::string &str) noexcept ;
};