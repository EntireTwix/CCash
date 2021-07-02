#pragma once
#include <json/json.h> //to be removed later
#include "xxhash_str.h"
#include "log.h"

struct User
{
    uint32_t balance = 0;
    XXH64_hash_t password;
#if MAX_LOG_SIZE > 0
    Log log;
#endif

    User(std::string &&init_pass) noexcept;
    User(uint32_t init_bal, std::string &&init_pass) noexcept;
    User(uint32_t init_bal, XXH64_hash_t init_pass) noexcept;
#if MAX_LOG_SIZE > 0
    User(uint32_t init_bal, XXH64_hash_t init_pass, const Json::Value &log_j) noexcept;
#endif

    Json::Value Serialize() const; //to be removed later
};
