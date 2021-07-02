#include "xxhash_str.h"

XXH64_hash_t xxHashStringGen::operator()(const std::string &str) const noexcept
{
    return XXH3_64bits(str.data(), str.size());
}
