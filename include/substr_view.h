#pragma once
#include <cassert>
#include <string>

class substr_view
{
private:
    const char *begin;
    size_t end;

public:
    substr_view(std::string_view str, size_t begin_init = 0, size_t end_init = 0) noexcept;
    bool operator==(const std::string &str) const noexcept;
    const char *data() const noexcept;
    std::string_view str_view() const noexcept;
};