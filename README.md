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

- [Parallel HashMap](https://github.com/greg7mdp/parallel-hashmap/tree/master)
- [drogon web framework (and all its dependencies)](https://github.com/an-tao/drogon/tree/master)

# Features

### Performance

- **NOT** written in Lua
- written in **C++**, arguably the fastest language
- **multi-threaded**
- **parallel hashmaps** a far superior HashMap implementation to the STD, that also benefit from multi-threaded

### Safety

- **Tamper Proof** relative to an in-game implementation
- **Auto-Saving**
- **OpenSSL** (HTTPS)

### Accessibility

- **Web front-end**
- **RESTful** API for connected services like a market, gambling, or anything else you can think of
- able to be used millions of blocks away, across dimensions, servers, **vanilla or modded**. In contrast to an in-game modded implementation that would be range limited.
