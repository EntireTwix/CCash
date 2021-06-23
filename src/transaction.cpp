#include "transaction.h"

Transaction::Transaction() = default;
Transaction::Transaction(std::string from_str, std::string to_str, uint32_t amount, uint64_t time) : amount(amount), time(time)
{
    from = std::move(from_str);
    to = std::move(to_str);
}
Transaction::Transaction(std::string from_str, std::string to_str, uint32_t amount) : amount(amount)
{
    using namespace std::chrono;
    from = std::move(from_str);
    to = std::move(to_str);
    time = std::time(NULL);
}