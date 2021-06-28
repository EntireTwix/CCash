#pragma once
#include <json/json.h> //to be removed later
#include <string>
#include <xxhash.h>
#include "log.h"

struct User
{
    uint32_t balance = 0;
    uint64_t password;
#if MAX_LOG_SIZE > 0
    Log log;
#endif

    User(const std::string &init_pass);
    User(uint32_t init_bal, const std::string &init_pass);
    User(uint32_t init_bal, uint64_t init_pass);
#if MAX_LOG_SIZE > 0
    User(uint32_t init_bal, uint64_t init_pass, const Json::Value &log_j);
#endif

    Json::Value Serialize() const; //to be removed later
};
