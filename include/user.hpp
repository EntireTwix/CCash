#pragma once
#include <string>
#include <mutex>

class User
{
private:
    uint_fast64_t balance;
    std::string password;

    std::mutex bal_lock;
    std::mutex pass_lock;

public:
    User(uint_fast64_t init_bal, std::string &&init_pass) : balance(init_bal), password(init_pass) {}

    bool ChangePassword(const std::string &attempt, std::string &&new_pass)
    {
        std::lock_guard<std::mutex> lock{pass_lock};
        const bool state = (password == attempt);
        if (state)
        {
            password = new_pass;
        }
        return state;
    }

    friend bool SendFunds(User &a, User &b, uint_fast64_t amount, const std::string &attempt)
    {
        bool state;
        {
            std::lock_guard<std::mutex> lock{a.pass_lock};
            state = (a.password == attempt);
        }

        std::scoped_lock<std::mutex, std::mutex> lock{a.bal_lock, b.bal_lock};
        state = state && (a.balance >= amount);
        if (state)
        {
            a.balance -= amount;
            b.balance += amount;
        }
        return state;
    }

    uint_fast64_t GetBal()
    {
        std::lock_guard<std::mutex> lock{bal_lock};
        return balance;
    }

    bool VerifyPassword(const std::string &attempt) //for connected services
    {
        std::lock_guard<std::mutex> lock{pass_lock};
        return (password == attempt);
    }
};