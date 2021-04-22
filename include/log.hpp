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
            data.resize(25);
        }
        std::rotate(data.begin(), data.begin() + 1, data.end());
        data[24] = std::move(v);
    }
};
