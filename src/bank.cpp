#include "bank.h"

using namespace drogon;

#if CONSERVATIVE_DISK_SAVE
void Bank::ChangesMade() noexcept
{
    return change_flag.store(1, std::memory_order_release);
}
void Bank::ChangesSaved() noexcept
{
    return change_flag.store(0, std::memory_order_release);
}
bool Bank::GetChangeState() noexcept
{
    return change_flag.load(std::memory_order_acquire);
}
#endif

BankResponse Bank::GetBal(const std::string &name) const noexcept
{
    int_fast64_t res = -1;
    users.if_contains(name, [&res](const User &u) {
        res = u.balance;
    });
    return res < 0 ? BankResponse(k404NotFound, "User not found") : BankResponse(k200OK, res);
}
BankResponse Bank::GetLogs(const std::string &name) noexcept
{
    BankResponse res;
    if (!users.if_contains(name, [&res](const User &u) {
            Json::Value temp;
            for (uint32_t i = u.log.data.size(); i > 0; --i)
            {
                temp[i - 1]["to"] = u.log.data[u.log.data.size() - i].to;
                temp[i - 1]["from"] = u.log.data[u.log.data.size() - i].from;
                temp[i - 1]["amount"] = (Json::UInt)u.log.data[u.log.data.size() - i].amount;
                temp[i - 1]["time"] = (Json::UInt64)u.log.data[u.log.data.size() - i].time;
            }
            res = {k200OK, std::move(temp)};
        }))
    {
        return BankResponse(k404NotFound, "User not found");
    }
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
    if constexpr (max_log_size > 0)
    {
        Transaction temp(a_name, b_name, amount);
        std::shared_lock<std::shared_mutex> lock{send_funds_l};
        if (!users.modify_if(a_name, [&temp, &state, amount](User &a) {
                //if A can afford it
                if (a.balance < amount)
                {
                    state = {k402PaymentRequired, "Sender has insufficient funds"};
                }
                else
                {
                    a.balance -= amount;
                    a.log.AddTrans(Transaction(temp));
                    state = {k200OK, "Transfer successful!"};
                }
            }))
        {
            return {k404NotFound, "Sender does not exist"};
        }
        if (state.first == k200OK)
        {
            users.modify_if(b_name, [&temp, amount](User &b) {
                b.balance += amount;
                b.log.AddTrans(std::move(temp));
            });
#if CONSERVATIVE_DISK_SAVE
            ChangesMade();
#endif
        }
        return state;
    }
    else
    {
        std::shared_lock<std::shared_mutex> lock{send_funds_l};
        users.modify_if(a_name, [this, &state, amount](User &a) {
            //if A can afford it
            if (a.balance < amount)
            {
                state = {k402PaymentRequired, "Sender has insufficient funds"};
            }
            else
            {
                a.balance -= amount;
                state = {k200OK, "Transfer successful!"};
            }
        });
        if (state.first == k200OK)
        {
            users.modify_if(b_name, [&a_name, &b_name, amount](User &b) {
                b.balance += amount;
            });
#if CONSERVATIVE_DISK_SAVE
            ChangesMade();
#endif
        }
        return state;
    }
}
bool Bank::VerifyPassword(const std::string &name, const std::string &attempt) const noexcept
{
    bool res = false;
    users.if_contains(name, [&res, &attempt](const User &u) {
        res = (u.password == XXH3_64bits(attempt.data(), attempt.size()));
    });
    return res;
}

void Bank::ChangePassword(const std::string &name, std::string &&new_pass) noexcept
{
    users.modify_if(name, [&new_pass](User &u) {
        u.password = XXH3_64bits(new_pass.data(), new_pass.size());
    });
#if CONSERVATIVE_DISK_SAVE
    ChangesMade();
#endif
}
BankResponse Bank::SetBal(const std::string &name, uint32_t amount) noexcept
{
    if (users.modify_if(name, [amount](User &u) {
            u.balance = amount;
        }))
    {
#if CONSERVATIVE_DISK_SAVE
        ChangesMade();
#endif
        return BankResponse(k200OK, "Balance set!");
    }
    else
    {
        return BankResponse(k404NotFound, "User not found");
    }
}

int_fast8_t Bank::AddUser(const std::string &name, const std::string &init_pass) noexcept
{
    if (name.size() > max_name_size)
    {
        return ErrorResponse::NameTooLong;
    }
    if (name.find(' ') != std::string::npos)
    {
        return ErrorResponse::InvalidRequest;
    }

    std::shared_lock<std::shared_mutex> lock{size_l};
    return (users.try_emplace_l(
               name, [](User &) {}, init_pass))
               ? true
               : ErrorResponse::UserAlreadyExists;
}
int_fast8_t Bank::AdminAddUser(const std::string &attempt, std::string &&name, uint32_t init_bal, std::string &&init_pass) noexcept
{
    if (name.size() > max_name_size)
    {
        return ErrorResponse::NameTooLong;
    }
    if (admin_pass != attempt)
    {
        return ErrorResponse::WrongPassword;
    }

    std::shared_lock<std::shared_mutex> lock{size_l};
    return (users.try_emplace_l(
               name, [](User &) {}, init_bal, std::move(init_pass)))
               ? true
               : ErrorResponse::UserAlreadyExists;
}
int_fast8_t Bank::DelUser(const std::string &name, const std::string &attempt) noexcept
{
    std::shared_lock<std::shared_mutex> lock{size_l};
    bool state = false;
#if RETURN_ON_DEL
    uint32_t bal;
    if (users.erase_if(name, [this, &bal, &name, &state, &attempt](User &u) {
            bal = u.balance;
#else
    if (users.erase_if(name, [this, &name, &state, &attempt](User &u) {
#endif
            return state = (XXH3_64bits(attempt.data(), attempt.size()) == u.password);
        }))
    {
        if constexpr (RETURN_ON_DEL)
        {
            if (state) //if the password matches
            {
#if RETURN_ON_DEL
                users.modify_if(return_account, [&bal](User &u) {
                    u.balance += bal;
                });
#endif
                return true;
            }
            else
            {
                return ErrorResponse::WrongPassword;
            }
        }
        else
        {
            return (state) ? true : ErrorResponse::WrongPassword;
        }
    }
    else
    {
        return ErrorResponse::UserNotFound;
    }
}
int_fast8_t Bank::AdminDelUser(const std::string &name, const std::string &attempt) noexcept
{
    std::shared_lock<std::shared_mutex> lock{size_l};
    bool state = false;
#if RETURN_ON_DEL
    uint32_t bal;
    if (users.erase_if(name, [this, &bal, &name, &state, &attempt](User &u) {
            bal = u.balance;
#else
    if (users.erase_if(name, [this, &name, &state, &attempt](User &u) {
#endif
            return state = (XXH3_64bits(attempt.data(), attempt.size()) == u.password);
        }))
    {
        if (state)
        {
#if RETURN_ON_DEL
            users.modify_if(return_account, [&bal](User &u) {
                u.balance += bal;
            });
#endif
            return true;
        }
        else
        {
            return ErrorResponse::WrongPassword;
        }
    }
    else
    {
        return ErrorResponse::UserNotFound;
    }
}

bool Bank::Contains(const std::string &name) const noexcept
{
    return users.contains(name);
}
bool Bank::AdminVerifyPass(const std::string &attempt) noexcept
{
    return (admin_pass == attempt);
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
            std::scoped_lock<std::shared_mutex, std::shared_mutex> lock{size_l, send_funds_l};
            for (const auto &u : users)
            {
                //we know it contains this key but we call this func to grab mutex
                users.if_contains(u.first, [&temp, &u](const User &u_val) {
                    temp[u.first] = u_val.Serialize();
                });
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
        ChangesSaved();
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
            if constexpr (max_log_size > 0)
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
