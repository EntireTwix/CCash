#pragma once
#include <json/json.h>
#include <string>

struct User
{
    uint_fast32_t balance = 0;
    uint_fast64_t password;

    /**
     * @brief User constructor
     * 
     * @param init_pass initial password
     */
    User(std::string &&init_pass) : password(XXH64(init_pass.data(), init_pass.size(), 0)) {}

    /**
     * @brief User Constructor for admins
     * 
     * @param init_bal initial balance
     * @param init_pass initial password 
     */
    User(uint_fast32_t init_bal, std::string &&init_pass) : balance(init_bal), password(XXH64(init_pass.data(), init_pass.size(), 0)) {}

    Json::Value Serialize() const
    {
        Json::Value res;
        res["balance"] = (Json::UInt)balance;
        res["password"] = (Json::LargestUInt)password;
        return res;
    }
};