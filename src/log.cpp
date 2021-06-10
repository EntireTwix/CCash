#include "log.h"

void Log::AddTrans(Transaction &&t)
{
    if (data.size() == max_log_size) // If we hit the max size
    {
        for (uint32_t i = 1; i < data.size(); i++) // Make room at the back
        {
            data[i - 1] = std::move(data[i]); // Shifts everything left
        }
        data[data.size() - 1] = std::move(t); // Place new in opened spot
        return;
    }
    else if (data.size() == data.capacity()) // If we haven't hit the max but hit capacity
    {
        data.reserve(data.capacity() + pre_log_size); // Reserve more memory
    }
    data.push_back(std::move(t)); // In either case we have space under max length, move to new spot
}
Json::Value Log::Serialize() const
{
    Json::Value res;
    for (uint32_t i = 0; i < data.size(); ++i)
    {
        res[i]["to"] = data[i].to;
        res[i]["from"] = data[i].from;
        res[i]["amount"] = (Json::UInt)data[i].amount;
        res[i]["time"] = (Json::UInt64)data[i].time;
    }
    return res;
}
