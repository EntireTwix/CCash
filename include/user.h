#pragma once
#include <json/json.h> //remove later
#include <string>
#include <algorithm>
#include <xxhash.h>
#include "log.h"

struct User
{
    uint32_t balance = 0;
    uint64_t password;
    Log log;

    User(const std::string &init_pass);
    User(uint32_t init_bal, const std::string &init_pass);
    User(uint32_t init_bal, uint64_t init_pass);
    User(uint32_t init_bal, uint64_t init_pass, const Json::Value &log_j);

    Json::Value Serialize() const;
};
