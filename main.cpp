#include <iostream>
#include <chrono>
#include <thread>
#include <sys/types.h>
#include <unistd.h>
#include "bank.hpp"

int main(int argc, char **argv)
{
    if (argc != 5)
    {
        std::cerr << "Usage: sudo ./bank <admin password> <saving frequency in minutes> <run in background (1 or 0)> <threads>\n";
        return 0;
    }
    if ((argv[3][0] != '0' && argv[3][0] != '1'))
    {
        std::cerr << "run in background state must be 1 or 0\n";
        return 0;
    }
    if (geteuid() != 0)
    {
        std::cerr << "ERROR: CCash MUST be ran as root\n";
        return 0;
    }

    //Loading users from users.json
    Bank.Load();

    //Admin Password
    Bank.admin_pass = argv[2];

    //Auto Saving
    volatile bool saving_flag = true;
    std::thread([&argv, &saving_flag]() {
        while (saving_flag)
        {
            std::this_thread::sleep_for(std::chrono::minutes(std::stoi(argv[2])));
            Bank.Save();
        }
    }).detach();

    return 0;
}