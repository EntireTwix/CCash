#pragma once
#include <fstream>
#include <shared_mutex>
#include <vector>
#include "parallel-hashmap/parallel_hashmap/phmap.h"
#include "user.hpp"

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

    /**
     * @brief size_lock should be grabbed if the operation MODIFIES the size (unique) or READS from the users where size changes could distort (shared)
     * 
     */
    std::shared_mutex size_lock;

    /**
     * @brief send_funds_l should be grabbed if balances are being MODIFIED (shared) or if an operation needs to READ without the intermediary states that sendfunds has (unique)
     * 
     */
    std::shared_mutex send_funds_l;

public:
    std::string admin_pass;

    bool AddUser(const std::string &name, std::string &&init_pass)
    {
        std::unique_lock<std::shared_mutex> lock{size_lock};
        return users.try_emplace_l(
            name, [](User &) {}, std::move(init_pass));
    }
    bool AdminAddUser(const std::string &attempt, std::string &&name, uint_fast32_t init_bal, std::string &&init_pass)
    {
        bool state = (admin_pass == attempt);
        if (state)
        {
            std::unique_lock<std::shared_mutex> lock{size_lock};
            state = users.try_emplace_l(
                name, [](User &) {}, init_bal, std::move(init_pass));
        }
        return state;
    }

    bool DelUser(const std::string &name, const std::string &attempt)
    {
        std::unique_lock<std::shared_mutex> lock{size_lock};
        return users.erase_if(name, [&attempt](const User &u) { return (std::hash<std::string>{}(attempt) == u.password); });
    }
    bool AdminDelUser(const std::string &name, const std::string &attempt)
    {
        std::unique_lock<std::shared_mutex> lock{size_lock};
        return users.erase_if(name, [this, &attempt](const User &) { return (admin_pass == attempt); });
    }

    bool SendFunds(const std::string &a_name, const std::string &b_name, uint_fast32_t amount, const std::string &attempt)
    {

        //cant send money to self, from self
        if (a_name == b_name)
        {
            return false;
        }

        //if A exists, A can afford it, and A's password matches
        bool state = false;
        std::shared_lock<std::shared_mutex> lock{send_funds_l}; //because SendFunds requires 3 locking operations
        users.modify_if(a_name, [&state, amount, &attempt](User &a) {
            if (state = (a.balance >= amount) && (a.password == std::hash<std::string>{}(attempt)), state)
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
                state = false; //because had to refund transaction
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
            res = u.password == std::hash<std::string>{}(attempt);
        });
        return res;
    }
    int_fast8_t ChangePassword(const std::string &name, const std::string &attempt, std::string &&new_pass)
    {
        int_fast8_t res = -1;
        users.modify_if(name, [&res, &attempt, &new_pass](User &u) {
            res = (u.password == std::hash<std::string>{}(attempt));
            if (res)
            {
                u.password = std::hash<std::string>{}(new_pass);
            }
        });
        return res;
    }

    Json::Value AllUsers()
    {
        Json::Value temp;
        Json::UInt i = 0;
        std::shared_lock<std::shared_mutex> lock{size_lock}; //gives readers of users the lock
        for (const auto &u : users)
        {
            //we know it contains this key but we call this func to grab mutex
            temp[i++] = u.first;
        }
        return temp;
    }

    void Save()
    {
        Json::StreamWriterBuilder builder;
        const std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());

        std::ofstream user_save("users.json");
        Json::Value temp;

        //loading info into json temp
        {
            std::shared_lock<std::shared_mutex> lock{size_lock};          //gives readers of users the lock
            std::unique_lock<std::shared_mutex> halt_funds{send_funds_l}; //halts all send fund requests
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
                users.try_emplace(u, temp[u]["balance"].asUInt(), std::move(temp[u]["password"].asString()));
            }
        }
    }
} bank;
