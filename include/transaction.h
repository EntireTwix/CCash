#pragma once
#include <string>
#include <chrono>
#include <cstdint>

struct Transaction
{
    std::string from = "", to = "";
    uint32_t amount = 0;
    uint64_t time = 0;

    Transaction();
    Transaction(std::string from_str, std::string to_str, uint32_t amount, uint64_t time);
    Transaction(std::string from_str, std::string to_str, uint32_t amount);
};
