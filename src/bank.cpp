#include "bank.h"

int_fast8_t Bank::AddUser(const std::string &name, const std::string &init_pass)
{
    if (name.size() > max_name_size)
    {
        return ErrorResponse::NameTooLong;
    }
    {
        std::shared_lock<std::shared_mutex> lock{size_l};
        if (users.try_emplace_l(
                name, [](User &) {}, init_pass))
        {
            return true;
        }
        else
        {
            return ErrorResponse::UserAlreadyExists;
        }
    }
}
int_fast8_t Bank::AdminAddUser(const std::string &attempt, std::string &&name, uint32_t init_bal, std::string &&init_pass)
{
    if (name.size() > max_name_size)
    {
        return ErrorResponse::NameTooLong;
    }
    if (admin_pass != attempt)
    {
        return ErrorResponse::WrongPassword;
    }
    {
        std::shared_lock<std::shared_mutex> lock{size_l};
        if (users.try_emplace_l(
                name, [](User &) {}, init_bal, std::move(init_pass)))
        {
            return true;
        }
        else
        {
            return ErrorResponse::UserAlreadyExists;
        }
    }
}
int_fast8_t Bank::DelUser(const std::string &name, const std::string &attempt)
{
    std::shared_lock<std::shared_mutex> lock{size_l};
    bool state = false;
    if (!users.erase_if(name, [&state, &attempt](User &u) { return state = (XXH3_64bits(attempt.data(), attempt.size()) == u.password); }))
    {
        return ErrorResponse::UserNotFound;
    }
    else
    {
        if (state)
        {
            return true;
        }
        else
        {
            return ErrorResponse::WrongPassword;
        }
    }
}
int_fast8_t Bank::AdminDelUser(const std::string &name, const std::string &attempt)
{
    std::shared_lock<std::shared_mutex> lock{size_l};
    bool state = false;
    if (!users.erase_if(name, [this, &state, &attempt](const User &) { return state = (admin_pass == attempt); }))
    {
        return ErrorResponse::UserNotFound;
    }
    else
    {
        if (state)
        {
            return true;
        }
        else
        {
            return ErrorResponse::WrongPassword;
        }
    }
}

int_fast8_t Bank::SendFunds(const std::string &a_name, const std::string &b_name, uint32_t amount, const std::string &attempt)
{
    //cant send money to self, from self or amount is 0
    if (a_name == b_name || !amount)
    {
        return ErrorResponse::InvalidRequest;
    }
    //as first modify_if checks a_name and grabs unique lock
    if (!Contains(b_name))
    {
        return ErrorResponse::UserNotFound;
    }

    int_fast8_t state = false;
    {
        std::shared_lock<std::shared_mutex> lock{send_funds_l}; //because SendFunds requires 3 locking operations
        if (users.modify_if(a_name, [&state, amount, &attempt](User &a) {
                //if A exists, A can afford it, and A's password matches
                if (a.balance < amount)
                {
                    state = ErrorResponse::InsufficientFunds;
                }
                else
                {
                    if (a.password != XXH3_64bits(attempt.data(), attempt.size()))
                    {
                        state = ErrorResponse::WrongPassword;
                    }
                    else
                    {
                        a.balance -= amount;
                        state = true;
                    }
                }
            }))
        {
            if (state)
            {
                //if B does exist
                if (users.modify_if(b_name, [amount](User &b) {
                        b.balance += amount;
                    }))
                {
                    if constexpr (max_log_size > 0)
                    {
                        Transaction temp(a_name, b_name, amount);
                        Transaction temp2 = temp;
                        users.modify_if(a_name, [&temp](User &a) {
                            a.log.AddTrans(std::move(temp));
                        });
                        users.modify_if(b_name, [&temp2](User &b) {
                            b.log.AddTrans(std::move(temp2));
                        });
                    }
                    return true;
                }
                else
                {
                    //attempt to refund if A exist
                    users.modify_if(a_name, [amount](User &a) {
                        a.balance += amount;
                    });
                    return ErrorResponse::UserNotFound; //because had to refund transaction
                }
            }
            else
            {
                return state;
            }
        }
        else
        {
            return ErrorResponse::UserNotFound;
        }
    }
}

int_fast8_t Bank::Contains(const std::string &name) const
{
    if (users.contains(name))
    {
        return true;
    }
    else
    {
        return ErrorResponse::UserNotFound;
    }
}
int_fast8_t Bank::AdminVerifyPass(const std::string &attempt)
{
    if (admin_pass == attempt)
    {
        return true;
    }
    else
    {
        return ErrorResponse::WrongPassword;
    }
}

int_fast8_t Bank::SetBal(const std::string &name, const std::string &attempt, uint32_t amount)
{
    if (admin_pass != attempt)
    {
        return ErrorResponse::WrongPassword;
    }
    if (users.modify_if(name, [amount](User &u) {
            u.balance = amount;
        }))
    {
        return true;
    }
    else
    {
        return ErrorResponse::UserNotFound;
    }
}
int_fast64_t Bank::GetBal(const std::string &name) const
{
    int_fast64_t res = ErrorResponse::UserNotFound;
    users.if_contains(name, [&res](const User &u) {
        res = u.balance;
    });
    return res;
}

int_fast8_t Bank::VerifyPassword(const std::string &name, const std::string &attempt) const
{
    int_fast8_t res = ErrorResponse::UserNotFound;
    users.if_contains(name, [&res, &attempt](const User &u) {
        if (u.password == XXH3_64bits(attempt.data(), attempt.size()))
        {
            res = true;
        }
        else
        {
            res = ErrorResponse::WrongPassword;
        }
    });
    return res;
}
int_fast8_t Bank::ChangePassword(const std::string &name, const std::string &attempt, std::string &&new_pass)
{
    int_fast8_t res = ErrorResponse::UserNotFound;
    users.modify_if(name, [&res, &attempt, &new_pass](User &u) {
        if (u.password != XXH3_64bits(attempt.data(), attempt.size()))
        {
            res = ErrorResponse::WrongPassword;
        }
        else
        {
            u.password = XXH3_64bits(new_pass.data(), new_pass.size());
        }
    });
    return res;
}

Json::Value Bank::GetLogs(const std::string &name, const std::string &attempt)
{
    Json::Value res;
    if (!users.if_contains(name, [&res, &attempt](const User &u) {
            if (u.password != XXH3_64bits(attempt.data(), attempt.size()))
            {
                res = ErrorResponse::WrongPassword;
            }
            else
            {
                Json::Value temp;
                for (uint32_t i = u.log.data.size(); i > 0; --i)
                {
                    temp[i - 1]["to"] = u.log.data[u.log.data.size() - i].to;
                    temp[i - 1]["from"] = u.log.data[u.log.data.size() - i].from;
                    temp[i - 1]["amount"] = (Json::UInt)u.log.data[u.log.data.size() - i].amount;
                    temp[i - 1]["time"] = (Json::UInt64)u.log.data[u.log.data.size() - i].time;
                }
                res = std::move(temp);
            }
        }))
    {
        return ErrorResponse::UserNotFound;
    }
    return res;
}

void Bank::Save()
{
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
        std::ofstream user_save("../users.json");
        Json::StreamWriterBuilder builder;
        const std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
        writer->write(temp, &user_save);
        user_save.close();
    }
}

//NOT THREAD SAFE, BY NO MEANS SHOULD THIS BE CALLED WHILE RECEIEVING REQUESTS
void Bank::Load()
{
    Json::CharReaderBuilder builder;

    Json::Value temp;
    std::ifstream user_save("../users.json");
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
