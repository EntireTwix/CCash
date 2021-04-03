#include <iostream>
#include <chrono>
#include <thread>
#include <sys/types.h>
#include <unistd.h>
#include "bank.hpp"

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        std::cerr << "Usage: sudo ./bank <admin password> <saving frequency in minutes> <threads>\n";
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
    Bank.admin_pass = argv[1];

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