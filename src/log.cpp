#include "log.h"

void Log::AddTrans(Transaction &&t)
{
    if (data.size() == MAX_LOG_SIZE) // If we hit the max size
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
        data.reserve(data.capacity() + PRE_LOG_SIZE); // Reserve more memory
    }
    data.push_back(std::move(t)); // In either case we have space under max length, move to new spot
    log_flag.SetChangesOn();
}

const Json::Value &Log::GetLog()
{
    if (log_flag.GetChangeState()) //if there are changes
    {
        //re-generate snapshot
        Json::Value res;
        for (uint32_t i = data.size(); i > 0; --i)
        {
            res[i - 1]["to"] = data[data.size() - i].to;
            res[i - 1]["from"] = data[data.size() - i].from;
            res[i - 1]["amount"] = (Json::UInt)data[data.size() - i].amount;
#ifdef _USE_32BIT_TIME_T
            res[i - 1]["time"] = (Json::UInt)data[data.size() - i].time;
#else
            res[i - 1]["time"] = (Json::UInt64)data[data.size() - i].time;
#endif
        }
        log_flag.SetChangesOff();
        return log_snapshot = res;
    }
    else
    {
        return log_snapshot;
    }
}

Json::Value Log::Serialize() const
{
    Json::Value res;
    for (uint32_t i = 0; i < data.size(); ++i)
    {
        res[i]["to"] = data[i].to;
        res[i]["from"] = data[i].from;
        res[i]["amount"] = (Json::UInt)data[i].amount;
#ifdef _USE_32BIT_TIME_T
        res[i]["time"] = (Json::UInt)data[i].time;
#else
        res[i]["time"] = (Json::UInt64)data[i].time;
#endif
    }
    return res;
}
