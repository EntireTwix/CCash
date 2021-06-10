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
#define PASS_HEADER std::string pass = req->getHeader("Password");

template <typename T>
INLINE Json::Value JsonReturn(T &&val)
{
    Json::Value res;
    if constexpr (std::is_same_v<T, int_fast8_t>)
    {
        res["value"] = (int)val; //becuase of json lib interpreting 67 as 'A' for example
    }
    else if constexpr (std::is_same_v<T, uint64_t>)
    {
        res["value"] = (Json::Int64)val;
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
        resp->setBody("<h1 class=\"code-line\" data-line-start=0 data-line-end=1 ><a id=\"Error_Responses_0\"></a>Error Responses</h1> <table class=\"table table-striped table-bordered\"> <thead> <tr> <th>#</th> <th>meaning</th> </tr> </thead> <tbody> <tr> <td>-1</td> <td>UserNotFound</td> </tr> <tr> <td>-2</td> <td>WrongPassword</td> </tr> <tr> <td>-3</td> <td>InvalidRequest</td> </tr> <tr> <td>-4</td> <td>WrongAdminPassword</td> </tr> <tr> <td>-5</td> <td>NameTooLong</td> </tr> <tr> <td>-6</td> <td>UserAlreadyExists</td> </tr> <tr> <td>-7</td> <td>InsufficientFunds</td> </tr> </tbody> </table> <h1 class=\"code-line\" data-line-start=12 data-line-end=13 ><a id=\"Things_of_Note_12\"></a>Things of Note</h1> <ul> <li class=\"has-line-data\" data-line-start=\"13\" data-line-end=\"14\">all endpoints respond with <strong>JSON</strong> file type</li> <li class=\"has-line-data\" data-line-start=\"14\" data-line-end=\"16\">&quot;<strong>A</strong>&quot; denotes requiring Authentication in the form of a header titled &quot;<strong>Password</strong>&quot;</li> </ul> <h1 class=\"code-line\" data-line-start=16 data-line-end=17 ><a id=\"Usage_16\"></a>Usage</h1> <table class=\"table table-striped table-bordered\"> <thead> <tr> <th style=\"text-align:center\">Name</th> <th style=\"text-align:left\">Path</th> <th style=\"text-align:center\">Method</th> <th style=\"text-align:center\">A</th> <th>Description</th> </tr> </thead> <tbody> <tr> <td style=\"text-align:center\">GetBal</td> <td style=\"text-align:left\">/{name}/bal</td> <td style=\"text-align:center\">GET</td> <td style=\"text-align:center\">true</td> <td>returns the balance of a given user <code>{name}</code></td> </tr> <tr> <td style=\"text-align:center\">GetLog</td> <td style=\"text-align:left\">/{name}/log</td> <td style=\"text-align:center\">GET</td> <td style=\"text-align:center\">true</td> <td>returns a list of last <code>n</code> number of transactions (a configurable amount) of a given user <code>{name}</code></td> </tr> <tr> <td style=\"text-align:center\">SendFunds</td> <td style=\"text-align:left\">/{name}/send/{to}/amount={amount}</td> <td style=\"text-align:center\">POST</td> <td style=\"text-align:center\">false</td> <td>sends <code>{amount}</code> from user <code>{name}</code> to user <code>{to}</code></td> </tr> <tr> <td style=\"text-align:center\">VerifyPassword</td> <td style=\"text-align:left\">/{name}/pass/verify</td> <td style=\"text-align:center\">GET</td> <td style=\"text-align:center\">true</td> <td>returns <code>true</code> or <code>false</code> depending on if the supplied user <code>{name}</code>'s password matches the password supplied in the header</td> </tr> </tbody> </table> <h1 class=\"code-line\" data-line-start=24 data-line-end=25 ><a id=\"Meta_Usage_24\"></a>Meta Usage</h1> <table class=\"table table-striped table-bordered\"> <thead> <tr> <th style=\"text-align:center\">Name</th> <th style=\"text-align:left\">Path</th> <th style=\"text-align:center\">Method</th> <th style=\"text-align:center\">A</th> <th>Description</th> </tr> </thead> <tbody> <tr> <td style=\"text-align:center\">ChangePassword</td> <td style=\"text-align:left\">/{name}/pass/change</td> <td style=\"text-align:center\">PATCH</td> <td style=\"text-align:center\">true</td> <td>if the password supplied in the header matches the user <code>{name}</code>'s password, the user’s password is changed to the one given in the body</td> </tr> <tr> <td style=\"text-align:center\">SetBal</td> <td style=\"text-align:left\">/admin/{name}/bal/amount={amount}</td> <td style=\"text-align:center\">PATCH</td> <td style=\"text-align:center\">true</td> <td>sets the balance of a give user <code>{name}</code> if the supplied password matches the admin password</td> </tr> </tbody> </table> <h1 class=\"code-line\" data-line-start=30 data-line-end=31 ><a id=\"System_Usage_30\"></a>System Usage</h1> <table class=\"table table-striped table-bordered\"> <thead> <tr> <th style=\"text-align:center\">Name</th> <th style=\"text-align:left\">Path</th> <th style=\"text-align:center\">Method</th> <th style=\"text-align:center\">A</th> <th>Description</th> </tr> </thead> <tbody> <tr> <td style=\"text-align:center\">Help</td> <td style=\"text-align:left\">/help</td> <td style=\"text-align:center\">GET</td> <td style=\"text-align:center\">false</td> <td>the page you’re looking at right now!</td> </tr> <tr> <td style=\"text-align:center\">Close</td> <td style=\"text-align:left\">/admin/close</td> <td style=\"text-align:center\">POST</td> <td style=\"text-align:center\">true</td> <td>saves and then closes the program if the supplied password matches the admin password</td> </tr> <tr> <td style=\"text-align:center\">Contains</td> <td style=\"text-align:left\">/contains/{name}</td> <td style=\"text-align:center\">GET</td> <td style=\"text-align:center\">false</td> <td>returns <code>true</code> or <code>false</code> depending on if the supplied user <code>{name}</code> exists</td> </tr> <tr> <td style=\"text-align:center\">AdminVerifyPass</td> <td style=\"text-align:left\">/admin/verify</td> <td style=\"text-align:center\">GET</td> <td style=\"text-align:center\">true</td> <td>returns <code>true</code> or <code>false</code> depending on if the password supplied in the header matches the admin password</td> </tr> </tbody> </table> <h1 class=\"code-line\" data-line-start=38 data-line-end=39 ><a id=\"User_Management_38\"></a>User Management</h1> <table class=\"table table-striped table-bordered\"> <thead> <tr> <th style=\"text-align:center\">Name</th> <th style=\"text-align:left\">Path</th> <th style=\"text-align:center\">Method</th> <th style=\"text-align:center\">A</th> <th>Description</th> </tr> </thead> <tbody> <tr> <td style=\"text-align:center\">AddUser</td> <td style=\"text-align:left\">/user/{name}</td> <td style=\"text-align:center\">POST</td> <td style=\"text-align:center\">true</td> <td>registers a user with the name <code>{name}</code>, balance of 0 and a password of the password supplied in the header</td> </tr> <tr> <td style=\"text-align:center\">AdminAddUser</td> <td style=\"text-align:left\">/admin/user/{name}?init_bal={init_bal}</td> <td style=\"text-align:center\">POST</td> <td style=\"text-align:center\">true</td> <td>if the password supplied in the header matches the admin password, then it registers a user with the name <code>{name}</code>, balance of <code>init_bal</code> and a password supplied by the body of the request</td> </tr> <tr> <td style=\"text-align:center\">DelUser</td> <td style=\"text-align:left\">/user/{name}</td> <td style=\"text-align:center\">DELETE</td> <td style=\"text-align:center\">true</td> <td>if the password supplied in the header matches the user <code>{name}</code>'s password, then the user is deleted</td> </tr> <tr> <td style=\"text-align:center\">AdminDelUser</td> <td style=\"text-align:left\">/admin/user/{name}</td> <td style=\"text-align:center\">DELETE</td> <td style=\"text-align:center\">true</td> <td>if the password supplied in the header matches the admin password, then the user is deleted</td> </tr> </tbody> </table>");
        resp->setExpiredTime(0);
        callback(resp);
    }
    void Close(req_args) const
    {
        PASS_HEADER
        bool res;
        if (pass == bank.admin_pass)
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
    void AddUser(req_args, std::string &&name) const
    {
        PASS_HEADER
        JSON(bank.AddUser(std::move(name), std::move(pass)));
    }
    void AdminAddUser(req_args, std::string &&name, uint32_t init_bal) const
    {
        PASS_HEADER
        JSON(bank.AdminAddUser(pass, std::move(name), init_bal, std::string(req->getBody())));
    }
    void DelUser(req_args, const std::string &name) const
    {
        PASS_HEADER
        JSON(bank.DelUser(name, pass));
    }
    void AdminDelUser(req_args, const std::string &name) const
    {
        PASS_HEADER
        JSON(bank.AdminDelUser(name, pass));
    }
    void SendFunds(req_args, const std::string name, const std::string to, uint32_t amount) const
    {
        PASS_HEADER
        JSON(bank.SendFunds(name, to, amount, pass));
    }
    void ChangePassword(req_args, const std::string &name) const
    {
        PASS_HEADER
        JSON(bank.ChangePassword(name, pass, std::string(req->getBody())));
    }
    void Contains(req_args, const std::string &name) const
    {
        JSON(bank.Contains(name));
    }
    void GetBal(req_args, const std::string &name) const
    {
        JSON(bank.GetBal(name));
    }
    void VerifyPassword(req_args, const std::string &name) const
    {
        PASS_HEADER
        JSON(bank.VerifyPassword(name, pass));
    }
    void SetBal(req_args, const std::string &name, uint32_t amount) const
    {
        PASS_HEADER
        JSON(bank.SetBal(name, pass, amount));
    }
    void AdminVerifyPass(req_args)
    {
        PASS_HEADER
        JSON(bank.AdminVerifyPass(pass));
    }
    void GetLog(req_args, const std::string &name)
    {
        if constexpr (max_log_size)
        {
            PASS_HEADER
            JSON(bank.GetLogs(name, pass));
        }
        else
        {
            auto resp = HttpResponse::newHttpJsonResponse(JsonReturn("Logs are Disabled"));
            resp->setExpiredTime(0); //cached forever
            callback(resp);
        }
    }

    METHOD_LIST_BEGIN

    //Usage
    METHOD_ADD(BankF::GetBal, "/{name}/bal", Get, Options);
    METHOD_ADD(BankF::GetLog, "/{name}/log", Get, Options);
    METHOD_ADD(BankF::SendFunds, "/{name}/send/{to}/amount={amount}", Post, Options);
    METHOD_ADD(BankF::VerifyPassword, "/{name}/pass/verify", Get, Options);

    //Meta Usage
    METHOD_ADD(BankF::ChangePassword, "/{name}/pass/change", Patch, Options);
    METHOD_ADD(BankF::SetBal, "/admin/{name}/bal/amount={amount}", Patch, Options);

    //System Usage
    METHOD_ADD(BankF::Help, "/help", Get, Options);
    METHOD_ADD(BankF::Close, "/admin/close", Post, Options);
    METHOD_ADD(BankF::Contains, "/contains/{name}", Get, Options);
    METHOD_ADD(BankF::AdminVerifyPass, "/admin/verify", Get, Options);

    //User Managment
    METHOD_ADD(BankF::AddUser, "/user/{name}", Post, Options);
    METHOD_ADD(BankF::AdminAddUser, "/admin/user/{name}?init_bal={init_bal}", Post, Options);
    METHOD_ADD(BankF::DelUser, "/user/{name}", Delete, Options);
    METHOD_ADD(BankF::AdminDelUser, "/admin/user/{name}", Delete, Options);

    METHOD_LIST_END
};