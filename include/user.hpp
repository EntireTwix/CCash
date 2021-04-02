#pragma once
#include <string>
#include <mutex>

class User
{
private:
    uint_fast64_t balance;
    std::string password;

public:
    /**
     * @brief User constructor
     * 
     * @param init_pass initial password
     */
    User(std::string &&init_pass) : password(init_pass) {}

    /**
     * @brief User Constructor for admins
     * 
     * @param init_bal initial balance
     * @param init_pass initial password 
     */
    User(uint_fast64_t init_bal, std::string &&init_pass) : balance(init_bal), password(init_pass) {}

    bool ChangePassword(const std::string &attempt, std::string &&new_pass)
    {
        const bool state = (password == attempt);
        if (state)
        {
            password = new_pass;
        }
        return state;
    }

    /**
     * @brief SendFunds allows sending of money between users if verification is provided in the form of a password and if the user has sufficient funds
     * 
     * @param a first user
     * @param b second user
     * @param amount amount being sent
     * @param attempt password of first user
     * @return wether transaction was succesful
     */
    static bool SendFunds(User &a, User &b, uint_fast64_t amount, const std::string &attempt)
    {
        const bool state = (a.password == attempt) && (a.balance >= amount);
        if (state)
        {
            a.balance -= amount;
            b.balance += amount;
        }
        return state;
    }

    /**
     * @brief Get the balance of the User object
     * 
     * @return the balance
     */
    uint_fast64_t GetBal()
    {
        return balance;
    }

    /**
     * @brief Used for Verification of password by external services, this can be used for logging in or signature
     * 
     * @param attempt the password
     * @return wether the passwords match
     */
    bool VerifyPassword(const std::string &attempt) //for connected services
    {
        return (password == attempt);
    }
};