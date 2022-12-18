#pragma once
#include <string>

/*
The intended use of this cursed class is in violating the encapsulation of std::string this class acts like std::string_view even though its stored in a std::string.
The reason this was needed is because sometimes we have a std::string_view instance and another library requires a const std::string& argument, forcing us to copy to a string before passing it, this copying is unnecessary.
*/

struct StrFromSV_Wrapper
{
    std::string str;
    StrFromSV_Wrapper() noexcept;
    StrFromSV_Wrapper(std::string_view sv) noexcept;
    ~StrFromSV_Wrapper() noexcept;
};