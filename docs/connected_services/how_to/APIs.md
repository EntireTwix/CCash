[PREVIOUS PAGE](explanation.md) | [NEXT PAGE](endpoints.md)

CCash is backwards compatible, so even if a language API does not support the newester version it can still call the old endpoints. Only when the major version increments are deprecated features made defunct (e.g `v1.0.0 -> v2.0.0`), check [endpoints.md](endpoints.md) to avoid using deprecated endpoints.
| author                                       | language |                                                                     | newest CCash supported version |
| :------------------------------------------- | :------: | ------------------------------------------------------------------- | :----------------------------: |
| [SpaceCat](https://github.com/SpaceCat-Chan) |  CCLua   | [CatsCCashLuaApi](https://github.com/SpaceCat-Chan/CatsCCashLuaApi) |            `v1.4.3`            |
| [Doggo](https://github.com/ArcNyxx)          |  Python  | [CCashPythonClient](https://github.com/ArcNyxx/ccash_python_client) |            `v1.4.3`            |
| [Sam](https://github.com/STBoyden)           |   Rust   | [ccash rs](https://github.com/STBoyden/ccash-rs)                    |            `v1.4.3`            |

for example here is a demo program for the lua API by SpaceCat

```lua
local ccash = require("ccash.api")
ccash.meta.set_server_address("https://ccashinstance.net/")

local cred_name = "my_account"
local cred_pass = "my_pass"

math.randomseed(os.time())
local temp_name = tostring(math.random(100, 2147483647))
print("temp account "..temp_name)

print("enter target name ")
local target_name = io.read()
print("enter target amount ")  
local target_amount = tonumber(io.read())

print(ccash.register(temp_name, "root123"))
print(ccash.send_funds(cred_name, cred_pass, temp_name, target_amount))
print(ccash.send_funds(temp_name, "root123", target_name, target_amount))
print(ccash.delete_self(temp_name, "root123"))
```

this particular program obfuscates transactions so that the person receiving it has no way of knowing who it came from 

```
my_account -> temp_account -> target_account
          +100            +100
```
then temp_account is deleted
