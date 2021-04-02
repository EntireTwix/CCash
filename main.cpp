#include <iostream>
#include "user.hpp"

int main()
{
    User x(1000, "pass123");
    std::cout << x.ChangePassword("pass123", "newpass123");

    return 0;
}