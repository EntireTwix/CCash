#include "bank_api.h"

// TODO: parser iterate(input).get(doc) error handling might be superfulous

#define CACHE_FOREVER resp->setExpiredTime(0)

#define CORS resp->addHeader("Access-Control-Allow-Origin", "*")

thread_local ondemand::parser parser;
#define SIMD_JSON_GEN                              \
    simdjson::padded_string input(req->getBody()); \
    ondemand::document doc;

#define RESPONSE_PARSE(R)                               \
    auto resp = HttpResponse::newCustomHttpResponse(R); \
    CORS;                                               \
    callback(resp)

#define RESPOND_TRUE                                                                            \
    auto resp = HttpResponse::newCustomHttpResponse(BankResponse{k204NoContent, std::nullopt}); \
    CORS;                                                                                       \
    CACHE_FOREVER;                                                                              \
    callback(resp)

#define NAME_PARAM req->getParameter("name")

//Usage
void api::GetBal(req_args, const std::string &name)
{
    RESPONSE_PARSE(Bank::GetBal(name));
}

#if MIN_API_SUPPORT == 1
void api::GetLogs(req_args)
{
#if MAX_LOG_SIZE > 0
    RESPONSE_PARSE(Bank::GetLogs(NAME_PARAM));
#else
    auto resp = HttpResponse::newCustomHttpResponse(BankResponse{k404NotFound, "\"Logs are Disabled\""});
    CORS;
    CACHE_FOREVER;
    callback(resp);
#endif
}
#endif

#if (API_VERSION >= 2) && (MIN_API_SUPPORT <= 2)
void api::GetLogsV2(req_args)
{
#if MAX_LOG_SIZE > 0
    RESPONSE_PARSE(Bank::GetLogsV2(NAME_PARAM));
#else
    auto resp = HttpResponse::newCustomHttpResponse(BankResponse{k404NotFound, "\"Logs are Disabled\""});
    CORS;
    CACHE_FOREVER;
    callback(resp);
#endif
}
#endif

void api::SendFunds(req_args)
{
    SIMD_JSON_GEN;
    BankResponse res;
    if (parser.iterate(input).get(doc))
    {
        res = BankResponse{k400BadRequest, "\"Invalid JSON\""};
    }
    else
    {
        std::string_view name;
        uint64_t amount; // as simdjson lacks .get(uint32_t support)
        if (doc["name"].get(name) || doc["amount"].get(amount) || (amount > std::numeric_limits<uint32_t>::max()))
        {
            res = BankResponse{k400BadRequest, "\"Missing/Invalid JSON arg(s)\""};
        }
        else
        {
            StrFromSV_Wrapper name_val(name);
            res = Bank::SendFunds(NAME_PARAM, name_val.str, amount);
        }
    }
    RESPONSE_PARSE(std::move(res));
}
void api::VerifyPassword(req_args) { RESPOND_TRUE; }

//Meta Usage
void api::ChangePassword(req_args)
{
    SIMD_JSON_GEN;
    BankResponse res;
    if (parser.iterate(input).get(doc))
    {
        res = BankResponse{k400BadRequest, "\"Invalid JSON\""};
    }
    else
    {
        std::string_view pass;
        if (doc["time"].get(pass))
        {
            res = BankResponse{k400BadRequest, "\"Missing/Invalid JSON arg(s)\""};
        }
        else
        {
            StrFromSV_Wrapper pass_val(pass);
            Bank::ChangePassword(NAME_PARAM, pass_val.str);
            res = BankResponse{k204NoContent, std::nullopt};
        }
    }
    RESPONSE_PARSE(std::move(res));
}
void api::AdminChangePassword(req_args)
{
    SIMD_JSON_GEN;
    BankResponse res;
    if (parser.iterate(input).get(doc))
    {
        res = BankResponse{k400BadRequest, "\"Invalid JSON\""};
    }
    else
    {
        std::string_view name, pass;
        if (doc["name"].get(name) || doc["time"].get(pass))
        {
            res = BankResponse{k400BadRequest, "\"Missing/Invalid JSON arg(s)\""};
        }
        else
        {
            StrFromSV_Wrapper name_val(name);
            if (Bank::Contains(name_val.str))
            {
                StrFromSV_Wrapper pass_val(pass);
                Bank::ChangePassword(name_val.str, pass_val.str);
                res = BankResponse{k204NoContent, std::nullopt};
            }
            else
            {
                res = BankResponse{k404NotFound, "\"User not found\""};
            }
        }
    }
    RESPONSE_PARSE(std::move(res));
}
void api::SetBal(req_args)
{
    SIMD_JSON_GEN;
    BankResponse res;
    if (parser.iterate(input).get(doc))
    {
        res = BankResponse{k400BadRequest, "\"Invalid JSON\""};
    }
    else
    {        
        std::string_view name;
        uint64_t amount;
        if (doc["name"].get(name) || doc["amount"].get(amount) || (amount > std::numeric_limits<uint32_t>::max()))
        {
            res = BankResponse{k400BadRequest, "\"Missing/Invalid JSON arg(s)\""};
        }
        else
        {
            StrFromSV_Wrapper name_val(name);
            res = Bank::SetBal(name_val.str, amount);
        }
    }
    RESPONSE_PARSE(std::move(res));
}
void api::ImpactBal(req_args)
{
    SIMD_JSON_GEN;
    BankResponse res;
    if (parser.iterate(input).get(doc))
    {
        res = BankResponse{k400BadRequest, "\"Invalid JSON\""};
    }
    else
    {
        std::string_view name;
        int64_t amount;
        if (doc["name"].get(name) || doc["amount"].get(amount))
        {
            res = BankResponse{k400BadRequest, "\"Missing/Invalid JSON arg(s)\""};
        }
        else
        {
            StrFromSV_Wrapper name_val(name);
            res = Bank::ImpactBal(name_val.str, amount);
        }
    }
    RESPONSE_PARSE(std::move(res));
}

//System Usage
void api::Help(req_args)
{
    auto resp = HttpResponse::newRedirectionResponse("https://github.com/EntireTwix/CCash/blob/main/README.md", k301MovedPermanently);
    CACHE_FOREVER;
    callback(resp);
}
void api::Close(req_args)
{
    Bank::Save();
    RESPOND_TRUE; //filter handles admin creds
    app().quit();
}
void api::Contains(req_args, const std::string &name)
{
    BankResponse res;
    if (Bank::Contains(name))
    {
        res = BankResponse{k204NoContent, std::nullopt};
    }
    else
    {
        res = BankResponse{k404NotFound, "\"User not found\""};
    }
    RESPONSE_PARSE(std::move(res));
}
void api::AdminVerifyAccount(req_args)
{
    RESPOND_TRUE; //filter handles admin creds
}
void api::ApiProperties(req_args)
{
    std::string info = "{\"version\":" + std::to_string(API_VERSION) + ",\"min_version\":" + std::to_string(MIN_API_SUPPORT) + ",\"max_log\":" + std::to_string(MAX_LOG_SIZE) + ",\"add_user_open\":" + std::to_string(ADD_USER_OPEN);
    if constexpr (RETURN_ON_DEL)
    {
        info += ",\"return_on_del\":\"" + std::string(return_account) + "\"}";
    }
    else
    {
        info += "}";
    }
    auto resp = HttpResponse::newCustomHttpResponse(BankResponse{k200OK, std::move(info)});
    CORS;
    CACHE_FOREVER;
    callback(resp);
}
void api::PruneUsers(req_args)
{
    SIMD_JSON_GEN;
    BankResponse res;
    if (parser.iterate(input).get(doc))
    {
        res = BankResponse{k400BadRequest, "\"Invalid JSON\""};
    }
    else
    {
#if MAX_LOG_SIZE > 0
        int64_t time;
        uint64_t amount;
        if (doc["time"].get(time) || doc["amount"].get(amount) || (amount > std::numeric_limits<uint32_t>::max()))
        {
            res = BankResponse{k400BadRequest, "\"Missing/Invalid JSON arg(s)\""};
        }
        else
        {
            res = Bank::PruneUsers(time, amount);
        }
#else
        uint64_t amount
        if (doc["amount"].get(amount) || (amount > std::numeric_limits<uint32_t>::max()))
        {
            res = BankResponse{k400BadRequest, "\"Missing/Invalid JSON arg(s)\""};
        }
        else
        {
            res = Bank::PruneUsers(amount);
        }
#endif
    }
    RESPONSE_PARSE(std::move(res));
}

void api::AddUser(req_args)
{
    if constexpr (ADD_USER_OPEN)
    {
        SIMD_JSON_GEN;
        BankResponse res;
        if (parser.iterate(input).get(doc))
        {
            res = BankResponse{k400BadRequest, "\"Invalid JSON\""};
        }
        else
        {
            std::string_view name, pass;
            if (doc["name"].get(name) || doc["pass"].get(pass))
            {
                res = BankResponse{k400BadRequest, "\"Missing/Invalid JSON arg(s)\""};
            }
            else
            {
                StrFromSV_Wrapper name_val(name);
                StrFromSV_Wrapper pass_val(pass);
                res = Bank::AddUser(name_val.str, 0, pass_val.str);
            }
        }
        RESPONSE_PARSE(std::move(res));
    }
    else
    {
        auto resp = HttpResponse::newCustomHttpResponse(BankResponse{k404NotFound, "\"AddUser is Disabled\""});
        CORS;
        CACHE_FOREVER;
        callback(resp);
    }
}
void api::AdminAddUser(req_args)
{
    SIMD_JSON_GEN;
    BankResponse res;
    if (parser.iterate(input).get(doc))
    {
        res = BankResponse{k400BadRequest, "\"Invalid JSON\""};
    }
    else
    {
        std::string_view name;
        uint64_t amount;
        std::string_view pass;
        if (doc["name"].get(name) || doc["amount"].get(amount) || doc["time"].get(pass) || (amount > std::numeric_limits<uint32_t>::max()))
        {
            res = BankResponse{k400BadRequest, "\"Missing/Invalid JSON arg(s)\""};
        }
        else
        {
            StrFromSV_Wrapper name_val(name);
            StrFromSV_Wrapper pass_val(pass);
            res = Bank::AddUser(name_val.str, amount, pass_val.str);
        }
    }
    RESPONSE_PARSE(std::move(res));
}
void api::DelSelf(req_args)
{
    Bank::DelSelf(NAME_PARAM);
    RESPOND_TRUE;
}
void api::AdminDelUser(req_args)
{
    SIMD_JSON_GEN;
    BankResponse res;
    if (parser.iterate(input).get(doc))
    {
        res = BankResponse{k400BadRequest, "\"Invalid JSON\""};
    }
    else
    {
        std::string_view name;
        if (doc["name"].get(name))
        {
            res = BankResponse{k400BadRequest, "\"Missing/Invalid JSON arg(s)\""};
        }
        else
        {
            StrFromSV_Wrapper name_val(name);
            res = Bank::DelUser(name_val.str);
        }
    }
    RESPONSE_PARSE(std::move(res));
}
