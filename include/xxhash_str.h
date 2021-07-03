#pragma once
#include <string>
#include <xxhash.h>

struct xxHashStringGen
{
    XXH64_hash_t operator()(const std::string &str) const noexcept;
    XXH64_hash_t operator()(std::string_view str) const noexcept;
};
