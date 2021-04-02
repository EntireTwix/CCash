#pragma once
#include "parallel-hashmap/parallel_hashmap/phmap.h"
#include "user.hpp"

class
{
private:
    phmap::parallel_node_hash_map<
        std::string, User,
        phmap::priv::hash_default_hash<std::string>,
        phmap::priv::hash_default_eq<std::string>,
        phmap::priv::Allocator<phmap::priv::Pair<const std::string, User>>,
        4UL,
        std::mutex>
        users;

public:
    std::string admin_pass;

    bool AddUser(const std::string &name, std::string &&init_pass)
    {
        return users.try_emplace_l(
            name, [](User &) {}, std::forward<std::string &&>(init_pass));
    }
    bool AdminAddUser(const std::string &attempt, std::string &&name, uint_fast32_t init_bal, std::string &&init_pass)
    {
        bool state = (admin_pass == attempt);
        if (state)
        {
            state = users.try_emplace_l(
                name, [](User &) {}, init_bal, std::forward<std::string &&>(init_pass));
        }
        return state;
    }

    bool DelUser(const std::string &name, const std::string &attempt)
    {
        return users.erase_if(name, [&attempt](const User &u) { return (attempt == u.password); });
    }
    bool AdminDelUser(const std::string &name, const std::string &attempt)
    {
        return users.erase_if(name, [this, &attempt](const User &) { return (admin_pass == attempt); });
    }

    int_fast64_t GetBal(const std::string &name) const
    {
        int_fast64_t res = -1;
        users.if_contains(name, [&res](const User &u) {
            res = u.balance;
        });
        return res;
    }

    bool SendFunds(const std::string &a_name, const std::string &b_name, uint_fast32_t amount, const std::string &attempt)
    {
        //if A exists, A can afford it, and A's password matches
        bool state = false;

        users.modify_if(a_name, [&state, amount, &attempt](User &a) {
            if (state = (a.balance >= amount) && (a.password == attempt), state)
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
} Bank;