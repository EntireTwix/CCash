#include "xxhash_str.h"

uint64_t xxHashStringGen::operator()(const std::string &str) const noexcept
{
    return XXH3_64bits(str.data(), str.size());
}
