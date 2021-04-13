#pragma once
#include <json/json.h>
#include <string>

struct User
{
    uint_fast32_t balance = 0;
    size_t password;

    /**
     * @brief User constructor
     * 
     * @param init_pass initial password
     */
    User(std::string &&init_pass) : password(std::hash<std::string>{}(init_pass)) {}

    /**
     * @brief User Constructor for admins
     * 
     * @param init_bal initial balance
     * @param init_pass initial password 
     */
    User(uint_fast32_t init_bal, std::string &&init_pass) : balance(init_bal), password(std::hash<std::string>{}(init_pass)) {}

    Json::Value Serialize() const
    {
        Json::Value res;
        res["balance"] = (Json::UInt)balance;
        res["password"] = (Json::LargestUInt)password;
        return res;
    }
};