[PREVIOUS PAGE](user_side.md) | [NEXT PAGE](../building.md)

# Implementation Features
## [Parallel Hashmap](https://github.com/greg7mdp/parallel-hashmap)
<!-- memory vs database -->
<!-- and while changes arent made on the basis of speed alone it does seem to fit the problem better as we only need to save every `n` minutes/on close. -->
<!-- phmap vs std hash map -->
#### STD vs phmap
this parallel hashmap implementation is the basis of CCash, its where all the user data is stored, compared to the STD's `std::unordered_map<T>` its much faster, this, multi threading support, and more can be found in the [writeup](https://greg7mdp.github.io/parallel-hashmap/).
#### Scalability
below is `GetBal()` being called where `x` axis grows with # of users reaching 10 million users, `y` axis is time in ns. The name given is random between 0 and max users at that time as to provide more accurate results.
![image](GetBal().png)
as the graph demonstrates, regardless of size `GetBal()` remains consistent at ~39ns on my 3700x single threaded.
## [xxHash](https://github.com/Cyan4973/xxHash)
xxhash is used for both hashing of passwords for storage aswell as the usernames for indexing the phmap, its speed is ridiculous at faster then `memcpy` rates of Gb/s.
## [Base64](https://github.com/aklomp/base64)
base64 decoding is required for Basic Auth so I used this clean and fast solution found [here](https://github.com/aklomp/base64) which uses SIMD if available.
## [Simdjson](https://github.com/simdjson/simdjson)
simdjson was the fastest JSON parsing I could find, its used for request parsing.
## [Drogon webframework](https://github.com/an-tao/drogon)
at the time of making this doc Drogon is the 3rd fastest web framework as per [this](https://www.techempower.com/benchmarks/#section=data-r20&hw=ph&test=composite) sites metric of measuring web frameworks, it also has multi threading support.
## Sparse saving
#### Saving on close
when the program is interupted with CONTROL + C it will save before closing the webserver, **it will not however save during a crash**.
#### Auto Saving
every `n` minutes, a configurable amount at launch, CCash will save.
#### Changes
for the above two cases, it will only save to disk if changes have been made since last save.
## Multi-threading support
considering phmap and drogon both massively benefit from being multi-threaded it seemed obvious that the entire program should be, this is enabled by default and manually settable at `MULTI_THREADED`. Below are some graphs visualizing the gain of doing so:
<!-- graph -->
## Backwards Compatible API
versioning is implemented by the endpoints path, for example `/v1`. Breaking changes will ideally be sparse and backwards compatability will be maintained, for example ideally API `v3` instance can still run `v1` endpoints.