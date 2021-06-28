#include "bank_api.h"

#define JSON(V) callback(HttpResponse::newHttpJsonResponse(JsonCast(V))); //temporary
#define PASS_HEADER req->getHeader("Password")                            //temporary

#define GEN_BODY                                \
    const auto temp_req = req->getJsonObject(); \
    const auto body = temp_req ? *temp_req : Json::Value();

#define RESPONSE_PARSE(R)                                                         \
    const auto r = R;                                                             \
    auto resp = HttpResponse::newHttpJsonResponse(JsonCast(std::move(r.second))); \
    resp->setStatusCode(r.first);                                                 \
    callback(resp);

#define RESPOND_TRUE                                               \
    auto resp = HttpResponse::newHttpJsonResponse(JsonCast(true)); \
    resp->setStatusCode(k200OK);                                   \
    resp->setExpiredTime(0);                                       \
    callback(resp);

#define NAME_PARAM req->getBody()

template <typename T>
constexpr Json::Value JsonCast(T &&val)
{
    if constexpr (std::is_same_v<T, int_fast8_t>)
    {
        return (int)val; //becuase of json lib interpreting 67 as 'A' for example
    }
    else if constexpr (std::is_same_v<T, uint64_t>)
    {
        return (Json::Int64)val;
    }
    else if constexpr (std::is_same_v<T, int64_t>)
    {
        return (Json::Int64)val;
    }
    else
    {
        return val;
    }
}

#if API_VERSION == 1
api::api(Bank &b) : bank(b)
{
}

void api::GetBal(req_args, const std::string &name) const
{
    RESPONSE_PARSE(bank.GetBal(name));
}
void api::GetLog(req_args)
{
    if constexpr (max_log_size > 0)
    {
        RESPONSE_PARSE(bank.GetLogs(NAME_PARAM.data()));
    }
    else
    {
        auto resp = HttpResponse::newHttpJsonResponse("Logs are Disabled");
        resp->setStatusCode(k404NotFound);
        resp->setExpiredTime(0); //cached forever
        callback(resp);
    }
}
void api::SendFunds(req_args) const
{
    GEN_BODY
    RESPONSE_PARSE(bank.SendFunds(NAME_PARAM.data(), body["to"].asCString(), body["amount"].asUInt()));
}
void api::VerifyPassword(req_args) const
{
    RESPOND_TRUE
}

void api::ChangePassword(req_args) const
{
    GEN_BODY
    bank.ChangePassword(NAME_PARAM.data(), std::move(body["new_pass"].asCString())); //may make asString()
    RESPOND_TRUE
}

void api::Help(req_args) const
{
    auto resp = HttpResponse::newHttpResponse();
    resp->setBody(""); //will be filled in with docs
    resp->setExpiredTime(0);
    callback(resp);
}
void api::Ping(req_args) const
{
    auto resp = HttpResponse::newHttpResponse();
    resp->setBody("pong");
    resp->setExpiredTime(0);
    callback(resp);
}
void api::Close(req_args) const
{
    bool res;
    if (PASS_HEADER == bank.admin_pass)
    {
        bank.Save();

        res = true;
        app().quit();
    }
    else
    {
        res = false;
    }
    JSON(res);
}
void api::AddUser(req_args, const std::string &name) const
{
    JSON(bank.AddUser(std::move(name), PASS_HEADER));
}
void api::AdminAddUser(req_args, std::string &&name, uint32_t init_bal) const
{
    JSON(bank.AdminAddUser(PASS_HEADER, std::move(name), init_bal, std::string(req->getBody())));
}
void api::DelUser(req_args, const std::string &name) const
{
    JSON(bank.DelUser(name, PASS_HEADER));
}
void api::AdminDelUser(req_args, const std::string &name) const
{
    JSON(bank.AdminDelUser(name, PASS_HEADER));
}
void api::Contains(req_args, const std::string &name) const
{
    JSON(bank.Contains(name));
}
void api::SetBal(req_args, const std::string &name, uint32_t amount) const
{
    JSON(bank.SetBal(name, PASS_HEADER, amount));
}
void api::AdminVerifyPass(req_args)
{
    JSON(bank.AdminVerifyPass(PASS_HEADER));
}
#endif