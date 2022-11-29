#pragma once
#include <drogon/HttpController.h>
#include "str_intrusion.h"
#include "json_filter.h"
#include "user_filter.h"

using namespace drogon;

#define req_args const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback

class api : public HttpController<api>
{
public:
#if API_VERSION >= 1
    static void GetBal(req_args, const std::string &name);
    static void GetLogs(req_args);
    static void SendFunds(req_args);
    static void VerifyPassword(req_args);

    static void ChangePassword(req_args);
    static void AdminChangePassword(req_args);
    static void SetBal(req_args);
    static void ImpactBal(req_args);

    static void Help(req_args);
    static void Close(req_args);
    static void Contains(req_args, const std::string &name);
    static void AdminVerifyAccount(req_args);
    static void PruneUsers(req_args);
    static void ApiProperties(req_args);

    static void AddUser(req_args);
    static void AdminAddUser(req_args);
    static void DelSelf(req_args);
    static void AdminDelUser(req_args);
#endif

    METHOD_LIST_BEGIN

    //Usage
    METHOD_ADD(api::GetBal, "/v1/user/balance?name={name}", Get, Options, "JsonFilter<false>");
#if MAX_LOG_SIZE > 0
    METHOD_ADD(api::GetLogs, "/v1/user/log", Get, Options, "JsonFilter<false>", "UserFilter<true, false>");
#else
    METHOD_ADD(api::GetLogs, "/v1/user/log", Get, Options, "JsonFilter<false>");
#endif
    METHOD_ADD(api::SendFunds, "/v1/user/transfer", Post, Options, "JsonFilter<true>", "UserFilter<true, false>"); //expects ["name"](string) and ["amount"](uint32)
    METHOD_ADD(api::VerifyPassword, "/v1/user/verify_password", Post, Options, "UserFilter<false, false>", "JsonFilter<false>");

    //Meta Usage
    METHOD_ADD(api::ChangePassword, "/v1/user/change_password", Patch, Options, "JsonFilter<true>", "UserFilter<true, false>");            //expects ["pass"](string)
    METHOD_ADD(api::AdminChangePassword, "/v1/admin/user/change_password", Patch, Options, "JsonFilter<true>", "UserFilter<false, true>"); //expects ["name"](string) and ["pass"](string)
    METHOD_ADD(api::SetBal, "/v1/admin/set_balance", Patch, Options, "JsonFilter<true>", "UserFilter<false, true>");                       //expects ["name"](string) and ["amount"](uint32)
    METHOD_ADD(api::ImpactBal, "/v1/admin/impact_balance", Post, Options, "JsonFilter<true>", "UserFilter<false, true>");                  //expects ["name"](string) and ["amount"](uint32)

    //System Usage
    METHOD_ADD(api::Help, "/v1/help", Get, Options);
    METHOD_ADD(api::Close, "/v1/admin/shutdown", Post, Options, "UserFilter<false, true>", "JsonFilter<false>");
    METHOD_ADD(api::Contains, "/v1/user/exists?name={name}", Get, Options, "JsonFilter<false>");
    METHOD_ADD(api::AdminVerifyAccount, "/v1/admin/verify_account", Post, Options, "UserFilter<false, true>", "JsonFilter<false>");
    METHOD_ADD(api::PruneUsers, "/v1/admin/prune_users", Post, "UserFilter<false, true>", "JsonFilter<true>"); //expects ["time"](int64) and ["amount"](uint32)
    METHOD_ADD(api::ApiProperties, "/properties", Get, Options);

    //User Managment
    METHOD_ADD(api::AddUser, "/v1/user/register", Post, Options);                                                           //expects ["name"](string) ["pass"](string)
    METHOD_ADD(api::AdminAddUser, "/v1/admin/user/register", Post, Options, "JsonFilter<true>", "UserFilter<false, true>"); //expects ["name"](string) ["amount"](uint32) ["pass"](string)
    METHOD_ADD(api::DelSelf, "/v1/user/delete", Delete, Options, "UserFilter<true, false>", "JsonFilter<false>");
    METHOD_ADD(api::AdminDelUser, "/v1/admin/user/delete", Delete, Options, "JsonFilter<true>", "UserFilter<false, true>"); //expects ["name"](string)

    METHOD_LIST_END
};