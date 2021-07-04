#include "bank.h"

__attribute__((always_inline)) inline bool ValidUsrname(const std::string &name) noexcept
{
    if (name.size() < min_name_size || name.size() > max_name_size)
    {
        return false;
    }
    for (const char &c : name)
    {
        if (!(std::isalpha(c) || std::isdigit(c) || c == '_'))
        {
            return false;
        }
    }
    return true;
}

using namespace drogon;

#if CONSERVATIVE_DISK_SAVE
bool Bank::GetChangeState() const noexcept
{
#if MULTI_THREADED
    return save_flag.GetChangeState();
#else
    return save_flag;
#endif
}
#endif

BankResponse
Bank::GetBal(const std::string &name) const noexcept
{
    uint64_t res = 0;
    users.if_contains(name, [&res](const User &u) { res = u.balance + 1; });
    return res ? BankResponse(k200OK, res - 1) : BankResponse(k404NotFound, "User not found");
}
BankResponse Bank::GetLogs(const std::string &name) noexcept
{
    BankResponse res;
#if MAX_LOG_SIZE > 0
    if (!users.modify_if(name, [&res](User &u) { res = {k200OK, u.log.GetLog()}; }))
    {
        return BankResponse(k404NotFound, "User not found");
    }
#endif
    return res;
}
BankResponse Bank::SendFunds(const std::string &a_name, const std::string &b_name, uint32_t amount) noexcept
{
    //cant send money to self, from self or amount is 0
    if (a_name == b_name)
    {
        return {k400BadRequest, "Sender and Reciever names cannot match"};
    }

    //cant send 0
    if (!amount)
    {
        return {k400BadRequest, "Amount being sent cannot be 0"};
    }

    //as first modify_if checks a_name and grabs unique lock
    if (!Contains(b_name))
    {
        return {k404NotFound, "Reciever does not exist"};
    }

    BankResponse state;
    std::shared_lock<std::shared_mutex> lock{save_lock}; //about 10% of this function's cost
#if MAX_LOG_SIZE > 0
    Transaction temp(a_name, b_name, amount);
    if (!users.modify_if(a_name, [&temp, &state, amount](User &a) {
#else
    if (!users.modify_if(a_name, [&state, amount](User &a) {
#endif
            //if A can afford it
            if (a.balance < amount)
            {
                state = {k400BadRequest, "Sender has insufficient funds"};
            }
            else
            {
                a.balance -= amount;
#if MAX_LOG_SIZE > 0
                a.log.AddTrans(Transaction(temp)); //about 40% of this function's cost
#endif
                state = {k200OK, "Transfer successful!"};
            }
        }))
    {
        return {k404NotFound, "Sender does not exist"};
    }
    if (state.first == k200OK)
    {
#if MAX_LOG_SIZE > 0
        users.modify_if(b_name, [&temp, amount](User &b) {
            b.balance += amount;
            b.log.AddTrans(std::move(temp));
        }); //about 40% of this function's cost
#else
        users.modify_if(b_name, [amount](User &b) { b.balance += amount; });
#endif

#if CONSERVATIVE_DISK_SAVE
#if MULTI_THREADED
        save_flag.SetChangesOn(); //about 5% of this function's cost
#else
        save_flag = true;
#endif
#endif
    }
    return state;
}
bool Bank::VerifyPassword(std::string_view name, std::string_view attempt) const noexcept
{
    bool res = false;
    users.if_contains(name.data(), [&res, &attempt](const User &u) { res = (u.password == xxHashStringGen{}(attempt)); });
    return res;
}

void Bank::ChangePassword(const std::string &name, std::string &&new_pass) noexcept
{
    users.modify_if(name, [&new_pass](User &u) { u.password = xxHashStringGen{}(new_pass); });
#if CONSERVATIVE_DISK_SAVE
#if MULTI_THREADED
    save_flag.SetChangesOn();
#else
    save_flag = true;
#endif
#endif
}
BankResponse Bank::SetBal(const std::string &name, uint32_t amount) noexcept
{
    if (users.modify_if(name, [amount](User &u) { u.balance = amount; }))
    {
#if CONSERVATIVE_DISK_SAVE
#if MULTI_THREADED
        save_flag.SetChangesOn();
#else
        save_flag = true;
#endif
#endif
        return {k200OK, "Balance set!"};
    }
    else
    {
        return {k404NotFound, "User not found"};
    }
}
BankResponse Bank::AddBal(const std::string &name, uint32_t amount) noexcept
{
    if (users.modify_if(name, [amount](User &u) { u.balance += amount; }))
    {
#if CONSERVATIVE_DISK_SAVE
#if MULTI_THREADED
        save_flag.SetChangesOn();
#else
        save_flag = true;
#endif
#endif
        return {k200OK, "Balance set!"};
    }
    else
    {
        return {k404NotFound, "User not found"};
    }
}
bool Bank::Contains(const std::string &name) const noexcept
{
    return users.contains(name);
}
bool Bank::AdminVerifyAccount(std::string_view name) noexcept
{
    return (name == admin_account);
}

BankResponse Bank::AddUser(std::string &&name, uint32_t init_bal, std::string &&init_pass) noexcept
{
    if (!ValidUsrname(name))
    {
        return {k400BadRequest, "Invalid Name, breaks size and/or character restrictions"};
    }
    std::shared_lock<std::shared_mutex> lock{save_lock};
    if (users.try_emplace_l(
            std::move(name), [](User &) {}, init_bal, std::move(init_pass)))
    {
#if CONSERVATIVE_DISK_SAVE
#if MULTI_THREADED
        save_flag.SetChangesOn();
#else
        save_flag = true;
#endif
#endif
        return {k200OK, "User added!"};
    }
    else
    {
        return {k409Conflict, "User already exists"};
    }
}
BankResponse Bank::DelUser(const std::string &name) noexcept
{
    std::shared_lock<std::shared_mutex> lock{save_lock};
#if RETURN_ON_DEL
    uint32_t bal;
    if (users.if_contains(name, [this, &bal](const User &u) {
            bal = u.balance;
        }))
    {
        users.modify_if(return_account, [ this, bal ](User & u))
        {
            u.balance += bal;
        }
    }
#endif
    if (users.erase(name))
    {
#if CONSERVATIVE_DISK_SAVE
#if MULTI_THREADED
        save_flag.SetChangesOn();
#else
        save_flag = true;
#endif
#endif
        return BankResponse(k200OK, "User deleted!");
    }
    else
    {
        return BankResponse(k404NotFound, "User not found");
    }
}
void Bank::Save()
{
#if CONSERVATIVE_DISK_SAVE
    if (GetChangeState())
    {
#endif
        Json::Value temp;

        //loading info into json temp
        {
            std::unique_lock<std::shared_mutex> lock{save_lock};
            for (const auto &u : users)
            {
                //we know it contains this key but we call this func to grab mutex
                users.if_contains(u.first, [&temp, &u](const User &u_val) { temp[u.first.data()] = u_val.Serialize(); });
            }
        }
        if (temp.isNull())
        {
            throw std::invalid_argument("Saving Failed\n");
        }
        else
        {
            std::ofstream user_save(users_location);
            Json::StreamWriterBuilder builder;
            const std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
            writer->write(temp, &user_save);
            user_save.close();
        }
#if CONSERVATIVE_DISK_SAVE
#if MULTI_THREADED
        save_flag.SetChangesOff();
#else
        save_flag = true;
#endif
    }
#endif
}

//NOT THREAD SAFE, BY NO MEANS SHOULD THIS BE CALLED WHILE RECEIEVING REQUESTS
void Bank::Load()
{
    Json::CharReaderBuilder builder;

    Json::Value temp;
    std::ifstream user_save(users_location);
    builder["collectComments"] = true;
    JSONCPP_STRING errs;
    if (!parseFromStream(builder, user_save, &temp, &errs))
    {
        std::cerr << errs << '\n';
        user_save.close();
        throw std::invalid_argument("Parsing Failed\n");
    }
    else
    {
        user_save.close();
        for (const auto &u : temp.getMemberNames())
        {
            if constexpr (MAX_LOG_SIZE > 0)
            {
                users.try_emplace(u, temp[u]["balance"].asUInt(), std::move(temp[u]["password"].asUInt64()), temp[u]["log"]);
            }
            else
            {
                users.try_emplace(u, temp[u]["balance"].asUInt(), std::move(temp[u]["password"].asUInt64()));
            }
        }
    }
}
