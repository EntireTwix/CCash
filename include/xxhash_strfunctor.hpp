#pragma once
#include <type_traits>
#include <string>
#include <xxhash.h>

struct xxHashStringGen
{
    inline uint64_t operator()(const std::string &str) const
    {
        return XXH3_64bits(str.data(), str.size());
    }
};