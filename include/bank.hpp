#pragma once
#include <fstream>
#include <shared_mutex>
#include "xxhash.h"
#include "parallel-hashmap/parallel_hashmap/phmap.h"
#include "user.hpp"
#include "log.hpp"

class
{
private:
    phmap::parallel_flat_hash_map<
        std::string, User,
        phmap::priv::hash_default_hash<std::string>,
        phmap::priv::hash_default_eq<std::string>,
        phmap::priv::Allocator<phmap::priv::Pair<const std::string, User>>,
        4UL,
        std::mutex>
        users;

    phmap::parallel_flat_hash_map<
        std::string, Log,
        phmap::priv::hash_default_hash<std::string>,
        phmap::priv::hash_default_eq<std::string>,
        phmap::priv::Allocator<phmap::priv::Pair<const std::string, Log>>,
        4UL,
        std::mutex>
        logs;
    /**
     * @brief size_l should be grabbed if the operation MODIFIES the size (shared), this is so that when save claims unique
     * 
     */
    std::shared_mutex size_l;

    /**
     * @brief send_funds_l should be grabbed if balances are being MODIFIED (shared) or if an operation needs to READ without the intermediary states that sendfunds has (unique)
     * 
     */
    std::shared_mutex send_funds_l;

public:
    std::string admin_pass;

    bool AddUser(const std::string &name, std::string &&init_pass)
    {
        std::shared_lock<std::shared_mutex> lock{size_l};
        return users.try_emplace_l(
            name, [](User &) {}, std::move(init_pass));
    }
    bool AdminAddUser(const std::string &attempt, std::string &&name, uint_fast32_t init_bal, std::string &&init_pass)
    {
        bool state = (admin_pass == attempt);
        if (state)
        {
            std::shared_lock<std::shared_mutex> lock{size_l};
            state = users.try_emplace_l(
                name, [](User &) {}, init_bal, std::move(init_pass));
        }
        return state;
    }

    bool DelUser(const std::string &name, const std::string &attempt)
    {
        bool state;
        {
            std::shared_lock<std::shared_mutex> lock{size_l};
            state = users.erase_if(name, [&attempt](User &u) { return (XXH3_64bits(attempt.data(), attempt.size()) == u.password); });
        }
        if (state)
        {
            logs.erase(name);
        }
        return state;
    }
    bool AdminDelUser(const std::string &name, const std::string &attempt)
    {
        bool state;
        {
            std::shared_lock<std::shared_mutex> lock{size_l};
            state = users.erase_if(name, [this, &attempt](const User &) { return (admin_pass == attempt); });
        }
        if (state)
        {
            logs.erase(name);
        }
        return state;
    }

    bool SendFunds(const std::string &a_name, const std::string &b_name, uint_fast32_t amount, const std::string &attempt)
    {

        //cant send money to self, from self or amount is 0
        if (a_name == b_name || !amount)
        {
            return false;
        }

        //if A exists, A can afford it, and A's password matches
        bool state = false;
        {
            std::shared_lock<std::shared_mutex> lock{send_funds_l}; //because SendFunds requires 3 locking operations
            users.modify_if(a_name, [&state, amount, &attempt](User &a) {
                if (state = (a.balance >= amount) && (a.password == XXH3_64bits(attempt.data(), attempt.size())))
                {
                    a.balance -= amount;
                }
            });

            if (state)
            {
                //if B doesnt exist
                if (!users.modify_if(b_name, [amount](User &b) {
                        b.balance += amount;
                    }))
                {
                    //attempt to refund if A exist
                    users.modify_if(a_name, [amount](User &a) {
                        a.balance += amount;
                    });
                    return false; //because had to refund transaction
                }
            }
        }

        if (state)
        {
            //if user lacks a log, one is created, this is to reduce usage
            Transaction temp(a_name, b_name, amount);
            Transaction temp2(a_name, b_name, amount);

            if (logs.try_emplace_l(a_name, [&temp](Log &l) { l.AddTrans(std::move(temp)); }))
            {
                logs.modify_if(a_name, [&temp](Log &l) {
                    l.AddTrans(std::move(temp));
                });
            }
            if (logs.try_emplace_l(b_name, [&temp2](Log &l) { l.AddTrans(std::move(temp2)); }))
            {
                logs.modify_if(b_name, [&temp2](Log &l) {
                    l.AddTrans(std::move(temp2));
                });
            }
        }

        return state;
    }
    bool Contains(const std::string &name) const
    {
        return users.contains(name);
    }
    bool SetBal(const std::string &name, const std::string &attempt, uint32_t amount)
    {
        bool state = (admin_pass == attempt);
        if (state)
        {
            users.modify_if(name, [amount](User &u) {
                u.balance = amount;
            });
        }
        return state;
    }
    bool AdminVerifyPass(const std::string &attempt)
    {
        return admin_pass == attempt;
    }

    int_fast64_t GetBal(const std::string &name) const
    {
        int_fast64_t res = -1;
        users.if_contains(name, [&res](const User &u) {
            res = u.balance;
        });
        return res;
    }
    int_fast8_t VerifyPassword(const std::string &name, const std::string &attempt) const
    {
        int_fast8_t res = -1;
        users.if_contains(name, [&res, &attempt](const User &u) {
            res = u.password == XXH3_64bits(attempt.data(), attempt.size());
        });
        return res;
    }
    int_fast8_t ChangePassword(const std::string &name, const std::string &attempt, std::string &&new_pass)
    {
        int_fast8_t res = -1;
        users.modify_if(name, [&res, &attempt, &new_pass](User &u) {
            res = (u.password == XXH3_64bits(attempt.data(), attempt.size()));
            if (res)
            {
                u.password = XXH3_64bits(new_pass.data(), new_pass.size());
            }
        });
        return res;
    }

    Json::Value GetLogs(const std::string &name, const std::string &attempt)
    {
        bool state = false;
        users.if_contains(name, [&state, &attempt](const User &u) {
            state = XXH3_64bits(attempt.data(), attempt.size()) == u.password;
        });

        if (!state)
        {
            return 0;
        }

        Json::Value res;
        if (!(logs.if_contains(name, [&res](const Log &l) {
                for (uint32_t i = l.data.size() - 1; i > 0; --i)
                {
                    if (!l.data[i].amount)
                    {
                        return;
                    }
                    res[99 - i]["to"] = l.data[i].to;
                    res[99 - i]["from"] = l.data[i].from;
                    res[99 - i]["amount"] = l.data[i].amount;
                }
            })))
        {
            return -1 * users.contains(name);
        }
        return res;
    }

    void
    Save()
    {
        Json::StreamWriterBuilder builder;
        const std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());

        std::ofstream user_save("users.json");
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

        writer->write(temp, &user_save);
        user_save.close();
    }

    //NOT THREAD SAFE, BY NO MEANS SHOULD THIS BE CALLED WHILE RECEIEVING REQUESTS
    void Load()
    {
        Json::CharReaderBuilder builder;

        Json::Value temp;
        std::ifstream user_save("users.json");
        builder["collectComments"] = true;
        JSONCPP_STRING errs;
        if (!parseFromStream(builder, user_save, &temp, &errs))
        {
            user_save.close();
        }
        else
        {
            user_save.close();
            for (const auto &u : temp.getMemberNames())
            {
                users.try_emplace(u, temp[u]["balance"].asUInt(), std::move(temp[u]["password"].asUInt64()));
            }
        }
    }
} bank;
