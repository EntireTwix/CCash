#include <iostream>
#include <chrono>
#include <thread>
#include "bank.hpp"

int main()
{
    std::thread([&]() {
        for (int i = 0; i < 10000; ++i)
        {
            Bank.AddUser(std::to_string(i), "root");
        }
    }).detach();
    std::this_thread::sleep_for(std::chrono::nanoseconds(10));
    Bank.Save();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return 0;
}