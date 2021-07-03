#pragma once
#include <string>
#include <xxhash.h>

struct xxHashStringGen
{
    XXH64_hash_t operator()(const std::string_view &str) const noexcept;
};
