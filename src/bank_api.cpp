#include "bank_api.h"

#define JSON(V) callback(HttpResponse::newHttpJsonResponse(JsonCast(V))); //temporary
#define PASS_HEADER req->getHeader("Password")                            //temporary

#define GEN_BODY                                \
    const auto temp_req = req->getJsonObject(); \
    const auto body = temp_req ? *temp_req : Json::Value();

#define RESPONSE_PARSE(R)                                                         \
    auto resp = HttpResponse::newHttpJsonResponse(JsonCast(std::move(R.second))); \
    resp->setStatusCode(R.first);                                                 \
    callback(resp);

#define NAME_PARAM req->getBody()

#define NAME_CHECK(suc) BankResponse b = (name == NAME_PARAM ? suc : BankResponse(k400BadRequest, "Requests name must match Auth's name"));

namespace v1
{
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

    api::api(Bank &b) : bank(b) {}

    void api::GetBal(req_args, const std::string &name) const
    {
        RESPONSE_PARSE(bank.GetBal(name));
    }
    void api::GetLog(req_args, const std::string &name)
    {
        if constexpr (max_log_size > 0)
        {
            NAME_CHECK(bank.GetLogs(name));
            RESPONSE_PARSE(b);
        }
        else
        {
            auto resp = HttpResponse::newHttpJsonResponse("Logs are Disabled");
            resp->setStatusCode(k404NotFound);
            resp->setExpiredTime(0); //cached forever
            callback(resp);
        }
    }
    void api::SendFunds(req_args, const std::string &name) const
    {
        GEN_BODY
        NAME_CHECK(bank.SendFunds(name, body["to"].asCString(), body["amount"].asUInt()));
        RESPONSE_PARSE(b);
    }
    void api::VerifyPassword(req_args, const std::string &name) const
    {
        NAME_CHECK(BankResponse(k200OK, true));
        RESPONSE_PARSE(b);
    }

    void api::ChangePassword(req_args, const std::string &name) const
    {
        GEN_BODY
        bank.ChangePassword(name, std::move(body["new_pass"].asCString())); //may make asString()
        NAME_CHECK(BankResponse(k200OK, true));
        RESPONSE_PARSE(b);
    }

    void api::Help(req_args) const
    {
        auto resp = HttpResponse::newHttpResponse();
        resp->setBody("");
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

}