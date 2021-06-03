#pragma once
#include <array>
#include <algorithm>
#include "log_consts.hpp"
#include "transactions.hpp"

struct Log
{
    std::vector<Transaction> data;
    uint32_t end = 0;
    void AddTrans(Transaction &&v)
    {
        end += (end < max_log_size); //branchless
        if (data.size() <= end && end < max_log_size) //if memory reserved is full and max isnt reached
        {
            if (data.size() + pre_log_size > max_log_size) //if prefetched memory is larger then max
            {
                data.resize(max_log_size); //just allocate max
            }
            else
            {
                data.resize(data.size() + pre_log_size); //prefetching memory
            }
        }
        for (uint32_t i = end - 1; i > 0; --i) //size: 10, 9-1, all moved to the right one
        {
            data[i] = std::move(data[i - 1]);
        }
        data[0] = std::move(v); //override first
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