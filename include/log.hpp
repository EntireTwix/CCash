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
};

//[*][*][]
