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
        log_snapshot.resize(0);
        //re-generate snapshot
        size_t predicted_size = ((59 + (2 * max_name_size)) * data.size()) + 1;
        if (log_snapshot.capacity() < predicted_size)
        {
            log_snapshot.reserve(predicted_size);
        }
        log_snapshot = '['; //1
        for (size_t i = 0; i < data.size(); ++i)
        {
            log_snapshot += "{\"to\":\"";                   //8
            log_snapshot += data[i].to;                     //max_name_size?
            log_snapshot += "\",\"from\":\"";               //10
            log_snapshot += data[i].from;                   //max_name_size?
            log_snapshot += "\",\"amount\":";               //12
            log_snapshot += std::to_string(data[i].amount); //10?
            log_snapshot += ",\"time\":";                   //8
            log_snapshot += std::to_string(data[i].time);   //10?
            log_snapshot += "},";                           //2
        }
        log_snapshot.back() = ']';
        log_flag.SetChangesOff();
    }
    return log_snapshot;
}