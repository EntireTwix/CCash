#include <iostream>
#include "user.hpp"

int main()
{
    User a(1000, "pass123");
    User b(0, "pass123");
    std::cout << a.ChangePassword("pass123", "newpass123") << '\n';
    std::cout << SendFunds(a, b, 250, "newpass123") << '\n';
    a.GetBal();
    a.VerifyPassword("newpass124");
    std::cout << a.GetBal() << '\n';

    return 0;
}