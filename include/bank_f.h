#pragma once
#include <drogon/HttpController.h>
#include "bank.h"

using namespace drogon;

#define req_args const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback

class BankF : public HttpController<BankF, false>
{
    Bank &bank;

public:
    BankF(Bank *b);
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
    METHOD_ADD(BankF::GetBal, "/{name}/bal", Get, Options);
    METHOD_ADD(BankF::GetLog, "/{name}/log", Get, Options);
    METHOD_ADD(BankF::SendFunds, "/{name}/send/{to}/amount={amount}", Post, Options);
    METHOD_ADD(BankF::VerifyPassword, "/{name}/pass/verify", Get, Options);

    //Meta Usage
    METHOD_ADD(BankF::ChangePassword, "/{name}/pass/change", Patch, Options);
    METHOD_ADD(BankF::SetBal, "/admin/{name}/bal/amount={amount}", Patch, Options);

    //System Usage
    METHOD_ADD(BankF::Help, "/help", Get, Options);
    METHOD_ADD(BankF::Ping, "/ping", Get, Options);
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