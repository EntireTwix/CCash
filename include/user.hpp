#pragma once
#include <json/json.h>
#include <string>
#include "log.hpp"

struct User
{
    uint_fast32_t balance = 0;
    uint64_t password;
    Log log;

    /**
     * @brief User constructor
     * 
     * @param init_pass initial password
     */
    User(std::string &&init_pass) : password(XXH3_64bits(init_pass.data(), init_pass.size())) {}

    /**
     * @brief User Constructor for admins
     * 
     * @param init_bal initial balance
     * @param init_pass initial password 
     */
    User(uint_fast32_t init_bal, std::string &&init_pass, bool state = false) : balance(init_bal), password(XXH3_64bits(init_pass.data(), init_pass.size())) {}
    User(uint_fast32_t init_bal, uint64_t init_pass, bool state = false) : balance(init_bal), password(init_pass) {}

    Json::Value Serialize() const
    {
        Json::Value res;
        res["balance"] = (Json::UInt)balance;
        res["password"] = (Json::UInt64)password;
        return res;
    }
};