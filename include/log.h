#pragma once
#include <json/json.h> // to be removed later
#include <array>
#include <algorithm>
#include "consts.hpp"
#include "change_flag.h"
#include "transaction.h"

struct Log
{
private:
    ChangeFlag log_flag;
    Json::Value log_snapshot;

public:
    std::vector<Transaction> data;

    const Json::Value &GetLog() noexcept;
    void AddTrans(Transaction &&t) noexcept;
    Json::Value Serialize() const; // to be removed later
};
