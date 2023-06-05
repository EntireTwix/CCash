[PREVIOUS PAGE](user_side.md) | [NEXT PAGE](../building.md)

# Implementation Features
## [Parallel Hashmap](https://github.com/greg7mdp/parallel-hashmap)
<!-- memory vs database -->
<!-- and while changes arent made on the basis of speed alone it does seem to fit the problem better as we only need to save every `n` minutes/on close. -->
<!-- phmap vs std hash map -->
#### STD vs phmap
this parallel hashmap implementation is the basis of CCash, its where all the user data is stored, compared to the STD's `std::unordered_map<T>` its much faster, this, multi threading support, and more can be found in the [writeup](https://greg7mdp.github.io/parallel-hashmap/).
![image](https://raw.githubusercontent.com/greg7mdp/parallel-hashmap/master/html/img/stl_flat_both.PNG)
![image](https://raw.githubusercontent.com/greg7mdp/parallel-hashmap/master/html/img/lock_various_sizes.PNG)
## [xxHash](https://github.com/Cyan4973/xxHash)
xxhash is used for both hashing of passwords for storage aswell as the usernames for indexing the phmap, its speed is ridiculous at faster then `memcpy` rates of Gb/s.
| Hash Name             | Width | Bandwidth (GB/s) |
| --------------------- | ----- | ---------------- |
| __XXH3__ (SSE2)       | 64    | 31.5 GB/s        |
| _RAM sequential read_ | N/A   | 28.0 GB/s        |
## [Base64](https://github.com/aklomp/base64)
base64 decoding is required for Basic Auth so I used this clean and fast solution which uses SIMD.
![image](https://github.com/aklomp/base64/blob/master/base64-benchmarks.png)
## [Simdjson](https://github.com/simdjson/simdjson)
simdjson was the fastest JSON parsing I could find, its used for request parsing.
![image](https://github.com/simdjson/simdjson/blob/master/doc/rome.png)
## [Drogon webframework](https://github.com/an-tao/drogon)
at the time of making this doc Drogon is the 3rd fastest web framework as per [this](https://www.techempower.com/benchmarks/#section=data-r20&hw=ph&test=composite) sites metric of measuring web frameworks, it also has multi threading support.

![image](https://user-images.githubusercontent.com/31377881/125891266-570c5154-8ae2-4358-9d7b-ccd82a18b132.png)
## Sparse saving
#### Saving on close
when the program is interupted with CONTROL + C it will save before closing the webserver, **it will not however save during a crash**.
#### Auto Saving
every `n` minutes, a configurable amount at launch, CCash will save.
#### Changes
for the above two cases, it will only save to disk if changes have been made since last save.
#### [Binary Encoding](https://github.com/chronoxor/FastBinaryEncoding)
saving is done using FBE, this slightly reduces file size compared to JSON and is much faster.
|                               Protocol                                | Message size | Serialization time | Deserialization time |
| :-------------------------------------------------------------------: | -----------: | -----------------: | -------------------: |
|                 [Cap'n'Proto](https://capnproto.org)                  |    208 bytes |             558 ns |               359 ns |
| [FastBinaryEncoding](https://github.com/chronoxor/FastBinaryEncoding) |    234 bytes |              66 ns |                82 ns |
|          [FlatBuffers](https://google.github.io/flatbuffers)          |    280 bytes |             830 ns |               290 ns |
|      [Protobuf](https://developers.google.com/protocol-buffers)       |    120 bytes |             628 ns |               759 ns |
|                     [JSON](http://rapidjson.org)                      |    301 bytes |             740 ns |               500 ns |
## Multi-threading support
considering phmap and drogon both massively benefit from being multi-threaded it seemed obvious that the entire program should be, this is enabled by default and manually settable at `MULTI_THREADED`. 
