#pragma once
#include <json/json.h>
#include <string>
#include "log.hpp"

struct User
{
    uint32_t balance = 0;
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
    User(uint32_t init_bal, std::string &&init_pass) : balance(init_bal), password(XXH3_64bits(init_pass.data(), init_pass.size())) {}

    /**
     * @brief User Constructor for loading
     * 
     * @param init_bal 
     * @param init_pass 
     */
    User(uint32_t init_bal, uint64_t init_pass) : balance(init_bal), password(init_pass) {}
    User(uint32_t init_bal, uint64_t init_pass, const Json::Value &log_j) : balance(init_bal), password(init_pass)
    {
        if (log_j.size())
        {
            if (max_log_size > (log_j.size() + pre_log_size)) //if current imported log's size + prefetch amount is less then max
            {
                //std::cout << "allocating " << log_j.size() + pre_log_size << '\n';
                log.data.reserve(log_j.size() + pre_log_size); //allocate that amount
            }
            else
            {
                //std::cout << "allocating " << max_log_size << '\n';
                log.data.reserve(max_log_size); //allocate max amount
            }
            for (uint32_t i = log_j.size(); i > 0; ++i)
            {
                log.data.push_back(std::move(Transaction(log_j[i - 1]["from"].asCString(), log_j[i - 1]["to"].asCString(), log_j[i - 1]["amount"].asUInt(), log_j[i - 1]["time"].asUInt64())));
            }
        }
    }

    Json::Value Serialize() const
    {
        Json::Value res;
        res["balance"] = (Json::UInt)balance;
        res["password"] = (Json::UInt64)password;
        if constexpr (max_log_size)
        {
            res["log"] = log.Serialize();
        }
        return res;
    }
};
