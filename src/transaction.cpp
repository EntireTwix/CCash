#include "transaction.h"

Transaction::Transaction() = default;
Transaction::Transaction(const std::string &from_str, const std::string &to_str, uint32_t amount, time_t time_val) : from(from_str), to(to_str), amount(amount), time(time_val) {}
Transaction::Transaction(const std::string &from_str, const std::string &to_str, uint32_t amount) : from(from_str), to(to_str), amount(amount) { time = std::time(NULL); }