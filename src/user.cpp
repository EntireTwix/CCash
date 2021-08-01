#include "user.h"

/**
     * @brief User Constructor for admins
     * 
     * @param init_bal initial balance
     * @param init_pass initial password 
     */
User::User(uint32_t init_bal, const std::string &init_pass) noexcept : balance(init_bal), password(xxHashStringGen{}(init_pass)) {}

/**
     * @brief User Constructor for loading
     * 
     * @param init_bal 
     * @param init_pass 
     */
User::User(uint32_t init_bal, XXH64_hash_t init_pass) noexcept : balance(init_bal), password(init_pass) {}

User::User(const bank_dom::User &u) noexcept : balance(u.balance), password(u.password)
{
#if MAX_LOG_SIZE > 0
    if (u.logs)
    {
        uint32_t i = 0;
        if (MAX_LOG_SIZE < u.logs.value().data.size())
        {
            i = (u.logs.value().data.size() - MAX_LOG_SIZE);
        }
        for (; i < u.logs.value().data.size(); ++i)
        {
            const bank_dom::Transaction &temp = u.logs.value().data[i];
            log.data.emplace_front(temp.from, temp.to, temp.amount, temp.time);
        }
    }
#endif
}
bank_dom::User User::Encode() const noexcept
{
#if MAX_LOG_SIZE > 0
    if (this->log.data.size())
    {
        bank_dom::Logs save_log;
        save_log.data.reserve(this->log.data.size());
        for (const Transaction &t : this->log.data)
        {
            save_log.data.emplace_back(t.from, t.to, t.amount, t.time);
        }
        return bank_dom::User(balance, password, save_log);
    }
    else
    {
        return bank_dom::User(balance, password, std::nullopt);
    }
#else
    return bank_dom::User(balance, password, std::nullopt);
#endif
}
