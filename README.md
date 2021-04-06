# CCash-2.0 [INCOMPLETE]

A complete simplification and improvement to the CCash project. A webserver hosting a bank system for Minecraft, able to be used from web browser or from CC/OC if you're playing modded.

#### Build

```
git clone --recurse-submodule https://github.com/EntireTwix/CCash-2.0/
mkdir build
cd build
cmake ..
make -j<threads>
sudo ./bank <admin password> <saving frequency in minutes> <threads>
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
- **Auto-Saving**, Saves on crash, Saves on close
- **OpenSSL** (HTTPS)

### Accessibility

- **Web front-end**
- **RESTful** API for connected services like a market, gambling, or anything else you can think of
- able to be used millions of blocks away, across dimensions, servers, **vanilla or modded**. In contrast to an in-game modded implementation that would be range limited.

### Connected Services

Go to `{ip}/BankF/help` to see the bank's methods. Using the Bank's API allows (you/others) can (make/use) connected services that utilize the bank, a couple ideas are

- a Market
- Gambling
- Shipping
- Leaderboard
- High-level bank abstractions such as loans
- Some trust based system similiar to Paypal

`**WARNING** : abruptly killing the program will result in data loss, use Close() method to close safely`
