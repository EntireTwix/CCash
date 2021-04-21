#pragma once
#include <array>
#include <algorithm>
#include "transactions.hpp"

struct Log
{
    std::array<Transaction, 100> data;
    void AddTrans(const Transaction &v)
    {
        std::rotate(data.begin(), data.begin() + 1, data.end());
        data[99] = std::move(v);
    }
};
