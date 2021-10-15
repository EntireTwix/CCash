# Building
[PREVIOUS PAGE](features/implementation.md) | [NEXT PAGE](FAQ.md)

## Advice
as CCash is very lightweight it can run on practically any device but here are some tips:
* single core machines should toggle `MULTI_THREADED` to `false`
* if your server is sufficiently active as so that each save frequency saving is highly likely then `CONSERVATIVE_DISK_SAVE` should be toggled to `false`
* `MAX_LOG_SIZE` should be adjusted as it takes up the most memory usage/storage of the ledger's features at ~203 bytes in memory and ~104 bytes in disk at default settings, so 5165 logs per Mb of RAM. Setting to 0 will disable logs
* with no users memory usage is 8.47 Mb
* Saving frequency (a runtime argument) being set to 0 will disable frequency saving and only save on close

## Docker
If you want to use the docker package, deploy information can be found [here](deploy.md)

## Drogon Depedencies 

### Linux
#### Debian
```
sudo apt install libjsoncpp-dev uuid-dev openssl libssl-dev zlib1g-dev make cmake
```
#### CentOS 7.5
```
yum install git gcc gcc-c++ 
git clone https://github.com/Kitware/CMake
cd CMake/
./bootstrap
make
make install
yum install centos-release-scl devtoolset-8
scl enable devtoolset-8 bash
git clone https://github.com/open-source-parsers/jsoncpp
cd jsoncpp/
mkdir build
cd build
cmake ..
make
make install
yum install libuuid-devel openssl-devel zlib-devel
```
#### Other
anything that can download the appropriate dependencies
```
make & cmake
jsoncpp
libuuid
openssl
zlib
```
### MacOS
```
brew install jsoncpp ossp-uuid openssl zlib 
```

## Actually, building
```
git clone --recurse-submodule https://github.com/EntireTwix/CCash/
cd CCash
cd third_party/base64
AVX2_CFLAGS=-mavx2 SSSE3_CFLAGS=-mssse3 SSE41_CFLAGS=-msse4.1 SSE42_CFLAGS=-msse4.2 AVX_CFLAGS=-mavx make lib/libbase64.o
cd ../..
mkdir build
cd build
cp ../config/config.json config.json
```

### CMake Flags
there are multiple flags responsible configuring CCash:
| name                   |     default      | description                                                                                                                                             | pros                             | cons                                                     |
| :--------------------- | :--------------: | ------------------------------------------------------------------------------------------------------------------------------------------------------- | -------------------------------- | -------------------------------------------------------- |
| USER_SAVE_LOC          | "users.dat"  | where the users are saved                                                                                                                               | `N/A`                            | `N/A`                                                    |
| DROGON_CONFIG_LOC      | "config.json" | where the config is located                                                                                                                             | `N/A`                            | `N/A`                                                    |
| MAX_LOG_SIZE           |       100        | max number of logs per user, last `n` transactions. If both this and pre log are toggled to 0 logs will not be compiled.                                | large history                    | higher memory usage                                      |
| CONSERVATIVE_DISK_SAVE |      `true`      | when `true` only saves when changes are made                                                                                                            | low # of disk operations         | some atomic overhead                                     |
| MULTI_THREADED         |      `true`      | when `true` the program is compiled to utilize `n` threads which corresponds to how many Cores your CPU has, plus 1 for saving                          | speed                            | memory lock overhead is wasteful on single core machines |
| RETURN_ON_DEL_NAME     |      `N/A`       | when defined, return on delete will be toggled and any accounts deleted will send their funds to the defined account, this prevent currency destruction | prevents destruction of currency | deleting accounts is made slower                         |


EXAMPLE:
```
cmake ..
```
sets these flags to their defaults, an example of setting a flag would be 
```
cmake -DMULTI_THREADING=false ..
```
with `-D`

### Finishing building
lastly type in
```
cmake <flags of your choice or none> ..
make -j<threads>
./bank
```

## Certs
make sure to edit `config.json` adding the certificate location if you're using HTTPS, I personally use [certbot](https://certbot.eff.org/). 
```json
{
  "listeners": [
      {
        "address": "0.0.0.0",
        "port": 80,
        "https": false
      },
      {
        "address": "0.0.0.0",
        "port": 443,
        "https": true,
        "cert": "",
        "key": ""
      }
  ]
}
```
editing
```json
"cert": "pubkey",
"key": "privkey"
```

Alternatively you can delete this entire section (Disabling HTTPS in the proccess)
```json
{
    "address": "0.0.0.0",
    "port": 443,
    "https": true,
    "cert": "",
    "key": ""
}
```
leaving
```json
{
  "listeners": [
      {
        "address": "0.0.0.0",
        "port": 80,
        "https": false
      }
  ]
}
```
**it is Highly recommened you secure your server**
