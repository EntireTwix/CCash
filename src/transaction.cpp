#include "transaction.h"

Transaction::Transaction() noexcept {};
Transaction::Transaction(const std::string &counterparty, bool receiving, uint32_t amount, time_t time) noexcept : counterparty(counterparty), receiving(receiving), amount(amount), time(time) {}
