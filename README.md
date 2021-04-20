# CCash

A webserver hosting a bank system for Minecraft, able to be used from web browser or from CC/OC if you're playing modded.

### Description

the currency model most Minecraft Servers adopt if any, is resource based, usually diamonds, this model is fraught with issues however:

- the primary issue is minecraft worlds are infinite leading to hyper inflation as everyone accrues more diamonds
- there is no central authority minting the currency, any consumer can introduce more diamonds to the system
- some resources are passively reapable, making the generation of currency a larger focus then of products
- locality is required for transaction
- theft is possible, ownership is possession based

CCash solves these issues and adds a level of abstraction, the main philosophy of CCash is to have fast core operations that other services build on

#### Demo

(BACKEND)
https://ccash.ryzerth.com/BankF/help

(FRONTEND)
https://ccashfrontend.herokuapp.com

#### Build

```
git clone --recurse-submodule https://github.com/EntireTwix/CCash/
mkdir build
cd build
cmake ..
make -j<threads>
sudo ./bank <admin password> <saving frequency in minutes> <threads>
```

**you also have to edit the config file to add your cert locations**, I personally use cert bot

### Connected Services

Go to `{ip}/BankF/help` to see the bank's methods. Using the Bank's API allows (you/others) to (make/use) connected services that utilize the bank, a couple ideas are

Implemented:

- [Web Frontend](https://github.com/Expand-sys/ccashfrontend)
- [CC API](https://github.com/Reactified/rpm/blob/main/packages/ccash-api/api.lua)
- [CC Frontend](https://github.com/Reactified/rpm/blob/main/packages/ccash-wallet/wallet.lua)

In-Dev:

- [a Market](https://github.com/STBoyden/market-api-2.0)

Ideas:

- Gambling
- Shipping
- High-level bank operations such as loans
- Some trust based system for transactions similiar to Paypal

`**WARNING** : abruptly killing the program will result in data loss, use Close() method to close safely`

# Features

### Performance

- **NOT** written in Lua
- written in **C++**, arguably the fastest language
- **multi-threaded**
- **parallel hashmaps** a far [superior](https://greg7mdp.github.io/parallel-hashmap/) HashMap implementation to the STD, that also benefit from multi-threaded
- **Passwords are Hashed**, allowing each user to be 12 bytes, which is allows hashmap to be flat ([XXH3](https://github.com/Cyan4973/xxHash/blob/dev/README.md))
- **Drogon** is a very fast [web framework](https://github.com/the-benchmarker/web-frameworks)

### Safety

- **Tamper Proof** relative to an in-game implementation
- **Auto-Saving** and Saves on close
- **HTTPS** (OpenSSL)

### Accessibility

- **Web front-end**
- **RESTful** API for connected services like a market, gambling, or anything else you can think of
- able to be used millions of blocks away, across dimensions, servers, **vanilla or modded**. In contrast to an in-game modded implementation that would be range limited.

#### Dependencies

- [Parallel HashMap](https://github.com/greg7mdp/parallel-hashmap/tree/master)
- [Drogon](https://github.com/an-tao/drogon/tree/master)
- [XXHASH](https://github.com/Cyan4973/xxHash)
