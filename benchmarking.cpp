#include <iostream>
#include <chrono>
#include <thread>
#include <sys/types.h>
#include <unistd.h>
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
    bank.AddUser("twix", "root");
    bank.AddUser("jolly", "root");
    bank.admin_pass = "root";
    Op_a(bank.AddUser("", ""), "add user: ", 1000000, bank.DelUser("", ""));
    Op_a(bank.AdminAddUser("root", "", 0, ""), "admin add user: ", 1000000, bank.DelUser("", ""));
    Op(bank.SetBal("twix", "root", 1000000), "set bal: ", 1000000);
    Op(bank.SendFunds("twix", "jolly", 1), "send funds: ", 1000000);

    bank.AddUser("", "");
    Op_a(bank.DelUser("", ""), "del user: ", 1000000, bank.AddUser("", ""));
    Op_a(bank.AdminDelUser("", "root"), "admin del user: ", 1000000, bank.AddUser("", ""));
    bank.DelUser("", "");

    Op(bank.Contains("twix"), "contains: ", 1000000);
    Op(bank.AdminVerifyPass("root"), "admin verify pass: ", 1000000);
    Op(bank.GetBal("twix"), "get bal: ", 1000000);
    Op(bank.VerifyPassword("twix", "root"), "verify pass: ", 1000000);
    Op(bank.ChangePassword("twix", "root"), "change pass: ", 1000000);
    Op(bank.GetLogs("twix"), "get logs: ", 10000);
    Op(bank.GetChangeState(), "change flag: ", 10000);
    Op(bank.Save(), "saving: ", 1);

    return 0;
}
