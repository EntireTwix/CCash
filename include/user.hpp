#pragma once
#include <json/json.h>
#include <string>
#include <algorithm>
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
            auto size = ((log_j.size() / pre_log_size) + 1) * pre_log_size; // Ensures that we have a log size aligned on a multiple of `pre_log_size`
            log.data.reserve(std::min(size, max_log_size)); // Ensures that the log size is under `max_log_size`
            for (uint32_t i = 0; i < log.size(); i++) // Matches the logs
            {
                log.data[i] = std::move(Transaction(
                    log_j[i]["from"].asCString(),
                    log_j[i]["to"].asCString(),
                    log_j[i]["amount"].asUInt(),
                    log_j[i]["time"].asUInt64()
                ));
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
