#include "bank_resp.h"

BankResponse::BankResponse() noexcept  = default;
BankResponse::BankResponse(drogon::HttpStatusCode code, std::string &&str) noexcept : first(code), second(str) {}
BankResponse::BankResponse(drogon::HttpStatusCode code, const std::string &str) noexcept  : first(code), second(str) {}
