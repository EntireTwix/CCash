#pragma once
#include <iostream> //temporary
#include <fstream>
#include <shared_mutex>
#include <drogon/HttpTypes.h>
#include <parallel-hashmap/parallel_hashmap/phmap.h>
#include "user.h"

#if (CONSERVATIVE_DISK_SAVE && MAX_LOG_SIZE < 0) && !MULTI_THREADED
#include "change_flag.h"
#endif

using BankResponse = std::pair<drogon::HttpStatusCode, Json::Value>;

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
    ChangeFlag save_flag;
#else
    bool save_flag = false;
#endif
#endif

    std::shared_mutex save_lock;

public:
    std::string admin_account;

    size_t NumOfUsers() const noexcept;
    uint64_t NumOfLogs() const noexcept;

#if CONSERVATIVE_DISK_SAVE
    bool GetChangeState() const noexcept;
#endif

    BankResponse GetBal(const std::string &name) const noexcept;
    BankResponse GetLogs(const std::string &name) noexcept;
    BankResponse SendFunds(const std::string &a_name, const std::string &b_name, uint32_t amount) noexcept;
    bool VerifyPassword(const std::string &name, std::string_view &&attempt) const noexcept;

    void ChangePassword(const std::string &name, std::string &&new_pass) noexcept;
    BankResponse SetBal(const std::string &name, uint32_t amount) noexcept;
    BankResponse AddBal(const std::string &name, uint32_t amount) noexcept;
    BankResponse SubBal(const std::string &name, uint32_t amount) noexcept;
    bool Contains(const std::string &name) const noexcept;
    bool AdminVerifyAccount(const std::string &name) noexcept;

    BankResponse AddUser(std::string &&name, uint32_t init_bal, std::string &&init_pass) noexcept;
    BankResponse DelUser(const std::string &name) noexcept;

    void Save();
    void Load();
};