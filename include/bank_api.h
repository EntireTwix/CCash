#pragma once
#include <drogon/HttpController.h>
#include "json_filter.h"
#include "admin_filter.h"
#include "user_filter.h"

using namespace drogon;

#define req_args const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback

class api : public HttpController<api, false>
{
    Bank &bank;

public:
    api(Bank &b) noexcept;
#if API_VERSION >= 1
    void GetBal(req_args, const std::string &name) const;
    void GetLog(req_args);
    void SendFunds(req_args) const;
    void VerifyPassword(req_args) const;

    void ChangePassword(req_args) const;
    void AdminChangePassword(req_args) const;
    void SetBal(req_args) const;

    void Help(req_args) const;
    void Ping(req_args) const;
    void Close(req_args) const;
    void Contains(req_args, const std::string &name) const;
    void AdminVerifyAccount(req_args) const;
    void ApiVersion(req_args) const;

    void AddUser(req_args) const;
    void AdminAddUser(req_args) const;
    void DelUser(req_args) const;
    void AdminDelUser(req_args) const;
#endif

    METHOD_LIST_BEGIN

#if API_VERSION >= 1
    //Usage
    METHOD_ADD(api::GetBal, "/v1/user/balance?name={name}", Get, Options);
#if MAX_LOG_SIZE > 0
    METHOD_ADD(api::GetLog, "/v1/user/log", Get, Options, "UserFilter");
#else
    METHOD_ADD(api::GetLog, "/v1/user/log", Get, Options);
#endif
    METHOD_ADD(api::SendFunds, "/v1/user/transfer", Post, Options, "JsonFilter", "UserFilter"); //expects ["to"](string) and ["amount"](32 bits)
    METHOD_ADD(api::VerifyPassword, "/v1/user/verify_password", Post, Options, "UserFilter");

    //Meta Usage
    METHOD_ADD(api::ChangePassword, "/v1/user/change_password", Patch, Options, "JsonFilter", "UserFilter");       //expects ["new_pass"](string)
    METHOD_ADD(api::AdminChangePassword, "/v1/user/change_password", Patch, Options, "JsonFilter", "AdminFilter"); //expects ["name"](string) and ["new_pass"](string)
    METHOD_ADD(api::SetBal, "/v1/admin/set_balance", Patch, Options, "JsonFilter", "AdminFilter");                 //expects ["name"](string) and ["amount"](32 bits)

    //System Usage
    METHOD_ADD(api::Help, "/v1/help", Get, Options);
    METHOD_ADD(api::Ping, "/v1/ping", Get, Options);
    METHOD_ADD(api::Close, "/v1/admin/shutdown", Post, Options, "AdminFilter");
    METHOD_ADD(api::Contains, "/v1/user/exists?name={name}", Get, Options);
    METHOD_ADD(api::AdminVerifyAccount, "/v1/admin/verify_account", Post, Options, "AdminFilter");

    //User Managment
    METHOD_ADD(api::AddUser, "/v1/user/register", Post, Options, "JsonFilter");                           //expects ["name"](string) ["pass"](string)
    METHOD_ADD(api::AdminAddUser, "/v1/admin/user/register", Post, Options, "JsonFilter", "AdminFilter"); //expects ["name"](string) ["balance"](32 bits) ["pass"](string)
    METHOD_ADD(api::DelUser, "/v1/delete", Delete, Options, "UserFilter");
    METHOD_ADD(api::AdminDelUser, "/v1/admin/delete", Delete, Options, "JsonFilter", "AdminFilter"); //expects ["name"](string)
#endif
    METHOD_ADD(api::ApiVersion, "/version", Get, Options);

    METHOD_LIST_END
};