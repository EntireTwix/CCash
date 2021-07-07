#include "bank_resp.h"

BankResponse::BankResponse() = default;
BankResponse::BankResponse(drogon::HttpStatusCode code, std::string &&str) : first(code), second(str) {}
