#pragma once
#include <cstdint>

//24 bytes total
struct Transaction
{
    std::string from = "", to = "";
    uint32_t amount = 0;

    void Concatinate(std::string &s)
    {
        if (s.size() > 10)
        {
            s.resize(10);
            s[7] = '.';
            s[8] = '.';
            s[9] = '.';
        }
    }

    Transaction() = default;
    Transaction(std::string from_str, std::string to_str, uint32_t amount) : amount(amount)
    {
        Concatinate(from_str);
        Concatinate(to_str);
        from = std::move(from_str);
        to = std::move(to_str);
    }
};