#pragma once
#include <json/json.h>
#include <string>

struct User
{
    uint_fast32_t balance = 0;
    uint64_t password;
    bool is_admin = false;

    /**
     * @brief User constructor
     * 
     * @param init_pass initial password
     */
    User(std::string &&init_pass) : password(XXH64(init_pass.data(), init_pass.size(), 1)) {}

    /**
     * @brief User Constructor for admins
     * 
     * @param init_bal initial balance
     * @param init_pass initial password 
     */
    User(uint_fast32_t init_bal, std::string &&init_pass, bool state = false) : balance(init_bal), password(XXH64(init_pass.data(), init_pass.size(), 1)), is_admin(state) {}
    User(uint_fast32_t init_bal, uint64_t init_pass, bool state = false) : balance(init_bal), password(init_pass), is_admin(state) {}

    Json::Value Serialize() const
    {
        Json::Value res;
        res["balance"] = (Json::UInt)balance;
        res["password"] = (Json::UInt64)password;
        res["is_admin"] = is_admin;
        return res;
    }
};