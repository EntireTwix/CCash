[PREVIOUS PAGE](user_side.md) | [NEXT PAGE](../building.md)

# Implementation Features
## Parallel Hashmap
<!-- memory vs database -->
<!-- phmap vs std hash map -->
<!-- https://greg7mdp.github.io/parallel-hashmap/ -->
#### Scalability
below is `GetBal()` being called where `x` axis grows with # of users reaching 10 million users, `y` axis is time in ns. The name given is random between 0 and max users at that time as to provide more accurate results
![image](GetBal().png)
as the graph demonstrates, regardless of size GetBal remains consistent at around 39ns on my 3700x.
## xxHash
## base64
## simdjson
## drogon webframework
## multi-threading support
## intelligent saving
## Backwards Compatible API
