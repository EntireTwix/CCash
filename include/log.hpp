#pragma once
#include <array>
#include <algorithm>
#include "log_consts.hpp"
#include "transactions.hpp"

struct Log
{
    std::vector<Transaction> data;
    void AddTrans(Transaction &&t)
    {
        if (data.size() == max_log_size) // If we hit the max size
        {
            for (auto i = data.size() - 1; i > 0; i--) // Make room at the back
            {
                data[i - 1] == std::move(data[i])
            }
        }
        else if (data.size() == data.capacity()) // If we haven't hit the max but hit capacity
        {
            data.reserve(data.capacity() + pre_alloc) // Reserve more memory
        }
        data[data.size() - 1] = std::move(t) // In any case, place new at the back
    }
    Json::Value Serialize() const
    {
        Json::Value res;
        for (uint32_t i = 0; i < end; ++i)
        {
            res[i]["to"] = data[i].to;
            res[i]["from"] = data[i].from;
            res[i]["amount"] = (Json::UInt)data[i].amount;
            res[i]["time"] = (Json::UInt64)data[i].time;
        }
        return res;
    }
};
