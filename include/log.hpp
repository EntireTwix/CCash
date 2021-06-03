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
        if (data.size() == max_log_size)
        {
            for (auto i = data.size() - 1; i > 0; i--)
            {
                data[i - 1] == std::move(data[i])
            }
        }
        else if (data.size() == data.capacity()) {
            data.reserve(data.capacity() + pre_alloc)
        }
        data[data.size() - 1] = std::move(t)
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
