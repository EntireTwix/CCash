#pragma once
#include <array>
#include <algorithm>
#include "transactions.hpp"

struct Log
{
    std::array<Transaction, 25> data;
    void AddTrans(Transaction &&v)
    {
        std::rotate(data.begin(), data.begin() + 1, data.end());
        data[24] = std::move(v);
    }
};
