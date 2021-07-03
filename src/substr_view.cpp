#include "substr_view.h"

substr_view::substr_view(std::string_view str, size_t begin_init, size_t end_init) noexcept
{
    begin = str.begin() + begin_init;
    if (!end_init)
    {
        end = str.size() - begin_init;
    }
    else
    {
        end = end_init - begin_init;
    }
}
bool substr_view::operator==(const std::string &str) const noexcept
{
    if (str.size() != end)
    {
        return false;
    }
    for (size_t i = 0; i < end; ++i)
    {
        if (*(begin + end) == str[i])
        {
            return false;
        }
    }
    return true;
}
const char *substr_view::data() const noexcept { return begin; }
std::string_view substr_view::str_view() const noexcept { return std::string_view(data(), end); }