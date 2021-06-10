# CCash

A webserver hosting a bank system for Minecraft, able to be used from web browser or from CC/OC if you're playing modded.

the currency model most Minecraft Servers adopt if any, is resource based, usually diamonds, this model is fraught with issues however:

- the primary issue is minecraft worlds are infinite leading to hyper inflation as everyone accrues more diamonds
- there is no central authority minting the currency, any consumer can introduce more diamonds to the system
- some resources are passively reapable, making the generation of currency a larger focus then of products
- locality is required for transaction
- theft is possible, ownership is possession based

CCash solves these issues and adds a level of abstraction, the main philosophy of CCash is to have fast core operations that other services build on

## Build

drogon depedencies

```
sudo apt install libjsoncpp-dev
sudo apt install uuid-dev
sudo apt install openssl
sudo apt install libssl-dev
sudo apt install zlib1g-dev
```

building the project

```
git clone --recurse-submodule https://github.com/EntireTwix/CCash/
cd CCash
mkdir build
cd build
cmake ..
make -j<threads>
```

then edit config.json to include the paths to your certs for HTTPS (I use certbot), or just remove the listener for port 443.

```
vim ../config.json
```

finally, run the program

```
sudo ./bank <admin password> <saving frequency in minutes> <threads>
```

## Connected Services


Go to `{ip}/BankF/help` to see the bank's methods (also found in releases as help.html). Using the Bank's API allows (you/others) to (make/use) connected services that utilize the bank, a couple ideas can be found [here](services.md)

## FAQ
**Q:** how is money initially injected into the economy

**A:** you can take any approach you want, one that I recommend is using a one way exchange via the CC ATM above to have players mine the initial currency, this rewards early adopters and has a sunk cost effect in that the resource is promptly burned

## [Contributions](https://github.com/EntireTwix/CCash/graphs/contributors)
Thank you to the contributors

| Name | Work |
| :--- | --- |
| [Expand](https://github.com/Expand-sys) | Frontend |
| [React](https://github.com/Reactified) | CC {API, Shops, and ATM} |
| [Doggo](https://github.com/FearlessDoggo21) | Logs loading/adding Optimized, Python API |


## Features

### Performance
- In memory database instead of on disk
- **NOT** written in Lua, like a OC/CC implementation
- written in **C++**, arguably the fastest language
- **multi-threaded**
- **parallel hashmaps** a far [superior](https://greg7mdp.github.io/parallel-hashmap/) HashMap implementation to the STD, that also benefits from multi-threaded
- **Drogon** is a very fast [web framework](https://www.techempower.com/benchmarks/#section=data-r20&hw=ph&test=composite)
- **Lightweight**, anecodotally I experienced 0.0% idle, <1% CPU usage on average, 7% at peak, 1000 requests in 0.85s

### Safety

- **Tamper Proof** relative to an in-game implementation
- **Auto-Saving** and Saves on close
- **HTTPS** (OpenSSL)

### Accessibility

- **RESTful** API for connected services like a market, gambling, or anything else you can think of
- able to be used millions of blocks away, across dimensions, servers, **vanilla or modded**.
- **Logging** of all transactions, configurable in [log_consts.hpp](include/log_consts.hpp)

## Dependencies

- [Parallel HashMap](https://github.com/greg7mdp/parallel-hashmap/tree/master)
- [Drogon](https://github.com/an-tao/drogon/tree/master)
- [XXHASH](https://github.com/Cyan4973/xxHash)
