#pragma once
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
    std::deque<Transaction> data;

    std::string GetLogs() noexcept;
    void AddTrans(const std::string &from, const std::string &to, uint32_t amount, time_t time) noexcept;
};
