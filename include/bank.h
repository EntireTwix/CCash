#pragma once
#include <iostream> //temporary
#include <fstream>
#include <shared_mutex>
#include <drogon/HttpTypes.h>
#include <parallel-hashmap/parallel_hashmap/phmap.h>
#include "error_responses.hpp" //temporary
#include "user.h"

#if CONSERVATIVE_DISK_SAVE && MAX_LOG_SIZE < 0
#include "change_flag.h"
#endif

using BankResponse = std::pair<drogon::HttpStatusCode, Json::Value>;

class Bank
{
private:
    phmap::parallel_flat_hash_map<
        std::string, User,
        phmap::priv::hash_default_hash<std::string>,
        phmap::priv::hash_default_eq<std::string>,
        phmap::priv::Allocator<phmap::priv::Pair<const std::string, User>>,
        4UL,
        std::mutex>
        users;

#if CONSERVATIVE_DISK_SAVE
    ChangeFlag save_flag;
#endif

    /**
     * @brief size_l should be grabbed if the operation MODIFIES the size (shared), this is so that when save claims unique
     * 
     */
    std::shared_mutex size_l;

    /**
     * @brief send_funds_l should be grabbed if balances are being MODIFIED (shared) or if an operation needs to READ without the intermediary states that sendfunds has (unique)
     * 
     */
    std::shared_mutex send_funds_l;

public:
    std::string admin_pass;

    bool GetChangeState() const noexcept;

    BankResponse GetBal(const std::string &name) const noexcept;
    BankResponse GetLogs(const std::string &name) noexcept;
    BankResponse SendFunds(const std::string &a_name, const std::string &b_name, uint32_t amount) noexcept;
    bool VerifyPassword(const std::string &name, const std::string &attempt) const noexcept;

    void ChangePassword(const std::string &name, std::string &&new_pass) noexcept;
    BankResponse SetBal(const std::string &name, uint32_t amount) noexcept;

    int_fast8_t AddUser(const std::string &name, const std::string &init_pass) noexcept;
    int_fast8_t AdminAddUser(const std::string &attempt, std::string &&name, uint32_t init_bal, std::string &&init_pass) noexcept;

    int_fast8_t DelUser(const std::string &name, const std::string &attempt) noexcept;
    int_fast8_t AdminDelUser(const std::string &name, const std::string &attempt) noexcept;

    bool Contains(const std::string &name) const noexcept;
    bool AdminVerifyPass(const std::string &attempt) noexcept;

    void Save();
    void Load();
};