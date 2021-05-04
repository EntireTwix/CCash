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

### Build
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

### Connected Services

Go to `{ip}/BankF/help` to see the bank's methods. Using the Bank's API allows (you/others) to (make/use) connected services that utilize the bank, a couple ideas are

#### Implemented:

- [Web Frontend](https://github.com/Expand-sys/ccashfrontend)
![image](https://user-images.githubusercontent.com/31377881/116965729-4ab44500-ac63-11eb-9f11-dc04be6b3d63.png)
- [CC API](https://github.com/Reactified/rpm/blob/main/packages/ccash-api/api.lua)
- [CC Frontend](https://github.com/Reactified/rpm/blob/main/packages/ccash-wallet/wallet.lua)

![image](https://user-images.githubusercontent.com/31377881/116967157-8b618d80-ac66-11eb-8f2e-4a6297ef0b16.png)

#### In-Dev:

- [a Market](https://github.com/STBoyden/market-api-2.0)

#### Ideas:

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
- **parallel hashmaps** a far [superior](https://greg7mdp.github.io/parallel-hashmap/) HashMap implementation to the STD, that also benefits from multi-threaded
- **Drogon** is a very fast [web framework](https://github.com/the-benchmarker/web-frameworks)

### Safety

- **Tamper Proof** relative to an in-game implementation
- **Auto-Saving** and Saves on close
- **HTTPS** (OpenSSL)

### Accessibility

- **RESTful** API for connected services like a market, gambling, or anything else you can think of
- able to be used millions of blocks away, across dimensions, servers, **vanilla or modded**. In contrast to an in-game modded implementation that would be range limited.

#### Dependencies

- [Parallel HashMap](https://github.com/greg7mdp/parallel-hashmap/tree/master)
- [Drogon](https://github.com/an-tao/drogon/tree/master)
- [XXHASH](https://github.com/Cyan4973/xxHash)
