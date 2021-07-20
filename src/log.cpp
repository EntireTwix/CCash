#include "log.h"

void Log::AddTrans(const std::string &from, const std::string &to, uint32_t amount, time_t time) noexcept
{
    log_flag.SetChangesOn();
    if (data.size() == MAX_LOG_SIZE)
    {
        data.pop_back();
    }
    data.emplace_back(from, to, amount, time);
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
        log_snapshot.back() = ']';
        log_flag.SetChangesOff();
    }
    return log_snapshot;
}