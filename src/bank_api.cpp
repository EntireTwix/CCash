#include "bank_api.h"

#define CACHE_FOREVER resp->setExpiredTime(0)

#define CORS resp->addHeader("Access-Control-Allow-Origin", "*")
#define GEN_BODY                                \
    const auto temp_req = req->getJsonObject(); \
    const auto body = temp_req ? *temp_req : Json::Value();

#define RESPONSE_PARSE(R)                                                         \
    const auto r(R);                                                              \
    auto resp = HttpResponse::newHttpJsonResponse(JsonCast(std::move(r.second))); \
    resp->setStatusCode(r.first);                                                 \
    CORS;                                                                         \
    callback(resp);

#define RESPOND_TRUE                                               \
    auto resp = HttpResponse::newHttpJsonResponse(JsonCast(true)); \
    resp->setStatusCode(k200OK);                                   \
    CACHE_FOREVER;                                                 \
    CORS;                                                          \
    callback(resp);

#define NAME_PARAM req->getBody().data()

template <typename T>
constexpr Json::Value JsonCast(T &&val)
{
    if constexpr (std::is_same_v<T, int_fast8_t>)
    {
        return (int)val; //becuase of json lib interpreting 67 as 'A' for example
    }
    else if constexpr (std::is_same_v<T, uint64_t>)
    {
        return (Json::UInt64)val;
    }
    else if constexpr (std::is_same_v<T, uint32_t>)
    {
        return (Json::UInt)val;
    }
    else
    {
        return val;
    }
}

api::api(Bank &b) noexcept : bank(b) {}

#if API_VERSION >= 1

//Usage
void api::GetBal(req_args, const std::string &name) const
{
    RESPONSE_PARSE(bank.GetBal(name));
}
void api::GetLog(req_args)
{
    if constexpr (MAX_LOG_SIZE > 0)
    {
        RESPONSE_PARSE(bank.GetLogs(NAME_PARAM));
    }
    else
    {
        auto resp = HttpResponse::newHttpJsonResponse("Logs are Disabled");
        resp->setStatusCode(k404NotFound);
        CORS;
        CACHE_FOREVER;
        callback(resp);
    }
}
void api::SendFunds(req_args) const
{
    GEN_BODY
    RESPONSE_PARSE(bank.SendFunds(NAME_PARAM, body["to"].asCString(), body["amount"].asUInt()));
}
void api::VerifyPassword(req_args) const { RESPOND_TRUE }

//Meta Usage
void api::ChangePassword(req_args) const
{
    GEN_BODY
    bank.ChangePassword(NAME_PARAM, std::move(body["new_pass"].asCString()));
    RESPOND_TRUE
}
void api::AdminChangePassword(req_args) const
{
    GEN_BODY
    bank.ChangePassword(body["name"].asCString(), std::move(body["new_pass"].asCString()));
    RESPOND_TRUE
}
void api::SetBal(req_args) const
{
    GEN_BODY
    RESPONSE_PARSE(bank.SetBal(body["name"].asCString(), body["amount"].asUInt()));
}

//System Usage
void api::Help(req_args) const
{
    auto resp = HttpResponse::newHttpResponse();
    resp->setBody(""); //will be filled in with docs
    CORS;
    CACHE_FOREVER;
    callback(resp);
}
void api::Ping(req_args) const
{
    auto resp = HttpResponse::newHttpResponse();
    resp->setBody("pong");
    CORS;
    CACHE_FOREVER;
    callback(resp);
}
void api::Close(req_args) const
{
    bank.Save();
    app().quit();
    RESPOND_TRUE //filter handles admin creds
}
void api::Contains(req_args, const std::string &name) const
{
    auto resp = HttpResponse::newHttpJsonResponse(JsonCast(bank.Contains(name)));
    resp->setStatusCode(k200OK);
    CORS;
    callback(resp);
}
void api::AdminVerifyAccount(req_args) const
{
    RESPOND_TRUE //filter handles admin creds
}
void api::ApiVersion(req_args) const
{
    auto resp = HttpResponse::newHttpJsonResponse(API_VERSION);
    resp->setStatusCode(k200OK);
    CORS;
    CACHE_FOREVER;
    callback(resp);
}

void api::AddUser(req_args) const
{
    GEN_BODY
    RESPONSE_PARSE(bank.AddUser(body["name"].asCString(), 0, body["pass"].asCString()))
}
void api::AdminAddUser(req_args) const
{
    GEN_BODY
    RESPONSE_PARSE(bank.AddUser(body["name"].asCString(), body["balance"].asUInt(), body["pass"].asCString()))
}
void api::DelUser(req_args) const
{
    GEN_BODY
    RESPONSE_PARSE(bank.DelUser(body["name"].asCString()))
}
void api::AdminDelUser(req_args) const
{
    GEN_BODY
    RESPONSE_PARSE(bank.DelUser(body["name"].asCString()))
}
#endif