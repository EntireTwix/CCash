#include "log.h"

void Log::AddTrans(const Transaction &t) noexcept
{
    log_flag.SetChangesOn();
#if MAX_LOG_SIZE == 1
    data = t;
#else
    if (data.size() == MAX_LOG_SIZE) // If we hit the max size
    {
        for (size_t i = 1; i < data.size(); i++) // Make room at the back
        {
            data[i - 1] = std::move(data[i]); // Shifts everything left
        }
        data[data.size() - 1] = std::move(t); // Place new in opened spot
        return;
    }
    data.push_back(t); // In either case we have space under max length, move to new spot
#endif
}

std::string Log::GetLogs() noexcept
{
    if (log_flag.GetChangeState() && data.size()) //if there are changes
    {
        //re-generate snapshot
        //({\"amount\":1,\"from\":\"\",\"time\":1625943626,\"to\":\"\"}, + (2*max_name_size)+10+10) * # of logs) + 1
        size_t predicted_size = ((58 + (2 * max_name_size)) * data.size()) + 1;
        if (log_snapshot.capacity() < predicted_size)
        {
            log_snapshot.reserve(predicted_size);
        }
        log_snapshot = '[';
        for (size_t i = 0; i < data.size(); ++i)
        {
            log_snapshot += "{\"to\":\"";
            log_snapshot += data[i].to;
            log_snapshot += "\",\"from\":\"";
            log_snapshot += data[i].from;
            log_snapshot += "\",\"amount\":";
            log_snapshot += std::to_string(data[i].amount);
            log_snapshot += ",\"time\":";
            log_snapshot += std::to_string(data[i].time);
            log_snapshot += "},";
        }
        log_snapshot[log_snapshot.size() - 1] = ']';
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
