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
    const Json::Value &GetLog();

    std::vector<Transaction> data;
    void AddTrans(Transaction &&t);
    Json::Value Serialize() const; // to be removed later
};
