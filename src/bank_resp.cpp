#include "bank_resp.h"

BankResponse::BankResponse() noexcept = default;
BankResponse::BankResponse(drogon::HttpStatusCode code, std::string &&str) noexcept : first(code), second(str) {}
BankResponse::BankResponse(drogon::HttpStatusCode code, const std::string &str) noexcept : first(code), second(str) {}

template <>
drogon::HttpResponsePtr drogon::toResponse(BankResponse &&data)
{
    static thread_local auto res = std::make_shared<HttpResponseImpl>(data.first, CT_APPLICATION_JSON);
    res->setBody(std::move(data.second));
    static thread_local auto &advices = HttpAppFrameworkImpl::instance().getResponseCreationAdvices();
    if (!advices.empty())
    {
        for (auto &advice : advices)
        {
            advice(res);
        }
    }
    return res;
}