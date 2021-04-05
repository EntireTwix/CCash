#pragma once
#include <drogon/HttpController.h>
#include "bank.hpp"

using namespace drogon;

#define req_args const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback
#define JSON(V) callback(HttpResponse::newHttpJsonResponse(JsonReturn(V)));
#define INLINE __attribute__((always_inline)) inline
#define GEN_BODY                                \
    const auto temp_req = req->getJsonObject(); \
    const auto body = temp_req ? *temp_req : Json::Value();

template <typename T>
INLINE Json::Value JsonReturn(T &&val)
{
    Json::Value res;
    if constexpr (std::is_same_v<T, bool>)
    {
        res["value"] = (int)val; //becuase of json lib interpreting 67 as 'A' for example
    }
    else
    {
        res["value"] = val;
    }
    return res;
}

class BankF : public HttpController<BankF, false>
{
public:
    void Help(req_args) const
    {
        auto resp = HttpResponse::newHttpResponse();
        auto handlerInfo = app().getHandlersInfo();
        std::string funcs;
        for (auto &info : handlerInfo)
        {
            funcs += std::get<0>(info);
            switch (std::get<1>(info))
            {
            case Get:
                funcs += " (GET) ";
                break;
            case Post:
                funcs += " (POST) ";
                break;
            case Delete:
                funcs += " (DELETE) ";
                break;
            case Put:
                funcs += " (PUT) ";
                break;
            case Options:
                funcs += " (OPTIONS) ";
                break;
            case Head:
                funcs += " (HEAD) ";
                break;
            case Patch:
                funcs += " (PATCH) ";
                break;
            default:
                break;
            }
            funcs += "<br>";
        }
        resp->setBody(funcs);
        resp->setExpiredTime(0);
        callback(resp);
    }
    void Close(req_args) const
    {
        GEN_BODY
        if (body["attempt"].asString() == bank.admin_pass)
        {
            bank.Save();
            JSON(true);
            app().quit();
        }
        else
        {
            JSON(false);
        }
    }
    void AddUser(req_args) const
    {
        GEN_BODY
        JSON(bank.AddUser(body["name"].asString(), body["init_pass"].asString()));
    }
    void AdminAddUser(req_args) const
    {
        GEN_BODY
        JSON(bank.AdminAddUser(body["attempt"].asString(), body["name"].asString(), body["init_bal"].asUInt(), body["init_pass"].asString()));
    }
    void DelUser(req_args) const
    {
        GEN_BODY
        JSON(bank.DelUser(body["name"].asString(), body["attempt"].asString()));
    }
    void AdminDelUser(req_args) const
    {
        GEN_BODY
        JSON(bank.AdminDelUser(body["name"].asString(), body["attempt"].asString()));
    }
    void SendFunds(req_args) const
    {
        GEN_BODY
        JSON(bank.SendFunds(body["a_name"].asString(), body["b_name"].asString(), body["amount"].asUInt(), body["attempt"].asString()));
    }
    void ChangePassword(req_args) const
    {
        GEN_BODY
        JSON(bank.ChangePassword(body["name"].asString(), body["attempt"].asString(), body["new_pass"].asString()));
    }
    void Contains(req_args, const std::string &name) const
    {
        JSON(bank.Contains(name));
    }
    void GetBal(req_args, const std::string &name) const
    {
        JSON(bank.GetBal(name));
    }
    void VerifyPassword(req_args) const
    {
        GEN_BODY
        JSON(bank.VerifyPassword(body["name"].asString(), body["attempt"].asString()));
    }

    METHOD_LIST_BEGIN
    METHOD_ADD(BankF::Help, "/help", Get);
    METHOD_ADD(BankF::Close, "/close", Post);
    METHOD_ADD(BankF::AddUser, "/addusr", Post);
    METHOD_ADD(BankF::AdminAddUser, "/admin/addusr", Post);
    METHOD_ADD(BankF::DelUser, "/delusr", Delete);
    METHOD_ADD(BankF::AdminDelUser, "/admin/delusr", Delete);
    METHOD_ADD(BankF::SendFunds, "/sendfunds", Post);
    METHOD_ADD(BankF::ChangePassword, "/changepass", Post);
    METHOD_ADD(BankF::Contains, "/contains/{name}", Get);
    METHOD_ADD(BankF::GetBal, "/getbal/{name}", Get);
    METHOD_ADD(BankF::VerifyPassword, "/vpass", Get);
    METHOD_LIST_END
};