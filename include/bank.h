#pragma once
#include <iostream> //temporary
#include <fstream>
#include <shared_mutex>
#include <atomic>
#include "error_responses.hpp"
#include "parallel-hashmap/parallel_hashmap/phmap.h"
#include "user.h"

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
    std::atomic<bool> change_flag = false; //if true changes have been made

    void ChangesMade() noexcept;  //called after making changes
    void ChangesSaved() noexcept; //called after saving
#elif
#define ChangesMade() ;
#define ChangesSaved() ;
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

    int_fast8_t AddUser(const std::string &name, const std::string &init_pass) noexcept;
    int_fast8_t AdminAddUser(const std::string &attempt, std::string &&name, uint32_t init_bal, std::string &&init_pass) noexcept;

    int_fast8_t DelUser(const std::string &name, const std::string &attempt) noexcept;
    int_fast8_t AdminDelUser(const std::string &name, const std::string &attempt) noexcept;

    int_fast8_t SendFunds(const std::string &a_name, const std::string &b_name, uint32_t amount, const std::string &attempt) noexcept;

    int_fast8_t Contains(const std::string &name) const noexcept;
    int_fast8_t AdminVerifyPass(const std::string &attempt) noexcept;

    int_fast8_t SetBal(const std::string &name, const std::string &attempt, uint32_t amount) noexcept;
    int_fast64_t GetBal(const std::string &name) const noexcept;

    int_fast8_t VerifyPassword(const std::string &name, const std::string &attempt) const noexcept;
    int_fast8_t ChangePassword(const std::string &name, const std::string &attempt, std::string &&new_pass) noexcept;

    Json::Value GetLogs(const std::string &name, const std::string &attempt) noexcept;

    void Save();

    //NOT THREAD SAFE
    void Load();
};

//TODO make branchless