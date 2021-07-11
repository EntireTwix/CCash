#include "transaction.h"

Transaction::Transaction() noexcept = default;
Transaction::Transaction(const std::string &from_str, const std::string &to_str, uint32_t amount, time_t time_val) noexcept : from(from_str), to(to_str), amount(amount), time(time_val) {}
Transaction::Transaction(const std::string &from_str, const std::string &to_str, uint32_t amount) noexcept : from(from_str), to(to_str), amount(amount) { time = std::time(NULL); }