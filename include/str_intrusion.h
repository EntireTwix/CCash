#pragma once
#include <string>

struct StrFromSV_Wrapper
{
    std::string str;
    StrFromSV_Wrapper() noexcept;
    StrFromSV_Wrapper(std::string_view sv) noexcept;
    ~StrFromSV_Wrapper() noexcept;
};