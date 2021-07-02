#pragma once
#include <string>
#include <xxhash.h>

struct xxHashStringGen
{
    uint64_t operator()(const std::string &str) const noexcept;
};
