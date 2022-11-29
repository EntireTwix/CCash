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
#if API_VERSION >= 2
    std::string log_snapshot_v2 = "null";
#endif

public:
    std::deque<Transaction> data;

    std::string GetLogs(const std::string& name) noexcept;
#if API_VERSION >= 2
    std::string GetLogsV2() noexcept;
#endif
    void AddTrans(const std::string &counterparty_str, bool receiving, uint32_t amount, time_t time) noexcept;
};
