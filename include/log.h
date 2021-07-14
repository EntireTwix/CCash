#pragma once
#include <json/json.h> // to be removed later
#include <deque>
#include "ccash_config.hpp"
#include "change_flag.h"
#include "transaction.h"
#include "simdjson.h"

using namespace simdjson;

struct Log
{
private:
    ChangeFlag<true> log_flag;
    std::string log_snapshot = "null";

public:
#if MAX_LOG_SIZE == 1
    Transaction data;
#else
    std::deque<Transaction> data;
#endif

    std::string GetLogs() noexcept;
    void AddTrans(const std::string &from, const std::string &to, uint32_t amount, time_t time) noexcept;
    Json::Value Serialize() const; // to be removed later
};
