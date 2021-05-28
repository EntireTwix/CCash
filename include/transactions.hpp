#pragma once
#include <chrono>
#include <cstdint>

struct Transaction
{
    std::string from = "", to = "";
    uint32_t amount = 0;
    uint64_t time = 0;

    Transaction() = default;
    Transaction(std::string from_str, std::string to_str, uint32_t amount, uint64_t time) : amount(amount), time(time)
    {
        from = std::move(from_str);
        to = std::move(to_str);
    }
    Transaction(std::string from_str, std::string to_str, uint32_t amount) : amount(amount)
    {
        using namespace std::chrono;
        from = std::move(from_str);
        to = std::move(to_str);
        time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    }
};
