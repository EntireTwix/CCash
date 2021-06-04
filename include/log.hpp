#pragma once
#include <array>
#include <algorithm>
#include "log_consts.hpp"
#include "transactions.hpp"

struct Log
{
    std::vector<Transaction> data;
    void AddTrans(Transaction &&v)
    {
        if (data.capacity() == data.size() && data.size() < max_log_size) //if memory reserved is full and max isnt reached
        {
            if (data.size() + pre_log_size > max_log_size) //if prefetched memory is larger then max
            {
                data.reserve(max_log_size); //just allocate max
            }
            else
            {
                data.reserve(data.size() + pre_log_size); //prefetching memory
            }
        }
        if (data.size() == max_log_size)
        {
            for (size_t i = 0; i < data.size() - 1; ++i)
            {
                data[i] = std::move(data[i + 1]);
            }
            data[data.size() - 1] = std::move(v);
        }
        else
        {
            data.push_back(std::move(v));
        }
    }
    Json::Value Serialize() const
    {
        Json::Value res;
        for (uint32_t i = 0; i < data.size(); ++i)
        {
            res[i]["to"] = data[i].to;
            res[i]["from"] = data[i].from;
            res[i]["amount"] = (Json::UInt)data[i].amount;
            res[i]["time"] = (Json::UInt64)data[i].time;
        }
        return res;
    }
};
