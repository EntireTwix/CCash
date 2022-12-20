#include "transaction.h"

Transaction::Transaction() noexcept {};
Transaction::Transaction(const std::string &counterparty_str, bool receiving, uint32_t amount, time_t time) noexcept : counterparty(counterparty_str), receiving(receiving), amount(amount), time(time) {}
