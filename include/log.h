#pragma once
#include <json/json.h>
#include <array>
#include <algorithm>
#include "consts.hpp"
#include "transaction.h"

struct Log
{
    std::vector<Transaction> data;
    void AddTrans(Transaction &&t);
    Json::Value Serialize() const;
};
