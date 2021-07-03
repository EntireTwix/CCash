#pragma once
#include <string>
#include <cstdint>
#include <ctime>

struct Transaction
{
    std::string from = "", to = "";
    uint32_t amount = 0;
    time_t time = 0;

    Transaction();
    Transaction(const std::string &from_str, const std::string &to_str, uint32_t amount, time_t time);
    Transaction(const std::string &from_str, const std::string &to_str, uint32_t amount);
};
