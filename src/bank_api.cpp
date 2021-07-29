#include "bank_api.h"

#define CACHE_FOREVER resp->setExpiredTime(0)

#define CORS resp->addHeader("Access-Control-Allow-Origin", "*")

static thread_local ondemand::parser parser;
#define SIMD_JSON_GEN                              \
    simdjson::padded_string input(req->getBody()); \
    auto doc = parser.iterate(input);

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

#if API_VERSION >= 1

//Usage
void api::GetBal(req_args, const std::string &name)
{
    RESPONSE_PARSE(Bank::GetBal(name));
}
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
void api::SendFunds(req_args)
{
    SIMD_JSON_GEN;
    BankResponse res;
    if (doc.error())
    {
        res = BankResponse{k400BadRequest, "\"Invalid JSON\""};
    }
    else
    {
        auto name = doc["name"].get_string();
        auto amount = doc["amount"].get_uint64();
        if (name.error() || amount.error())
        {
            res = BankResponse{k400BadRequest, "\"Missing JSON arg(s)\""};
        }
        else
        {
            StrFromSV_Wrapper name_val(name.value());
            res = Bank::SendFunds(NAME_PARAM, name_val.str, amount.value());
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
    if (doc.error())
    {
        res = BankResponse{k400BadRequest, "\"Invalid JSON\""};
    }
    else
    {
        auto pass = doc["pass"].get_string();
        if (pass.error())
        {
            res = BankResponse{k400BadRequest, "\"Missing JSON arg(s)\""};
        }
        else
        {
            StrFromSV_Wrapper pass_val(pass.value());
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
    if (doc.error())
    {
        res = BankResponse{k400BadRequest, "\"Invalid JSON\""};
    }
    else
    {
        auto name = doc["name"].get_string();
        auto pass = doc["pass"].get_string();
        if (name.error() || pass.error())
        {
            res = BankResponse{k400BadRequest, "\"Missing JSON arg(s)\""};
        }
        else
        {
            StrFromSV_Wrapper name_val(name.value());
            if (Bank::Contains(name_val.str))
            {
                StrFromSV_Wrapper pass_val(pass.value());
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
    if (doc.error())
    {
        res = BankResponse{k400BadRequest, "\"Invalid JSON\""};
    }
    else
    {
        auto name = doc["name"].get_string();
        auto amount = doc["amount"].get_uint64();
        if (name.error() || amount.error())
        {
            res = BankResponse{k400BadRequest, "\"Missing JSON arg(s)\""};
        }
        else
        {
            StrFromSV_Wrapper name_val(name.value());
            res = Bank::SetBal(name_val.str, amount.value());
        }
    }
    RESPONSE_PARSE(std::move(res));
}
void api::ImpactBal(req_args)
{
    SIMD_JSON_GEN;
    BankResponse res;
    if (doc.error())
    {
        res = BankResponse{k400BadRequest, "\"Invalid JSON\""};
    }
    else
    {
        auto name = doc["name"].get_string();
        auto amount = doc["amount"].get_int64();
        if (name.error() || amount.error())
        {
            res = BankResponse{k400BadRequest, "\"Missing JSON arg(s)\""};
        }
        else
        {
            StrFromSV_Wrapper name_val(name.value());
            res = Bank::ImpactBal(name_val.str, amount.value());
        }
    }
    RESPONSE_PARSE(std::move(res));
}

//System Usage
void api::Help(req_args)
{
    auto resp = HttpResponse::newRedirectionResponse("https://github.com/EntireTwix/CCash/blob/README.md", k301MovedPermanently);
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
    std::string info = "{\"version\":" + std::to_string(API_VERSION) + ",\"max_log\":" + std::to_string(MAX_LOG_SIZE);
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
    if (doc.error())
    {
        res = BankResponse{k400BadRequest, "\"Invalid JSON\""};
    }
    else
    {
#if MAX_LOG_SIZE > 0
        auto time = doc["time"].get_int64();
        auto amount = doc["amount"].get_uint64();
        if (time.error() || amount.error())
        {
            res = BankResponse{k400BadRequest, "\"Missing JSON arg(s)\""};
        }
        else
        {
            res = Bank::PruneUsers(time.value(), amount.value());
        }
#else
        auto amount = doc["amount"].get_uint64();
        if (amount.error())
        {
            res = BankResponse{k400BadRequest, "\"Missing JSON arg(s)\""};
        }
        else
        {
            res = Bank::PruneUsers(amount.value());
        }
#endif
    }
    RESPONSE_PARSE(std::move(res));
}

void api::AddUser(req_args)
{
    SIMD_JSON_GEN;
    BankResponse res;
    if (doc.error())
    {
        res = BankResponse{k400BadRequest, "\"Invalid JSON\""};
    }
    else
    {
        auto name = doc["name"].get_string();
        auto pass = doc["pass"].get_string();
        if (name.error() || pass.error())
        {
            res = BankResponse{k400BadRequest, "\"Missing JSON arg(s)\""};
        }
        else
        {
            StrFromSV_Wrapper name_val(name.value());
            StrFromSV_Wrapper pass_val(pass.value());
            res = Bank::AddUser(name_val.str, 0, pass_val.str);
        }
    }
    RESPONSE_PARSE(std::move(res));
}
void api::AdminAddUser(req_args)
{
    SIMD_JSON_GEN;
    BankResponse res;
    if (doc.error())
    {
        res = BankResponse{k400BadRequest, "\"Invalid JSON\""};
    }
    else
    {
        auto name = doc["name"].get_string();
        auto amount = doc["amount"].get_uint64();
        auto pass = doc["pass"].get_string();
        if (name.error() || amount.error() || pass.error())
        {
            res = BankResponse{k400BadRequest, "\"Missing JSON arg(s)\""};
        }
        else
        {
            StrFromSV_Wrapper name_val(name.value());
            StrFromSV_Wrapper pass_val(pass.value());
            res = Bank::AddUser(name_val.str, amount.value(), pass_val.str);
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
    if (doc.error())
    {
        res = BankResponse{k400BadRequest, "\"Invalid JSON\""};
    }
    else
    {
        auto name = doc["name"].get_string();
        if (name.error())
        {
            res = BankResponse{k400BadRequest, "\"Missing JSON arg(s)\""};
        }
        else
        {
            StrFromSV_Wrapper name_val(name.value());
            res = Bank::DelUser(name_val.str);
        }
    }
    RESPONSE_PARSE(std::move(res));
}
#endif
