#include "user.h"

/**
     * @brief User constructor
     * 
     * @param init_pass initial password
     */
User::User(std::string &&init_pass) noexcept : password(xxHashStringGen{}(init_pass)) {}

/**
     * @brief User Constructor for admins
     * 
     * @param init_bal initial balance
     * @param init_pass initial password 
     */
User::User(uint32_t init_bal, std::string &&init_pass) noexcept : balance(init_bal), password(xxHashStringGen{}(init_pass)) {}

/**
     * @brief User Constructor for loading
     * 
     * @param init_bal 
     * @param init_pass 
     */
User::User(uint32_t init_bal, XXH64_hash_t init_pass) noexcept : balance(init_bal), password(init_pass) {}
#if MAX_LOG_SIZE > 0
User::User(uint32_t init_bal, XXH64_hash_t init_pass, const Json::Value &log_j) noexcept : balance(init_bal), password(init_pass)
{
    if (log_j.size())
    {
        for (uint32_t i = (log_j.size() - MAX_LOG_SIZE) * (log_j.size() > MAX_LOG_SIZE); i < log_j.size(); i++)
        {
#if MAX_LOG_SIZE == 1
            log.data = (
#else
            log.data.push_back(
#endif
                Transaction(
                    log_j[i]["from"].asCString(),
                    log_j[i]["to"].asCString(),
                    log_j[i]["amount"].asUInt(),
#ifdef _USE_32BIT_TIME_T
                    log_j[i]["time"].asUInt()));
#else
                    log_j[i]["time"].asUInt64()));
#endif
        }
    }
}
#endif

Json::Value User::Serialize() const
{
    Json::Value res;
    res["balance"] = (Json::UInt)balance;
    res["password"] = (Json::UInt64)password;
#if MAX_LOG_SIZE > 0
    res["log"] = log.Serialize();
#endif
    return res;
}