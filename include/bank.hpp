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
        const bool state = (admin_pass == attempt);
        if (state)
        {
            users.try_emplace_l(
                name, [](User &) {}, init_bal, std::forward<std::string &&>(init_pass));
        }
        return state;
    }

    bool DelUser(const std::string &name, const std::string &attempt)
    {
        bool state = false;
        //if password for user is correct, and user exists
        users.if_contains(name, [&attempt, &state](const User &u) {
            if (attempt == u.password)
            {
                state = true;
            }
        });
        if (state) //correct password
        {
            state = users.erase_if(name, [](const User &) { return true; });
        }
        return state;
    }
    bool AdminDelUser(const std::string &name, const std::string &attempt)
    {
        bool state = (attempt == admin_pass);
        if (state)
        {
            state = users.erase_if(name, [&state](const User &) { state = true; return state; });
        }
        return state;
    }

    int_fast64_t GetBal(const std::string &name)
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