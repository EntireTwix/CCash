#include <iostream>
#include "user.hpp"

int main()
{
    User a(1000, "pass123");
    User b(0, "pass123");
    a.ChangePassword("pass123", "newpass123");
    SendFunds(a, b, 250, "newpass123");
    a.GetBal();
    a.VerifyPassword("newpass124");

    return 0;
}