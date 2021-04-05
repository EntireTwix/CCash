#pragma once
#include <drogon/HttpController.h>
#include "bank.hpp"

using namespace drogon;

#define req_args const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback
#define JSON(V) callback(HttpResponse::newHttpJsonResponse(JsonReturn(V)));
#define INLINE __attribute__((always_inline)) inline

template <typename T>
INLINE Json::Value JsonReturn(T &&val)
{
    Json::Value res;
    std::string reason;
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
        resp->setBody("");
        resp->setExpiredTime(0);
        callback(resp);
    }
    void Close(req_args, const std::string &attempt) const
    {
        auto resp = HttpResponse::newHttpResponse();
        if (attempt == bank.admin_pass)
        {
            resp->setBody("<p><span style=" color
                          : #339966;
                          "><strong>[Webserver Closed]</strong></span></p>");
            bank.Save();
            callback(resp);
            app().quit();
        }
        else
        {
            resp->setBody("<p><span style=" color
                          : #ff0000;
                          "><strong>[Invalid Password]</strong></span></p>");
            callback(resp);
        }
    }
    void AddUser(req_args, const std::string &name, std::string &&init_pass) const { JSON(bank.AddUser(name, std::move(init_pass))); }
    void AdminAddUser(req_args, const std::string &attempt, std::string &&name, uint_fast32_t init_bal, std::string &&init_pass) const { JSON(bank.AdminAddUser(attempt, std::move(name), init_bal, std::move(init_pass))); }

    void DelUser(req_args, const std::string &name, const std::string &attempt) const { JSON(bank.DelUser(name, attempt)); }
    void AdminDelUser(req_args, const std::string &name, const std::string &attempt) const { JSON(bank.AdminDelUser(name, attempt)); }

    void SendFunds(req_args, const std::string &a_name, const std::string &b_name, uint_fast32_t amount, const std::string &attempt) const { JSON(bank.SendFunds(a_name, b_name, amount, attempt)); }
    void ChangePassword(req_args, const std::string &name, const std::string &attempt, std::string &&new_pass) const { JSON(bank.ChangePassword(name, attempt, std::move(new_pass))); }

    void Contains(req_args, const std::string &name) const { JSON(bank.Contains(name)); }
    void GetBal(req_args, const std::string &name) const { JSON(bank.GetBal(name)); }
    void VerifyPassword(req_args, const std::string &name, const std::string &attempt) const { JSON(bank.VerifyPassword(name, attempt)); }

    METHOD_LIST_BEGIN
    METHOD_ADD(BankF::Help, "/help", Get);
    //gotta add bindings here
    METHOD_LIST_END
};