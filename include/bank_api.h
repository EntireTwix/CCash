#pragma once
#include <drogon/HttpController.h>
#include "user_filter.h"

using namespace drogon;

#define req_args const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback

namespace v1
{
    class api : public HttpController<api, false>
    {
        Bank &bank;

    public:
        api(Bank &b);
        void GetBal(req_args, const std::string &name) const;
        void GetLog(req_args, const std::string &name);
        void SendFunds(req_args, const std::string &name) const;
        void VerifyPassword(req_args, const std::string &name) const;

        void ChangePassword(req_args, const std::string &name) const;

        void Help(req_args) const;
        void Ping(req_args) const;
        void Close(req_args) const;
        void AddUser(req_args, const std::string &name) const;
        void AdminAddUser(req_args, std::string &&name, uint32_t init_bal) const;
        void DelUser(req_args, const std::string &name) const;
        void AdminDelUser(req_args, const std::string &name) const;
        void Contains(req_args, const std::string &name) const;
        void SetBal(req_args, const std::string &name, uint32_t amount) const;
        void AdminVerifyPass(req_args);

        METHOD_LIST_BEGIN

        //Usage
        METHOD_ADD(api::GetBal, "/users/{name}/bal", Get, Options);                        //done
        METHOD_ADD(api::GetLog, "/users/{name}/log", Get, Options, "UserFilter");          //snapshot not implemented
        METHOD_ADD(api::SendFunds, "/users/{name}/transfer", Post, Options, "UserFilter"); //responses incomplete
        METHOD_ADD(api::VerifyPassword, "/users/{name}/verify_password", Get, Options);    //done

        //Meta Usage
        METHOD_ADD(api::ChangePassword, "/users/{name}/change_password", Patch, Options); //done
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
};