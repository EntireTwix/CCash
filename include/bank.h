#pragma once
#include <iostream> //temporary
#include <fstream>
#include <shared_mutex>
#include <parallel-hashmap/parallel_hashmap/phmap.h>
#include "bank_resp.h"
#include "user.h"

#if (CONSERVATIVE_DISK_SAVE && MAX_LOG_SIZE < 0) && !MULTI_THREADED
#include "change_flag.h"
#endif

class Bank
{
#if MULTI_THREADED
    phmap::parallel_flat_hash_map<
        std::string, User,
        xxHashStringGen,
        phmap::priv::hash_default_eq<std::string>,
        phmap::priv::Allocator<phmap::priv::Pair<const std::string, User>>,
        4UL,
        std::mutex>
        users;
#else
    phmap::parallel_flat_hash_map<std::string, User, xxHashStringGen> users;
#endif

private:
#if CONSERVATIVE_DISK_SAVE
#if MULTI_THREADED
    ChangeFlag<false> save_flag;
#else
    bool save_flag = false;
#endif
#endif

    //must grab as shared if the operation is gonna modify "users"'s size or can be caught in a intermediary state such as SendFunds()
    //must grab as unique if the operation is gonna user iterators
    std::shared_mutex iter_lock;

public:
    std::string admin_account;

    size_t NumOfUsers() const noexcept;
    size_t NumOfLogs() const noexcept;
    size_t SumBal() const noexcept;

    BankResponse GetBal(const std::string &name) const noexcept;
#if MAX_LOG_SIZE > 0
    BankResponse GetLogs(const std::string &name) noexcept;
#endif
    BankResponse SendFunds(const std::string &a_name, const std::string &b_name, uint32_t amount) noexcept;
    bool VerifyPassword(const std::string &name, const std::string_view &attempt) const noexcept;

    void ChangePassword(const std::string &name, const std::string &new_pass) noexcept;
    BankResponse SetBal(const std::string &name, uint32_t amount) noexcept;
    BankResponse ImpactBal(const std::string &name, int64_t amount) noexcept;
    bool Contains(const std::string &name) const noexcept;
    BankResponse PruneUsers(time_t threshold_time, uint32_t threshold_bal) noexcept;

    BankResponse AddUser(const std::string &name, uint32_t init_bal, const std::string &init_pass) noexcept;
    BankResponse DelUser(const std::string &name) noexcept;
    void DelSelf(const std::string &name) noexcept;

    const char *Save();
    void Load();
};