#pragma once
#include <array>
#include <algorithm>
#include "transactions.hpp"

struct Log
{
    std::vector<Transaction> data;
    void AddTrans(Transaction &&v)
    {
        if (!data.size())
        {
            data.resize(50);
        }
        for (size_t i = 1; i < data.size(); ++i)
        {
            if (!data[i].amount)
            {
                data[i] = std::move(data[i - 1]);
                break;
            }
            data[i] = std::move(data[i - 1]);
        }
        data[0] = std::move(v);
    }
};
