#pragma once
#include <drogon/HttpController.h>
#include "bank.h"

using namespace drogon;

#define req_args const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback

class api : public HttpController<api, false>
{
    Bank &bank;

public:
    api(Bank *b);
    void Help(req_args) const;
    void Ping(req_args) const;
    void Close(req_args) const;
    void AddUser(req_args, const std::string &name) const;
    void AdminAddUser(req_args, std::string &&name, uint32_t init_bal) const;
    void DelUser(req_args, const std::string &name) const;
    void AdminDelUser(req_args, const std::string &name) const;
    void SendFunds(req_args, const std::string name, const std::string to, uint32_t amount) const;
    void ChangePassword(req_args, const std::string &name) const;
    void Contains(req_args, const std::string &name) const;
    void GetBal(req_args, const std::string &name) const;
    void VerifyPassword(req_args, const std::string &name) const;
    void SetBal(req_args, const std::string &name, uint32_t amount) const;
    void AdminVerifyPass(req_args);
    void GetLog(req_args, const std::string &name);
    
    METHOD_LIST_BEGIN

    //Usage
    METHOD_ADD(api::GetBal, "/{name}/bal", Get, Options);
    METHOD_ADD(api::GetLog, "/{name}/log", Get, Options);
    METHOD_ADD(api::SendFunds, "/{name}/send/{to}?amount={amount}", Post, Options);
    METHOD_ADD(api::VerifyPassword, "/{name}/pass/verify", Get, Options);

    //Meta Usage
    METHOD_ADD(api::ChangePassword, "/{name}/pass/change", Patch, Options);
    METHOD_ADD(api::SetBal, "/admin/{name}/bal?amount={amount}", Patch, Options);

    //System Usage
    METHOD_ADD(api::Help, "/help", Get, Options);
    METHOD_ADD(api::Ping, "/ping", Get, Options);
    METHOD_ADD(api::Close, "/admin/close", Post, Options);
    METHOD_ADD(api::Contains, "/contains/{name}", Get, Options);
    METHOD_ADD(api::AdminVerifyPass, "/admin/verify", Get, Options);

    //User Managment
    METHOD_ADD(api::AddUser, "/user/{name}", Post, Options);
    METHOD_ADD(api::AdminAddUser, "/admin/user/{name}?init_bal={init_bal}", Post, Options);
    METHOD_ADD(api::DelUser, "/user/{name}", Delete, Options);
    METHOD_ADD(api::AdminDelUser, "/admin/user/{name}", Delete, Options);

    METHOD_LIST_END
};