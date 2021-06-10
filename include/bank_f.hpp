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
        resp->setBody("<body> <h1>ALL FUNCTIONS (that have args) ARE EXPECTING JSON AS DATA TYPE</h1> <h2>/BankF/<span style=\"color: #993300;\">admin</span>/close (POST)</h2> <p><span style=\"background-color: #808080;\">&nbsp;attempt&nbsp;</span> - admin password</p> <blockquote> <p>Closes and Saves the server.</p> </blockquote> <h2>/BankF/user (POST)</h2> <p><span style=\"background-color: #808080;\">&nbsp;name&nbsp;</span> - name of the user being added (must be less then 50 characters)</p> <p><span style=\"background-color: #808080;\">&nbsp;init_pass&nbsp;</span> - initial password for the user being added</p> <blockquote> <p>Adds a user to the bank</p> </blockquote> <h2>/BankF/<span style=\"color: #993300;\">admin</span>/user (POST)</h2> <p><span style=\"background-color: #808080;\">&nbsp;name&nbsp;</span> - name of the user being added</p> <p><span style=\"background-color: #808080;\">&nbsp;attempt&nbsp;</span> - admin password required to add user with balance</p> <p><span style=\"background-color: #808080;\">&nbsp;init_bal&nbsp;</span> - initial balance for user being added</p> <p><span style=\"background-color: #808080;\">&nbsp;init_pass&nbsp;</span> - initial password for user being added</p> <blockquote> <p>Adds a user with initial balance</p> </blockquote> <h2>/BankF/sendfunds (POST)</h2> <p><span style=\"background-color: #808080;\">&nbsp;a_name&nbsp;</span> - sender's name</p> <p><span style=\"background-color: #808080;\">&nbsp;b_name&nbsp;</span> - reciever's name</p> <p><span style=\"background-color: #808080;\">&nbsp;amount&nbsp;</span> - amount being sent</p> <p><span style=\"background-color: #808080;\">&nbsp;attempt&nbsp;</span> - password of sender</p> <blockquote> <p>Sends money from one user to another</p> </blockquote> <h2>/BankF/changepass (PATCH)</h2> <p><span style=\"background-color: #808080;\">&nbsp;name&nbsp;</span> - name of user's password being changes</p> <p><span style=\"background-color: #808080;\">&nbsp;attempt&nbsp;</span> - password of user being changed</p> <p><span style=\"background-color: #808080;\">&nbsp;new_pass&nbsp;</span> - new password to replace the current user's password</p> <blockquote> <p>Changes password of a user, returns -1 if user doesnt exist</p> </blockquote> <h2>/BankF/<span style=\"color: #993300;\">admin</span>/{<span style=\"color: #339966;\">name</span>}/bal (PATCH)</h2> <p><span style=\"background-color: #808080;\">&nbsp;name&nbsp;</span> - the name of the user being set</p> <p><span style=\"background-color: #808080;\">&nbsp;attempt&nbsp;</span> - the admin password required</p> <p><span style=\"background-color: #808080;\">&nbsp;amount&nbsp;</span> - the new balance of the user</p> <blockquote> <p>Sets the balance of a user</p> </blockquote> <h2>/BankF/help (GET)</h2> <blockquote> <p>the page you're looking at right now!</p> </blockquote> <h2>/BankF/vpass (POST)</h2> <p><span style=\"background-color: #808080;\">&nbsp;name&nbsp;</span> - name of user being verified</p> <p><span style=\"background-color: #808080;\">&nbsp;attempt&nbsp;</span> - password being verified</p> <blockquote> <p>returns 0 or 1 based on if [attempt] is equal to the password of the user [name], or -1 if user does not exist. The intended usage for this function is for connected services</p> </blockquote> <h2>/BankF/contains/{<span style=\"color: #339966;\">name</span>} (GET)</h2> <blockquote> <p>returns a 0 or 1 based on if the bank contains the user</p> </blockquote> <h2>/BankF/{<span style=\"color: #339966;\">name</span>}/bal (GET)</h2> <blockquote> <p>returns the balance of a given user's name, if -1 that means the user does not exist</p> </blockquote> <h2>/BankF/<span style=\"color: #993300;\">admin</span>/vpass (POST)</h2><p><span style=\"background-color: #808080;\">&nbsp;attempt&nbsp;</span> - admin password</p> <blockquote> <p>Verifies if password entered is admin password</p> </blockquote> <h2>/BankF/{<span style=\"color: #339966;\">name</span>}/log (POST)</h2><p><span style=\"background-color: #808080;\">&nbsp;attempt&nbsp;</span> - user password</p> <blockquote> <p>returns a list of last 100 transactions, -1 if user not found, 0 if invalid password</p> </blockquote><h2>/BankF/user (DELETE)</h2> <p><span style=\"background-color: #808080;\">&nbsp;name&nbsp;</span> - name of user being deleted</p> <p><span style=\"background-color: #808080;\">&nbsp;attempt&nbsp;</span> - password of user being deleted</p> <blockquote> <p>Deletes a user with the password of the user as verification</p> </blockquote> <h2>/BankF/<span style=\"color: #993300;\">admin</span>/user (DELETE)</h2> <p><span style=\"background-color: #808080;\">&nbsp;name&nbsp;</span> - name of user being deleted</p> <p><span style=\"background-color: #808080;\">&nbsp;attempt&nbsp;</span> - admin password</p> <blockquote> <p>Deletes a user with admin password as verification</p> </blockquote> </body>");
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
    void AdminAddUser(req_args, std::string &&name, uint32_t init_bal, std::string &&init_pass) const
    {
        PASS_HEADER
        JSON(bank.AdminAddUser(pass, std::move(name), init_bal, std::move(init_pass)));
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
    METHOD_ADD(BankF::Close, "/admin/close", Post, Options);
    METHOD_ADD(BankF::AddUser, "/user/{name}", Post, Options);
    METHOD_ADD(BankF::AdminAddUser, "/admin/user/{name}", Post, Options);
    METHOD_ADD(BankF::SendFunds, "{name}/send/{to}/amount={amount}", Post, Options);

    METHOD_ADD(BankF::ChangePassword, "/{name}/pass/change", Patch, Options); //sub optimal
    METHOD_ADD(BankF::SetBal, "/admin/{name}/bal/amount={amount}", Patch, Options);

    METHOD_ADD(BankF::Help, "/help", Get, Options);
    METHOD_ADD(BankF::VerifyPassword, "/{name}/pass/verify", Post, Options); //maybe get?
    METHOD_ADD(BankF::Contains, "/contains/{name}", Get, Options);
    METHOD_ADD(BankF::GetBal, "/{name}/bal", Get, Options);
    METHOD_ADD(BankF::AdminVerifyPass, "/admin/verify", Post, Options); //maybe get?
    METHOD_ADD(BankF::GetLog, "/{name}/log", Post, Options);

    METHOD_ADD(BankF::DelUser, "/user/{name}", Delete, Options);
    METHOD_ADD(BankF::AdminDelUser, "/admin/user/{name}", Delete, Options);
    METHOD_LIST_END
};