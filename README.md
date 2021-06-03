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

Go to `{ip}/BankF/help` to see the bank's methods (also found in releases as help.html). Using the Bank's API allows (you/others) to (make/use) connected services that utilize the bank, a couple ideas are

### Implemented:

- [Web Frontend](https://github.com/Expand-sys/ccashfrontend)
![image](https://user-images.githubusercontent.com/31377881/116965729-4ab44500-ac63-11eb-9f11-dc04be6b3d63.png)
- [CC Frontend](https://github.com/Reactified/rpm/blob/main/packages/ccash-wallet)
![image](https://user-images.githubusercontent.com/31377881/116967157-8b618d80-ac66-11eb-8f2e-4a6297ef0b16.png)
- [CC Shop](https://github.com/Reactified/rpm/tree/main/packages/ccash-shop)
![image](https://user-images.githubusercontent.com/31377881/120050327-de163700-bfd1-11eb-9d5a-f75c003e867c.png)
![image](https://user-images.githubusercontent.com/31377881/120050367-09992180-bfd2-11eb-9a22-449d73c196cf.png)
- [CC API](https://github.com/Reactified/rpm/blob/main/packages/ccash-api/api.lua)
- [CC ATM](https://github.com/Reactified/misc/tree/main/lua/ccash-bank) an ATM for economies allowing for an initial exchange to start up

### In-Dev:

- [a Market](https://github.com/STBoyden/market-api-2.0)
- [Ebay equivelant](https://github.com/EntireTwix/CSHBay)

### Ideas:

- Gambling
- Shipping
- High-level bank operations such as loans
- Some trust based system for transactions similiar to Paypal

`**WARNING** : abruptly killing the program will result in data loss, use Close() method to close safely`

## Features

### Performance

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
- able to be used millions of blocks away, across dimensions, servers, **vanilla or modded**. In contrast to an in-game modded implementation that would be range limited.
- **Logging** of all transactions, configurable in [log_consts.hpp](include/log_consts.hpp)

## Dependencies

- [Parallel HashMap](https://github.com/greg7mdp/parallel-hashmap/tree/master)
- [Drogon](https://github.com/an-tao/drogon/tree/master)
- [XXHASH](https://github.com/Cyan4973/xxHash)
