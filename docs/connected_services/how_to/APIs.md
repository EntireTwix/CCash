[PREVIOUS PAGE](explanation.md) | [NEXT PAGE](endpoints.md)

note: all CCash version's API are backwards compatible, so while a language's API may not support the newest CCash release it can still use the old endpoints.
| author                                       | language |                                                                     |    v1 endpoints    | v2 endpoints |
| :------------------------------------------- | :------: | ------------------------------------------------------------------- | :----------------: | :----------: |
| [SpaceCat](https://github.com/SpaceCat-Chan) |  CCLua   | [CatsCCashLuaApi](https://github.com/SpaceCat-Chan/CatsCCashLuaApi) | :heavy_check_mark: |     :x:      |
| [Doggo](https://github.com/ArcNyxx)          |  Python  | [CCashPythonClient](https://github.com/ArcNyxx/ccash_python_client) | :heavy_check_mark: |     :x:      |
| [Sam](https://github.com/STBoyden)           |   Rust   | [ccash rs](https://github.com/STBoyden/ccash-rs)                    | :heavy_check_mark: |     :x:      |

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
