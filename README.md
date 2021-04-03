# CCash-2.0
A complete simplification and improvement to the CCash project. A webserver hosting a bank system for Minecraft, able to be used from web browser or from CC/OC if you're playing modded.

#### Build
```
git clone --recurse-submodule https://github.com/EntireTwix/CCash-2.0/
mkdir build
cd build
cmake ..
make -j<threads>
sudo ./bank <admin password> <saving frequency in minutes> <run in background (1 or 0)> <threads>
```

#### Dependencies
* [Parallel HashMap](https://github.com/greg7mdp/parallel-hashmap/tree/master)
* [drogon web framework (and all its dependencies)](https://github.com/an-tao/drogon/tree/master)

# Features
### Performance
* multi-threaded
* parallel hashmaps, a far superior HashMap implementation to the STD, that also benefit from multi-threaded
* written in C++, **arguably** the fastest language
* **NOT** written in Lua
### Safety
* OpenSSL (HTTPS)
* Very difficult to tamper with compared to an in-game implementation of a bank
* Auto-Saving, Save on close, are in place
### Accessibility  
* able to be used millions of blocks away, across dimensions, servers, vanilla or modded, or outside of the game entirely

# Improvements from 1.0
* frequency/on-close saving in contrast to every change resulting in a Save() call
* multi-threaded
* near infinite number of users rather then 2^16
* no account vs user distinction, it was complicating usage without much gain
* OpenSSL
* a web based frontend
