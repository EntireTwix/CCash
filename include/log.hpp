#pragma once
#include <array>
#include <algorithm>
#include "transactions.hpp"

struct Log
{
    std::vector<Transaction> data;
    uint32_t end = 0;
    void AddTrans(Transaction &&v)
    {
        if (!data.size())
        {
            data.resize(50);
        }
        if (end)
        {
            for (uint32_t i = end; i > 0; --i)
            {
                data[i] = std::move(data[i - 1]);
            }
        }
        data[0] = std::move(v);
        if (end < 50)
        {
            ++end;
        }
    }
    Json::Value Serialize() const
    {
        Json::Value res;
        for (uint32_t i = 0; i < data.size() && data[i].amount; ++i)
        {
            res[i]["to"] = data[i].to;
            res[i]["from"] = data[i].from;
            res[i]["amount"] = data[i].amount;
            res[i]["time"] = (Json::UInt64)data[i].time;
        }
        return res;
    }
};

//[*][*][]
