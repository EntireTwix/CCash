#include <iostream>
#include <chrono>
#include <thread>
#include "bank.hpp"

int main()
{
    Bank.Load();
    Bank.SendFunds("0", "1", 50, "root");
    Bank.Save();
    return 0;
}