#pragma once
#include <iostream> //temporary
#include <fstream>
#include <shared_mutex>
#include <atomic>
#include <drogon/HttpTypes.h>
#include "error_responses.hpp" //temporary
#include "parallel-hashmap/parallel_hashmap/phmap.h"
#include "user.h"

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

    std::atomic<bool> change_flag = false; //if true changes have been made

private:
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

#if CONSERVATIVE_DISK_SAVE
    void ChangesMade() noexcept;  //called after making changes
    void ChangesSaved() noexcept; //called after saving
#endif

public:
    std::string admin_pass;

#if CONSERVATIVE_DISK_SAVE
    bool GetChangeState() noexcept;
#endif

    BankResponse GetBal(const std::string &name) const noexcept;
    BankResponse GetLogs(const std::string &name) noexcept;
    BankResponse SendFunds(const std::string &a_name, const std::string &b_name, uint32_t amount) noexcept;
    bool VerifyPassword(const std::string &name, const std::string &attempt) const noexcept; //internally used

    void ChangePassword(const std::string &name, std::string &&new_pass) noexcept;

    int_fast8_t AddUser(const std::string &name, const std::string &init_pass) noexcept;
    int_fast8_t AdminAddUser(const std::string &attempt, std::string &&name, uint32_t init_bal, std::string &&init_pass) noexcept;

    int_fast8_t DelUser(const std::string &name, const std::string &attempt) noexcept;
    int_fast8_t AdminDelUser(const std::string &name, const std::string &attempt) noexcept;

    bool Contains(const std::string &name) const noexcept;
    bool AdminVerifyPass(const std::string &attempt) noexcept; //interall used

    BankResponse SetBal(const std::string &name, uint32_t amount) noexcept;

    void Save();
    void Load();
};