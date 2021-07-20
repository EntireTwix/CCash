#pragma once
#include "xxhash_str.h"
#include "bank_dom_final_models.h"
#include "log.h"

struct User
{
    uint32_t balance;
    XXH64_hash_t password;
#if MAX_LOG_SIZE > 0
    Log log;
#endif

    User(uint32_t init_bal, const std::string &init_pass) noexcept;
    User(uint32_t init_bal, XXH64_hash_t init_pass) noexcept;
#if MAX_LOG_SIZE > 0
    User(uint32_t init_bal, XXH64_hash_t init_pass, const Json::Value &log_j) noexcept;
#endif
    User(const bank_dom::User &u) noexcept;
    bank_dom::User Encode() const noexcept;
};
