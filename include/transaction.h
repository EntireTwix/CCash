#pragma once
#include <string>
#include <cstdint>
#include <ctime>

struct Transaction
{
    std::string counterparty = "";
    bool receiving = false;
    uint32_t amount = 0;
    time_t time = 0;

    Transaction() noexcept;
    Transaction(const std::string &counterparty_str, bool receiving, uint32_t amount, time_t time) noexcept;
};

/*
TODO: v1 vs v2 functionality
TODO: FBE

TODO: v2/api
TODO: update stats on run (203 bytes)
TODO: update Docs
*/