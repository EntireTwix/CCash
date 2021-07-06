#include <iostream>
#include <chrono>
#include <thread>
#include <sys/types.h>
#include <unistd.h>
#include <random>
#include "bank.h"

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

using namespace std::chrono;

static Bank bank;

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

    bank.AddUser("twix", 0, "root");
    bank.AddUser("jolly", 0, "root");
    bank.admin_account = "twix";

    Op(std::hash<std::string>{}("twix is pog champ :flushed:"), "std::hash<std::string> ", 100000);
    Op(xxHashStringGen{}("twix is pog champ :flushed:", 27), "xxHashStringGen ", 100000);

    Op_a(bank.AddUser("", 0, ""), "add user: ", 100000, bank.DelUser(""));
    Op(bank.ImpactBal("twix", 1), "impact bal: ", 100000);
    Op(bank.SetBal("twix", 100000), "set bal: ", 100000);
    Op(bank.SendFunds("twix", "jolly", 1), "send funds: ", 100000);
    Op(bank.SendFunds("twix", "twix", 1), "invalid send funds: ", 100000);

    bank.AddUser("", 0, "");
    Op_a(bank.DelUser(""), "del user: ", 100000, bank.AddUser("", 0, ""));
    bank.DelUser("");

    Op(bank.Contains("twix"), "contains: ", 100000);
    Op(bank.AdminVerifyAccount("twix"), "admin verify pass: ", 100000);
    Op(bank.GetBal("twix"), "get bal: ", 100000);
    Op(bank.VerifyPassword("twix", "root"), "verify pass: ", 100000);
    Op(bank.ChangePassword("twix", "root"), "change pass: ", 100000);
#if MAX_LOG_SIZE > 0
    Op(bank.GetLogs("twix"), "get logs: ", 10000);
#endif
    Op(bank.Save(), "saving: ", 1);
#if CONSERVATIVE_DISK_SAVE
    Op(bank.GetChangeState(), "change flag: ", 100000);
#endif

    //GetBal scalining test
    // std::default_random_engine generator;
    // std::uniform_real_distribution<double> distribution(0.0, 1.0);

    // for (size_t i = 0; i < 10000000; ++i)
    // {
    //     bank.AddUser(std::to_string(i), 100000, "root");
    //     if (i % 10000 == 0)
    //     {
    //         auto u = std::to_string((int)(distribution(generator) * i));
    //         Op(bank.GetBal(u), std::to_string(i) + ", ", 100000);
    //     }
    // }

    return 0;
}