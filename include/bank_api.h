#pragma once
#include <drogon/HttpController.h>
#include "str_intrusion.h"
#include "json_filter.h"
#include "user_filter.h"

using namespace drogon;

#define req_args const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback

class api : public HttpController<api, false>
{
    Bank &bank;

public:
    api(Bank &b) noexcept;
    void JsonCpp(req_args) const;
    void Json(req_args) const;

#if API_VERSION >= 1
    void GetBal(req_args, const std::string &name) const;
    void GetLogs(req_args);
    void SendFunds(req_args) const;
    void VerifyPassword(req_args) const;

    void ChangePassword(req_args) const;
    void AdminChangePassword(req_args) const;
    void SetBal(req_args) const;
    void ImpactBal(req_args) const;

    void Help(req_args) const;
    void Close(req_args) const;
    void Contains(req_args, const std::string &name) const;
    void AdminVerifyAccount(req_args) const;
    void ApiProperties(req_args) const;

    void AddUser(req_args) const;
    void AdminAddUser(req_args) const;
    void DelSelf(req_args) const;
    void AdminDelUser(req_args) const;

#endif

    METHOD_LIST_BEGIN

#if API_VERSION >= 1
    //Usage
    METHOD_ADD(api::GetBal, "/v1/user/balance?name={name}", Get, Options, "JsonFilter<false>");
#if MAX_LOG_SIZE > 0
    METHOD_ADD(api::GetLogs, "/v1/user/log", Get, Options, "JsonFilter<false>", "UserFilter<true, false>");
#else
    METHOD_ADD(api::GetLogs, "/v1/user/log", Get, Options, "JsonFilter<false>");
#endif
    METHOD_ADD(api::SendFunds, "/v1/user/transfer", Post, Options, "JsonFilter<true>", "UserFilter<true, false>"); //expects ["name"](string) and ["amount"](32 bits)
    METHOD_ADD(api::VerifyPassword, "/v1/user/verify_password", Post, Options, "UserFilter<false, false>", "JsonFilter<false>");

    //Meta Usage
    METHOD_ADD(api::ChangePassword, "/v1/user/change_password", Patch, Options, "JsonFilter<true>", "UserFilter<true, false>");            //expects ["pass"](string)
    METHOD_ADD(api::AdminChangePassword, "/v1/admin/user/change_password", Patch, Options, "JsonFilter<true>", "UserFilter<false, true>"); //expects ["name"](string) and ["pass"](string)
    METHOD_ADD(api::SetBal, "/v1/admin/set_balance", Patch, Options, "JsonFilter<true>", "UserFilter<false, true>");                       //expects ["name"](string) and ["amount"](32 bits)
    METHOD_ADD(api::ImpactBal, "/v1/admin/impact_balance", Post, Options, "JsonFilter<true>", "UserFilter<false, true>");                  //expects ["name"](string) and ["amount"](32 bits)

    //System Usage
    METHOD_ADD(api::Help, "/v1/help", Get, Options);
    METHOD_ADD(api::Close, "/v1/admin/shutdown", Post, Options, "UserFilter<false, true>", "JsonFilter<false>");
    METHOD_ADD(api::Contains, "/v1/user/exists?name={name}", Get, Options, "JsonFilter<false>");
    METHOD_ADD(api::AdminVerifyAccount, "/v1/admin/verify_account", Post, Options, "UserFilter<false, true>", "JsonFilter<false>");

    //User Managment
    METHOD_ADD(api::AddUser, "/v1/user/register", Post, Options);                                                           //expects ["name"](string) ["pass"](string)
    METHOD_ADD(api::AdminAddUser, "/v1/admin/user/register", Post, Options, "JsonFilter<true>", "UserFilter<false, true>"); //expects ["name"](string) ["amount"](32 bits) ["pass"](string)
    METHOD_ADD(api::DelSelf, "/v1/user/delete", Delete, Options, "UserFilter<true, false>", "JsonFilter<false>");
    METHOD_ADD(api::AdminDelUser, "/v1/admin/user/delete", Delete, Options, "JsonFilter<true>", "UserFilter<false, true>"); //expects ["name"](string)
#endif
    METHOD_ADD(api::ApiProperties, "/properties", Get, Options);

    METHOD_LIST_END
};