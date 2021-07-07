#include "log.h"

void Log::AddTrans(Transaction &&t) noexcept
{
#if MAX_LOG_SIZE == 1
    data = std::move(t);
#else
    if (data.size() == MAX_LOG_SIZE) // If we hit the max size
    {
        for (uint32_t i = 1; i < data.size(); i++) // Make room at the back
        {
            data[i - 1] = std::move(data[i]); // Shifts everything left
        }
        data[data.size() - 1] = std::move(t); // Place new in opened spot
        return;
    }
    data.push_back(std::move(t)); // In either case we have space under max length, move to new spot
#endif
    log_flag.SetChangesOn();
}

const Json::Value &Log::GetLog() noexcept
{
    if (log_flag.GetChangeState()) //if there are changes
    {
        //re-generate snapshot
        Json::Value res;
#if MAX_LOG_SIZE == 1
        res[0]["to"] = data.to;
        res[0]["from"] = data.from;
#ifdef _USE_32BIT_TIME_T
        res[0]["time"] = (Json::UInt)data.time;
#else
        res[0]["time"] = (Json::UInt64)data.time;
#endif
#else
        for (uint32_t i = data.size(); i > 0; --i)
        {
            res[i - 1]["to"] = data[data.size() - i].to;
            res[i - 1]["from"] = data[data.size() - i].from;
            res[i - 1]["amount"] = (Json::UInt)data[data.size() - i].amount;
            res[i - 1]["time"] = (Json::Int64)data[data.size() - i].time;
        }
#endif
        log_snapshot = res;
        log_flag.SetChangesOff();
    }
    return log_snapshot;
}

Json::Value Log::Serialize() const
{
    Json::Value res;
#if MAX_LOG_SIZE == 1
    res[0]["to"] = data.to;
    res[0]["from"] = data.from;
#ifdef _USE_32BIT_TIME_T
    res[0]["time"] = (Json::UInt)data.time;
#else
    res[0]["time"] = (Json::UInt64)data.time;
#endif
#else
    for (uint32_t i = 0; i < data.size(); ++i)
    {
        res[i]["to"] = data[i].to;
        res[i]["from"] = data[i].from;
        res[i]["amount"] = (Json::UInt)data[i].amount;
#ifdef _USE_32BIT_TIME_T
        res[i]["time"] = (Json::Int)data[i].time;
#else
        res[i]["time"] = (Json::Int64)data[i].time;
#endif
    }
#endif
    return res;
}
