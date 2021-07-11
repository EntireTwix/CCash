#include "bank_api.h"

//all my homies hate jsoncpp

#define CACHE_FOREVER resp->setExpiredTime(0)

#define CORS resp->addHeader("Access-Control-Allow-Origin", "*")

#define GEN_BODY                                                    \
    static thread_local const auto temp_req = req->getJsonObject(); \
    static thread_local const auto body = temp_req ? *temp_req : Json::Value()

static thread_local ondemand::parser parser;
#define SIMD_JSON_GEN                                                  \
    static thread_local simdjson::padded_string input(req->getBody()); \
    static thread_local ondemand::document doc = parser.iterate(input)

#define RESPONSE_PARSE(R)                                                   \
    static thread_local auto resp = HttpResponse::newCustomHttpResponse(R); \
    CORS;                                                                   \
    callback(resp)

#define RESPOND_TRUE                                                                                                \
    static thread_local auto resp = HttpResponse::newCustomHttpResponse(BankResponse(k204NoContent, std::nullopt)); \
    CORS;                                                                                                           \
    CACHE_FOREVER;                                                                                                  \
    callback(resp)

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
        static thread_local auto resp = HttpResponse::newCustomHttpResponse(BankResponse(k404NotFound, "\"Logs are Disabled\""));
        CORS;
        CACHE_FOREVER;
        callback(resp);
    }
}
void api::SendFunds(req_args) const
{
    SIMD_JSON_GEN;
    auto name = doc.find_field("name").get_string();
    auto amount = doc.find_field("amount").get_uint64();
    BankResponse res;
    if (name.error() || amount.error())
    {
        res = BankResponse(k400BadRequest, "Invalid JSON");
    }
    else
    {
        StrFromSV_Wrapper name_val(name.value());
        res = bank.SendFunds(NAME_PARAM, name_val.str, amount.value());
    }
    RESPONSE_PARSE(std::move(res));
}
void api::VerifyPassword(req_args) const { RESPOND_TRUE; }

//Meta Usage
void api::ChangePassword(req_args) const
{
    SIMD_JSON_GEN;
    auto pass = doc.find_field("pass").get_string();
    BankResponse res;
    if (pass.error())
    {
        res = BankResponse(k400BadRequest, "Invalid JSON");
    }
    else
    {
        StrFromSV_Wrapper pass_val(pass.value());
        bank.ChangePassword(NAME_PARAM, std::move(pass_val.str));
    }
    RESPOND_TRUE;
}
void api::AdminChangePassword(req_args) const
{
    SIMD_JSON_GEN;
    auto name = doc.find_field("name").get_string();
    auto pass = doc.find_field("pass").get_string();
    BankResponse res;
    if (name.error() || pass.error())
    {
        res = BankResponse(k400BadRequest, "Invalid JSON");
    }
    else
    {
        StrFromSV_Wrapper name_val(name.value());
        StrFromSV_Wrapper pass_val(pass.value());
        bank.ChangePassword(name_val.str, std::move(pass_val.str));
    }
    RESPOND_TRUE;
}
void api::SetBal(req_args) const
{
    SIMD_JSON_GEN;
    auto name = doc.find_field("name").get_string();
    auto amount = doc.find_field("amount").get_uint64();
    BankResponse res;
    if (name.error() || amount.error())
    {
        res = BankResponse(k400BadRequest, "Invalid JSON");
    }
    else
    {
        StrFromSV_Wrapper name_val(name.value());
        res = bank.SetBal(name_val.str, amount.value());
    }
    RESPONSE_PARSE(std::move(res));
}
void api::ImpactBal(req_args) const
{
    SIMD_JSON_GEN;
    auto name = doc.find_field("name").get_string();
    auto amount = doc.find_field("amount").get_int64();
    BankResponse res;
    if (name.error() || amount.error())
    {
        res = BankResponse(k400BadRequest, "Invalid JSON");
    }
    else
    {
        StrFromSV_Wrapper name_val(name.value());
        res = bank.ImpactBal(name_val.str, amount.value());
    }
    RESPONSE_PARSE(std::move(res));
}

//System Usage
void api::Help(req_args) const
{
    static thread_local auto resp = HttpResponse::newRedirectionResponse("https://github.com/EntireTwix/CCash/blob/README.md");
    CACHE_FOREVER;
    callback(resp);
}
void api::Close(req_args) const
{
    bank.Save();
    RESPOND_TRUE; //filter handles admin creds
    app().quit();
}
void api::Contains(req_args, const std::string &name) const
{
    RESPONSE_PARSE(BankResponse(k200OK, bank.Contains(name) ? "true" : "false"));
}
void api::AdminVerifyAccount(req_args) const
{
    RESPOND_TRUE; //filter handles admin creds
}
void api::ApiProperties(req_args) const
{
    //yet to be converted to simdjson
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

    static thread_local auto resp = HttpResponse::newHttpJsonResponse(std::move(temp));
    CORS;
    CACHE_FOREVER;
    callback(resp);
}
void api::AddUser(req_args) const
{
    SIMD_JSON_GEN;
    auto name = doc.find_field("name").get_string();
    auto pass = doc.find_field("pass").get_string();
    BankResponse res;
    if (name.error() || pass.error())
    {
        res = BankResponse(k400BadRequest, "Invalid JSON");
    }
    else
    {
        StrFromSV_Wrapper name_val(name.value());
        StrFromSV_Wrapper pass_val(pass.value());
        res = bank.AddUser(std::move(name_val.str), 0, std::move(pass_val.str));
    }
    RESPONSE_PARSE(std::move(res));
}
void api::AdminAddUser(req_args) const
{
    SIMD_JSON_GEN;
    auto name = doc.find_field("name").get_string();
    auto amount = doc.find_field("amount").get_uint64();
    auto pass = doc.find_field("pass").get_string();
    BankResponse res;
    if (name.error() || amount.error() || pass.error())
    {
        res = BankResponse(k400BadRequest, "Invalid JSON");
    }
    else
    {
        StrFromSV_Wrapper name_val(name.value());
        StrFromSV_Wrapper pass_val(pass.value());
        res = bank.AddUser(std::move(name_val.str), amount.value(), std::move(pass_val.str));
    }
    RESPONSE_PARSE(std::move(res));
}
void api::DelUser(req_args) const
{
    RESPONSE_PARSE(bank.DelUser(NAME_PARAM));
}
void api::AdminDelUser(req_args) const
{
    SIMD_JSON_GEN;
    auto name = doc.find_field("name").get_string();
    BankResponse res;
    if (name.error())
    {
        res = BankResponse(k400BadRequest, "Invalid JSON");
    }
    else
    {
        StrFromSV_Wrapper name_val(name.value());
        res = bank.DelUser(name_val.str);
    }
    RESPONSE_PARSE(std::move(res));
}
#endif