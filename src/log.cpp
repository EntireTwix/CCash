#include "log.h"

void Log::AddTrans(const std::string &counterparty_str, bool receiving, uint32_t amount, time_t time) noexcept
{
    log_flag.SetChangesOn();
    if (data.size() == MAX_LOG_SIZE)
    {
        data.pop_back();
    }
    data.emplace_back(counterparty_str, receiving, amount, time);
}

std::string Log::GetLogs(const std::string& name) noexcept
{
    if (log_flag.GetChangeState() && data.size()) //if there are changes
    {
        log_snapshot.resize(0);
        //re-generate snapshot
        size_t predicted_size = ((57 + (2 * max_name_size)) * data.size()) + 1;
        if (log_snapshot.capacity() < predicted_size)
        {
            log_snapshot.reserve(predicted_size);
        }
        log_snapshot = '['; //1
        for (size_t i = 0; i < data.size(); ++i)
        {
            log_snapshot += "{\"to\":\"";                                                       //7
            log_snapshot += data[i].receiving? name : data[i].counterparty;                     //max_name_size?
            log_snapshot += "\",\"from\":\"";                                                   //10
            log_snapshot += data[i].receiving? data[i].counterparty : name;                     //max_name_size?
            log_snapshot += "\",\"amount\":";                                                   //11
            log_snapshot += std::to_string(data[i].amount);                                     //10?
            log_snapshot += ",\"time\":";                                                       //8
            log_snapshot += std::to_string(data[i].time);                                       //10?
            log_snapshot += "},";                                                               //2
        }
        log_snapshot.back() = ']';
        log_flag.SetChangesOff();
    }

    return log_snapshot;
}
std::string Log::GetLogsV2() noexcept
{
    if (log_flag.GetChangeState() && data.size()) //if there are changes
    {
        log_snapshot_v2.resize(0);
        //re-generate snapshot
        size_t predicted_size = ((77 + max_name_size) * data.size()) + 1;
        if (log_snapshot_v2.capacity() < predicted_size)
        {
            log_snapshot_v2.reserve(predicted_size);
        }
        log_snapshot_v2 = '['; //1
        for (size_t i = 0; i < data.size(); ++i)
        {
            log_snapshot_v2 += "{\"counterparty\":\"";                                             //17
            log_snapshot_v2 += data[i].counterparty;                                               //max_name_size?
            log_snapshot_v2 += "\",\"receiving\":\"";                                              //15
            log_snapshot_v2 += std::to_string(data[i].receiving);                                  //4
            log_snapshot_v2 += "\",\"amount\":";                                                   //11
            log_snapshot_v2 += std::to_string(data[i].amount);                                     //10?
            log_snapshot_v2 += ",\"time\":";                                                       //8
            log_snapshot_v2 += std::to_string(data[i].time);                                       //10?
            log_snapshot_v2 += "},";                                                               //2
        }
        log_snapshot_v2.back() = ']';
        log_flag.SetChangesOff();
    }

    return log_snapshot_v2;
}
