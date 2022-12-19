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

#if USE_DEPRECATED_ENDPOINTS
    ChangeFlag<true> log_flag;
    std::string log_snapshot = "null";
#endif

    ChangeFlag<true> log_flag_v2;
    std::string log_snapshot_v2 = "null";

public:
    std::deque<Transaction> data;

#if USE_DEPRECATED_ENDPOINTS
    std::string GetLogs(const std::string& name) noexcept;
#endif
    std::string GetLogsV2() noexcept;
    void AddTrans(const std::string &counterparty_str, bool receiving, uint32_t amount, time_t time) noexcept;
};
