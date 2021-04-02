#include <iostream>
#include "bank.hpp"

int main()
{
    Bank.admin_pass = "root";
    std::cout << Bank.AddUser("Twix", "pass123") << '\n';
    std::cout << Bank.GetBal("Twixy") << '\n';
    std::cout << Bank.AdminAddUser("root", "Jollymonsam", 2500, "pass123") << '\n';
    std::cout << Bank.GetBal("Jollymonsam") << '\n';
    std::cout << Bank.SendFunds("Jollymonsam", "Twix", 333, "pass123") << '\n';
    std::cout << Bank.GetBal("Twix") << " | " << Bank.GetBal("Jollymonsam") << '\n';

    return 0;
}