#include "bank_f.h"

#define JSON(V) callback(HttpResponse::newHttpJsonResponse(JsonReturn(V)));
#define INLINE __attribute__((always_inline)) inline
#define GEN_BODY                                \
    const auto temp_req = req->getJsonObject(); \
    const auto body = temp_req ? *temp_req : Json::Value();
#define PASS_HEADER req->getHeader("Password")

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

BankF::BankF(Bank *b) : bank(*b) {}

void BankF::Help(req_args) const
{
    auto resp = HttpResponse::newHttpResponse();
    resp->setBody("  <h1 class=\"code-line\" data-line-start=0 data-line-end=1 ><a id=\"Error_Responses_0\"></a>Error Responses</h1> <table class=\"table table-striped table-bordered\"> <thead> <tr> <th>#</th> <th>meaning</th> </tr> </thead> <tbody> <tr> <td>-1</td> <td>UserNotFound</td> </tr> <tr> <td>-2</td> <td>WrongPassword</td> </tr> <tr> <td>-3</td> <td>InvalidRequest</td> </tr> <tr> <td>-4</td> <td>WrongAdminPassword</td> </tr> <tr> <td>-5</td> <td>NameTooLong</td> </tr> <tr> <td>-6</td> <td>UserAlreadyExists</td> </tr> <tr> <td>-7</td> <td>InsufficientFunds</td> </tr> </tbody> </table> <h1 class=\"code-line\" data-line-start=12 data-line-end=13 ><a id=\"Things_of_Note_12\"></a>Things of Note</h1> <ul> <li class=\"has-line-data\" data-line-start=\"13\" data-line-end=\"14\">all endpoints respond with <strong>JSON</strong> file type</li> <li class=\"has-line-data\" data-line-start=\"14\" data-line-end=\"16\">&quot;<strong>A</strong>&quot; denotes requiring Authentication in the form of a header titled &quot;<strong>Password</strong>&quot;</li> </ul> <h1 class=\"code-line\" data-line-start=16 data-line-end=17 ><a id=\"Usage_16\"></a>Usage</h1> <table class=\"table table-striped table-bordered\"> <thead> <tr> <th style=\"text-align:center\">Name</th> <th style=\"text-align:left\">Path</th> <th style=\"text-align:center\">Method</th> <th style=\"text-align:center\">A</th> <th>Description</th> </tr> </thead> <tbody> <tr> <td style=\"text-align:center\">GetBal</td> <td style=\"text-align:left\">/{name}/bal</td> <td style=\"text-align:center\">GET</td> <td style=\"text-align:center\">false</td> <td>returns the balance of a given user <code>{name}</code></td> </tr> <tr> <td style=\"text-align:center\">GetLog</td> <td style=\"text-align:left\">/{name}/log</td> <td style=\"text-align:center\">GET</td> <td style=\"text-align:center\">true</td> <td>returns a list of last <code>n</code> number of transactions (a configurable amount) of a given user <code>{name}</code></td> </tr> <tr> <td style=\"text-align:center\">SendFunds</td> <td style=\"text-align:left\">/{name}/send/{to}/amount={amount}</td> <td style=\"text-align:center\">POST</td> <td style=\"text-align:center\">true</td> <td>sends <code>{amount}</code> from user <code>{name}</code> to user <code>{to}</code></td> </tr> <tr> <td style=\"text-align:center\">VerifyPassword</td> <td style=\"text-align:left\">/{name}/pass/verify</td> <td style=\"text-align:center\">GET</td> <td style=\"text-align:center\">true</td> <td>returns <code>1</code> if the supplied user <code>{name}</code>'s password matches the password supplied in the header</td> </tr> </tbody> </table> <h1 class=\"code-line\" data-line-start=24 data-line-end=25 ><a id=\"Meta_Usage_24\"></a>Meta Usage</h1> <table class=\"table table-striped table-bordered\"> <thead> <tr> <th style=\"text-align:center\">Name</th> <th style=\"text-align:left\">Path</th> <th style=\"text-align:center\">Method</th> <th style=\"text-align:center\">A</th> <th>Description</th> </tr> </thead> <tbody> <tr> <td style=\"text-align:center\">ChangePassword</td> <td style=\"text-align:left\">/{name}/pass/change</td> <td style=\"text-align:center\">PATCH</td> <td style=\"text-align:center\">true</td> <td>if the password supplied in the header matches the user <code>{name}</code>'s password, the user’s password is changed to the one given in the body</td> </tr> <tr> <td style=\"text-align:center\">SetBal</td> <td style=\"text-align:left\">/admin/{name}/bal/amount={amount}</td> <td style=\"text-align:center\">PATCH</td> <td style=\"text-align:center\">true</td> <td>sets the balance of a give user <code>{name}</code> if the supplied password matches the admin password</td> </tr> </tbody> </table> <h1 class=\"code-line\" data-line-start=30 data-line-end=31 ><a id=\"System_Usage_30\"></a>System Usage</h1> <table class=\"table table-striped table-bordered\"> <thead> <tr> <th style=\"text-align:center\">Name</th> <th style=\"text-align:left\">Path</th> <th style=\"text-align:center\">Method</th> <th style=\"text-align:center\">A</th> <th>Description</th> </tr> </thead> <tbody> <tr> <td style=\"text-align:center\">Help</td> <td style=\"text-align:left\">/help</td> <td style=\"text-align:center\">GET</td> <td style=\"text-align:center\">false</td> <td>the page you’re looking at right now!</td> </tr> <tr> <td style=\"text-align:center\">Close</td> <td style=\"text-align:left\">/admin/close</td> <td style=\"text-align:center\">POST</td> <td style=\"text-align:center\">true</td> <td>saves and then closes the program if the supplied password matches the admin password</td> </tr> <tr> <td style=\"text-align:center\">Contains</td> <td style=\"text-align:left\">/contains/{name}</td> <td style=\"text-align:center\">GET</td> <td style=\"text-align:center\">false</td> <td>returns <code>true</code> or <code>false</code> depending on if the supplied user <code>{name}</code> exists</td> </tr> <tr> <td style=\"text-align:center\">AdminVerifyPass</td> <td style=\"text-align:left\">/admin/verify</td> <td style=\"text-align:center\">GET</td> <td style=\"text-align:center\">true</td> <td>returns <code>1</code> if the password supplied in the header matches the admin password</td> </tr> </tbody> </table> <h1 class=\"code-line\" data-line-start=38 data-line-end=39 ><a id=\"User_Management_38\"></a>User Management</h1> <table class=\"table table-striped table-bordered\"> <thead> <tr> <th style=\"text-align:center\">Name</th> <th style=\"text-align:left\">Path</th> <th style=\"text-align:center\">Method</th> <th style=\"text-align:center\">A</th> <th>Description</th> </tr> </thead> <tbody> <tr> <td style=\"text-align:center\">AddUser</td> <td style=\"text-align:left\">/user/{name}</td> <td style=\"text-align:center\">POST</td> <td style=\"text-align:center\">true</td> <td>registers a user with the name <code>{name}</code>, balance of 0 and a password of the password supplied in the header</td> </tr> <tr> <td style=\"text-align:center\">AdminAddUser</td> <td style=\"text-align:left\">/admin/user/{name}?init_bal={init_bal}</td> <td style=\"text-align:center\">POST</td> <td style=\"text-align:center\">true</td> <td>if the password supplied in the header matches the admin password, then it registers a user with the name <code>{name}</code>, balance of <code>init_bal</code> and a password supplied by the body of the request</td> </tr> <tr> <td style=\"text-align:center\">DelUser</td> <td style=\"text-align:left\">/user/{name}</td> <td style=\"text-align:center\">DELETE</td> <td style=\"text-align:center\">true</td> <td>if the password supplied in the header matches the user <code>{name}</code>'s password, then the user is deleted</td> </tr> <tr> <td style=\"text-align:center\">AdminDelUser</td> <td style=\"text-align:left\">/admin/user/{name}</td> <td style=\"text-align:center\">DELETE</td> <td style=\"text-align:center\">true</td> <td>if the password supplied in the header matches the admin password, then the user is deleted</td> </tr> </tbody> </table> ");
    resp->setExpiredTime(0);
    callback(resp);
}
void BankF::Close(req_args) const
{
    bool res;
    if (PASS_HEADER == bank.admin_pass)
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
void BankF::Ping(req_args) const
{
    auto resp = HttpResponse::newHttpResponse();
    resp->setBody("pong");
    resp->setExpiredTime(0);
    callback(resp);
}
void BankF::AddUser(req_args, const std::string &name) const
{
    JSON(bank.AddUser(std::move(name), PASS_HEADER));
}
void BankF::AdminAddUser(req_args, std::string &&name, uint32_t init_bal) const
{
    JSON(bank.AdminAddUser(PASS_HEADER, std::move(name), init_bal, std::string(req->getBody())));
}
void BankF::DelUser(req_args, const std::string &name) const
{
    JSON(bank.DelUser(name, PASS_HEADER));
}
void BankF::AdminDelUser(req_args, const std::string &name) const
{
    JSON(bank.AdminDelUser(name, PASS_HEADER));
}
void BankF::SendFunds(req_args, const std::string name, const std::string to, uint32_t amount) const
{
    JSON(bank.SendFunds(name, to, amount, PASS_HEADER));
}
void BankF::ChangePassword(req_args, const std::string &name) const
{
    JSON(bank.ChangePassword(name, PASS_HEADER, std::string(req->getBody())));
}
void BankF::Contains(req_args, const std::string &name) const
{
    JSON(bank.Contains(name));
}
void BankF::GetBal(req_args, const std::string &name) const
{
    JSON(bank.GetBal(name));
}
void BankF::VerifyPassword(req_args, const std::string &name) const
{
    JSON(bank.VerifyPassword(name, PASS_HEADER));
}
void BankF::SetBal(req_args, const std::string &name, uint32_t amount) const
{
    JSON(bank.SetBal(name, PASS_HEADER, amount));
}
void BankF::AdminVerifyPass(req_args)
{
    JSON(bank.AdminVerifyPass(PASS_HEADER));
}
void BankF::GetLog(req_args, const std::string &name)
{
    if constexpr (max_log_size)
    {
        JSON(bank.GetLogs(name, PASS_HEADER));
    }
    else
    {
        auto resp = HttpResponse::newHttpJsonResponse(JsonReturn("Logs are Disabled"));
        resp->setExpiredTime(0); //cached forever
        callback(resp);
    }
}