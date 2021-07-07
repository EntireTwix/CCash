#include "bank_api.h"

//all my homies hate jsoncpp

#define CACHE_FOREVER resp->setExpiredTime(0);

#define CORS resp->addHeader("Access-Control-Allow-Origin", "*")

#define GEN_BODY                                \
    const auto temp_req = req->getJsonObject(); \
    const auto body = temp_req ? *temp_req : Json::Value();

#define RESPONSE_PARSE(R)                                     \
    const auto resp = HttpResponse::newCustomHttpResponse(R); \
    CORS;                                                     \
    callback(resp);

#define RESPOND_TRUE                                               \
    const auto resp = HttpResponse::newCustomHttpResponse("true"); \
    CORS;                                                          \
    CACHE_FOREVER                                                  \
    callback(resp);

#define NAME_PARAM req->getParameter("name")

api::api(Bank &b) noexcept : bank(b)
{
}

#if API_VERSION >= 1

//Usage
void api::GetBal(req_args, const std::string &name) const
{
    RESPONSE_PARSE(bank.GetBal(name));
}
void api::GetLogs(req_args)
{
    if constexpr (MAX_LOG_SIZE > 0)
    {
        RESPONSE_PARSE(bank.GetLogs(NAME_PARAM));
    }
    else
    {
        auto resp = HttpResponse::newCustomHttpResponse(BankResponse(k404NotFound, "\"Logs are Disabled\""));
        CORS;
        CACHE_FOREVER
        callback(resp);
    }
}
void api::SendFunds(req_args) const
{
    GEN_BODY
    RESPONSE_PARSE(bank.SendFunds(NAME_PARAM, body["name"].asCString(), body["amount"].asUInt()));
}
void api::VerifyPassword(req_args) const { RESPOND_TRUE }

//Meta Usage
void api::ChangePassword(req_args) const
{
    GEN_BODY
    bank.ChangePassword(NAME_PARAM, std::move(body["pass"].asCString()));
    RESPOND_TRUE
}
void api::AdminChangePassword(req_args) const
{
    GEN_BODY
    bank.ChangePassword(body["name"].asCString(), std::move(body["pass"].asCString()));
    RESPOND_TRUE
}
void api::SetBal(req_args) const
{
    GEN_BODY
    RESPONSE_PARSE(bank.SetBal(body["name"].asCString(), body["amount"].asUInt()));
}
void api::ImpactBal(req_args) const
{
    GEN_BODY
    RESPONSE_PARSE(bank.ImpactBal(body["name"].asCString(), body["amount"].asInt64()));
}

//System Usage
void api::Help(req_args) const
{
    auto resp = HttpResponse::newRedirectionResponse("https://github.com/EntireTwix/CCash/blob/Refractor/README.md"); //may make README.md
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
    const auto resp = HttpResponse::newCustomHttpResponse(bank.Contains(name) ? "true" : "false");
    CORS;
    callback(resp);
}
void api::AdminVerifyAccount(req_args) const
{
    RESPOND_TRUE //filter handles admin creds
}
void api::ApiProperties(req_args) const
{
    Json::Value temp;
    temp["version"] = API_VERSION;
    temp["max_log"] = MAX_LOG_SIZE;
    temp["min_name"] = min_name_size;
    temp["max_name"] = max_name_size;
    temp["return_on_del"] = RETURN_ON_DEL;
    if constexpr (RETURN_ON_DEL)
    {
        temp["return_on_del_acc"] = return_account;
    }

    auto resp = HttpResponse::newHttpJsonResponse(std::move(temp));
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
    RESPONSE_PARSE(bank.AddUser(body["name"].asCString(), body["amount"].asUInt(), body["pass"].asCString()))
}
void api::DelUser(req_args) const
{
    GEN_BODY
    RESPONSE_PARSE(bank.DelUser(NAME_PARAM))
}
void api::AdminDelUser(req_args) const
{
    GEN_BODY
    RESPONSE_PARSE(bank.DelUser(body["name"].asCString()))
}
#endif