#include <iostream>
#include <chrono>
#include <thread>
#include <sys/types.h>
#include <unistd.h>
#include <random>
#include "xxhash_str.h"
#include "bank.h"

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

using namespace std::chrono;

#include <ctime>
#include <ratio>
#include <chrono>

#define time_func_a(f, a, x)                                                                  \
    {                                                                                         \
        using namespace std::chrono;                                                          \
        uint32_t timer = 0;                                                                   \
        for (int i = 0; i < x; ++i)                                                           \
        {                                                                                     \
            auto t1 = high_resolution_clock::now().time_since_epoch();                        \
            f;                                                                                \
            auto t2 = high_resolution_clock::now().time_since_epoch();                        \
            a;                                                                                \
            timer += std::chrono::duration_cast<std::chrono::nanoseconds>((t2 - t1)).count(); \
        }                                                                                     \
        std::cout << timer / x << '\n';                                                       \
    }

#define time_func(f, x)                                                                       \
    {                                                                                         \
        using namespace std::chrono;                                                          \
        uint32_t timer = 0;                                                                   \
        for (int i = 0; i < x; ++i)                                                           \
        {                                                                                     \
            auto t1 = high_resolution_clock::now().time_since_epoch();                        \
            f;                                                                                \
            auto t2 = high_resolution_clock::now().time_since_epoch();                        \
            timer += std::chrono::duration_cast<std::chrono::nanoseconds>((t2 - t1)).count(); \
        }                                                                                     \
        std::cout << timer / x << '\n';                                                       \
    }

#define Op_a(v, name, x, a)   \
    {                         \
        std::cout << name;    \
        time_func_a(v, a, x); \
    }

#define Op(v, name, x)     \
    {                      \
        std::cout << name; \
        time_func(v, x);   \
    }

int main(int argc, char **argv)
{
    for (size_t i = 0; i < 10000; ++i)
    {
        Bank::AddUser(std::to_string(i), 0, "root");
    }
    Bank::AddUser("twix", 0, "root");
    Bank::AddUser("jolly", 0, "root");
    Bank::admin_account = "twix";

    const std::string data("this string is quite long which is relevant when testing the speed of a hasing function");
    Op(std::hash<std::string>{}(data), "hash<string>: ", 1000000);
    Op(xxHashStringGen{}(data), "xxHashStringGen: ", 1000000);

    Op_a(Bank::AddUser("abc", 0, "abc"), "add user: ", 1000000, Bank::DelUser("abc"));
    Op(Bank::ImpactBal("twix", 1), "impact bal: ", 1000000);
    Op(Bank::SetBal("twix", 1000000), "set bal: ", 1000000);
    Op(Bank::SendFunds("twix", "jolly", 1), "send funds: ", 1000000);
    Op(Bank::SendFunds("", "", 1), "invalid send funds: ", 1000000);

    Bank::AddUser("abc", 0, "abc");
    Op_a(Bank::DelUser("abc"), "del user: ", 1000000, Bank::AddUser("abc", 0, "abc"));
    Op_a(Bank::DelSelf("abc"), "del self: ", 1000000, Bank::AddUser("abc", 0, "abc"));
    Bank::DelUser("abc");

    Op(Bank::Contains("twix"), "contains: ", 1000000);
    Op(Bank::GetBal("twix"), "get bal: ", 1000000);
    Op(Bank::VerifyPassword("twix", "root"), "verify pass: ", 1000000);
    Op(Bank::ChangePassword("twix", "root"), "change pass: ", 1000000);
#if MAX_LOG_SIZE > 0
    Op(Bank::GetLogs("twix"), "get logs init: ", 1);
    Op(Bank::GetLogs("twix"), "get logs cached: ", 1000000);
#endif
    Op(Bank::PruneUsers(0, 0), "prune users: ", 1);
    Op(Bank::Save(), "saving: ", 1);

    //GetBal scalining test
    //std::default_random_engine generator;
    //std::uniform_real_distribution<double> distribution(0.0, 1.0);

    // for (size_t i = 0; i < 10000000; ++i)
    // {
    //     Bank::AddUser(std::to_string(i), 100000, "root");
    //     if (i % 10000 == 0)
    //     {
    //         auto u = std::to_string((int)(distribution(generator) * i));
    //         Op(Bank::GetBal(u), std::to_string(i) + ", ", 100000);
    //     }
    // }

    return 0;
}
