#pragma once
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
    static phmap::parallel_flat_hash_map<
        std::string, User,
        xxHashStringGen,
        phmap::priv::hash_default_eq<std::string>,
        phmap::priv::Allocator<phmap::priv::Pair<const std::string, User>>,
        4UL,
        std::mutex>
        users;
#else
    static phmap::parallel_flat_hash_map<std::string, User, xxHashStringGen> users;
#endif

private:
#if CONSERVATIVE_DISK_SAVE
    static ChangeFlag<false> save_flag;
#endif

    //must grab as shared if the operation is gonna modify "users"'s size or can be caught in a intermediary state such as SendFunds()
    //must grab as unique if the operation is gonna user iterators
    static std::shared_mutex iter_lock;

public:
    static std::string admin_account;

    static size_t NumOfUsers() noexcept;
    static size_t NumOfLogs() noexcept;
    static size_t SumBal() noexcept;

    static BankResponse GetBal(const std::string &name) noexcept;
#if MAX_LOG_SIZE > 0
    static BankResponse GetLogs(const std::string &name) noexcept;
#endif
    static BankResponse SendFunds(const std::string &a_name, const std::string &b_name, uint32_t amount) noexcept;
    static bool VerifyPassword(const std::string &name, const std::string_view &attempt) noexcept;

    static void ChangePassword(const std::string &name, const std::string &new_pass) noexcept;
    static BankResponse SetBal(const std::string &name, uint32_t amount) noexcept;
    static BankResponse ImpactBal(const std::string &name, int64_t amount) noexcept;
    static bool Contains(const std::string &name) noexcept;
#if MAX_LOG_SIZE > 0
    static BankResponse PruneUsers(time_t threshold_time, uint32_t threshold_bal) noexcept;
#else
    static BankResponse PruneUsers(uint32_t threshold_bal) noexcept;
#endif

    static BankResponse AddUser(const std::string &name, uint32_t init_bal, const std::string &init_pass) noexcept;
    static BankResponse DelUser(const std::string &name) noexcept;
    static void DelSelf(const std::string &name) noexcept;

    static const char *Save();
    static void Load();
};