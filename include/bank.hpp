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
            name, []() {}, init_pass);
    }
    bool AdminAddUser(const std::string &attempt, std::string &&name, uint_fast64_t init_bal, std::string &&init_pass)
    {
        const bool state = (admin_pass == attempt);
        if (state)
        {
            users.try_emplace_l(
                name, []() {}, init_bal, init_pass);
        }
        return state;
    }
    bool SendFunds(const std::string &a_name, const std::string &b_name, uint_fast64_t amount, const std::string &attempt)
    {
        //if A exists, A can afford it, and A's password matches
        bool state = false;

        users.modify_if(a_name, [&state, amount, &attempt](User &a) {
            if (state = (a.balance >= amount) && (a.password == attempt))
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
                //attempt to refund if destination doesnt exist
                users.modify_if(a_name, [amount](User &a) {
                    a.balance += amount;
                });
            }
        }

        return state;
    }
} Bank;