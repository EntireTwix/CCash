#include "transaction.h"

Transaction::Transaction() = default;
Transaction::Transaction(std::string from_str, std::string to_str, uint32_t amount, time_t time) : from(from_str), to(to_str), amount(amount) {}
Transaction::Transaction(std::string from_str, std::string to_str, uint32_t amount) : from(from_str), to(to_str), amount(amount) { time = std::time(NULL); }