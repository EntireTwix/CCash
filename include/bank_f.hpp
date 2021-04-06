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
        resp->setBody("<h1>ALL FUNCTIONS (that have args) ARE EXPECTING JSON AS DATA TYPE</h1><h2><br />/BankF/<span style=\"color: #993300;\">admin</span>/close (POST)</h2><p><span style=\"background-color: #808080;\">&nbsp;attempt&nbsp;</span> - admin password</p><blockquote><p>Closes and Saves the server.</p></blockquote><h2><br />/BankF/user (POST)</h2><p><span style=\"background-color: #808080;\">&nbsp;name&nbsp;</span> - name of the user being added</p><p>&nbsp;init_pass - initial password for the user being added</p><blockquote><p>Adds a user to the bank</p></blockquote><h2><br />/BankF/<span style=\"color: #993300;\">admin</span>/user (POST)</h2><p><span style=\"background-color: #808080;\">&nbsp;name&nbsp;</span> - name of the user being added</p><p><span style=\"background-color: #808080;\">&nbsp;attempt&nbsp;</span> - admin password required to add user with balance</p><p><span style=\"background-color: #808080;\">&nbsp;init_bal&nbsp;</span> - initial balance for user being added</p><p><span style=\"background-color: #808080;\">&nbsp;init_pass&nbsp;</span> - initial password for user being added</p><blockquote><p>Adds a user with initial balance</p></blockquote><h2><br />/BankF/sendfunds (POST)</h2><p><span style=\"background-color: #808080;\">&nbsp;a_name&nbsp;</span> - sender's name</p><p><span style=\"background-color: #808080;\">&nbsp;b_name&nbsp;</span> - reciever's name</p><p><span style=\"background-color: #808080;\">&nbsp;amount&nbsp;</span> - amount being sent</p><p><span style=\"background-color: #808080;\">&nbsp;attempt&nbsp;</span> - password of sender</p><blockquote><p>Sends money from one user to another</p></blockquote><h2><br />/BankF/changepass (PATCH)</h2><p><span style=\"background-color: #808080;\">&nbsp;name&nbsp;</span> - name of user's password being changes</p><p><span style=\"background-color: #808080;\">&nbsp;attempt&nbsp;</span> - password of user being changed</p><p><span style=\"background-color: #808080;\">&nbsp;new_pass&nbsp;</span> - new password to replace the current user's password</p><blockquote><p>&nbsp;Changes password of a user</p></blockquote><p>&nbsp;</p><h2>/BankF/{<span style=\"color: #339966;\">name</span>}/bal (PATCH)</h2><p><span style=\"background-color: #808080;\">&nbsp;name&nbsp;</span> - the name of the user being set</p><p><span style=\"background-color: #808080;\">&nbsp;attempt&nbsp;</span> - the admin password required</p><p><span style=\"background-color: #808080;\">&nbsp;amount&nbsp;</span> - the new balance of the user</p><blockquote><p>Sets the balance of a user</p></blockquote><h2>&nbsp;</h2><h2>/BankF/help (GET)</h2><blockquote><p>the page you're looking at right now!</p></blockquote><h2><br />/BankF/vpass (GET)</h2><p><span style=\"background-color: #808080;\">&nbsp;name&nbsp;</span> - name of user being verified</p><p><span style=\"background-color: #808080;\">&nbsp;attempt&nbsp;</span> - password being verified</p><blockquote><p>returns 0 or 1 based on if [attempt] is equal to the password of the user [name]. The intended usage for this function is for connected services</p></blockquote><h2><br />/BankF/contains/{<span style=\"color: #339966;\">name</span>} (GET)</h2><blockquote><p>returns a 0 or 1 based on if the bank contains the user</p></blockquote><p>&nbsp;</p><h2>/BankF/{<span style=\"color: #339966;\">name</span>}/bal (GET)</h2><blockquote><p>returns the balance of a given user's name, if -1 that means the user does not exist</p></blockquote><h2><br />/BankF/user (DELETE)</h2><p><span style=\"background-color: #808080;\">&nbsp;name&nbsp;</span> - name of user being deleted</p><p><span style=\"background-color: #808080;\">&nbsp;attempt&nbsp;</span> - password of user being deleted</p><blockquote><p>Deletes a user with the password of the user as verification</p></blockquote><h2><br />/BankF/<span style=\"color: #993300;\">admin</span>/user (DELETE)</h2><p><span style=\"background-color: #808080;\">&nbsp;name&nbsp;</span> - name of user being deleted</p><p><span style=\"background-color: #808080;\">&nbsp;attempt&nbsp;</span> - admin password</p><blockquote><p>Deletes a user with admin password as verification</p></blockquote>");
        resp->setExpiredTime(0);
        callback(resp);
    }
    void Close(req_args) const
    {
        GEN_BODY
        bool res;
        if (body["attempt"].asCString() == bank.admin_pass)
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
    void AddUser(req_args) const
    {
        GEN_BODY
        JSON(bank.AddUser(body["name"].asCString(), body["init_pass"].asCString()));
    }
    void AdminAddUser(req_args) const
    {
        GEN_BODY
        JSON(bank.AdminAddUser(body["attempt"].asCString(), body["name"].asCString(), body["init_bal"].asUInt(), body["init_pass"].asCString()));
    }
    void DelUser(req_args) const
    {
        GEN_BODY
        JSON(bank.DelUser(body["name"].asCString(), body["attempt"].asCString()));
    }
    void AdminDelUser(req_args) const
    {
        GEN_BODY
        JSON(bank.AdminDelUser(body["name"].asCString(), body["attempt"].asCString()));
    }
    void SendFunds(req_args) const
    {
        GEN_BODY
        JSON(bank.SendFunds(body["a_name"].asCString(), body["b_name"].asCString(), body["amount"].asUInt(), body["attempt"].asCString()));
    }
    void ChangePassword(req_args) const
    {
        GEN_BODY
        JSON(bank.ChangePassword(body["name"].asCString(), body["attempt"].asCString(), body["new_pass"].asCString()));
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
        JSON(bank.VerifyPassword(body["name"].asCString(), body["attempt"].asCString()));
    }
    void SetBal(req_args, const std::string &name) const
    {
        GEN_BODY
        JSON(bank.SetBal(name, body["attempt"].asCString(), body["amount"].asUInt()));
    }

    METHOD_LIST_BEGIN
    METHOD_ADD(BankF::Close, "/admin/close", Post);
    METHOD_ADD(BankF::AddUser, "/user", Post);
    METHOD_ADD(BankF::AdminAddUser, "/admin/user", Post);
    METHOD_ADD(BankF::SendFunds, "/sendfunds", Post);

    METHOD_ADD(BankF::ChangePassword, "/changepass", Patch);
    METHOD_ADD(BankF::SetBal, "/{name}/bal", Patch);

    METHOD_ADD(BankF::Help, "/help", Get);
    METHOD_ADD(BankF::VerifyPassword, "/vpass", Get);
    METHOD_ADD(BankF::Contains, "/contains/{name}", Get);
    METHOD_ADD(BankF::GetBal, "/{name}/bal", Get);

    METHOD_ADD(BankF::DelUser, "/user", Delete);
    METHOD_ADD(BankF::AdminDelUser, "/admin/user", Delete);
    METHOD_LIST_END
};