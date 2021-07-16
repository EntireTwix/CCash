[PREVIOUS PAGE](user_side.md) | [NEXT PAGE](../building.md)

# Implementation Features
## Parallel Hashmap
<!-- memory vs database -->
<!-- phmap vs std hash map -->
<!-- https://greg7mdp.github.io/parallel-hashmap/ -->
### Scalability
below is `GetBal()` being called where `x` axis grows with # of users reaching 10 million users, `y` axis is time in ns. The name given is random between 0 and max users at that time as to provide more accurate results
![image](GetBal().png)
as the graph demonstrates, regardless of size GetBal remains consistent at around 39ns on my 3700x.
## xxHash
## Base64
## Simdjson
## Drogon webframework
## Multi-threading support
## Intelligent saving
### Saving on close
when the program is interupted with CONTROL + C it will save before closing the webserver, **it will not however save during a crash**.
### Auto Saving
every `x` minutes, a configurable amount at launch, CCash will save.
### Changes
for the above two cases, it will only save if changes have been made since last commit.
## Backwards Compatible API
versioning is implemented by the endpoints path, for example `/v1`. Breaking changes will ideally be sparse and backwards compatability will be maintained, for example ideally API `v3` instance can still run `v1` endpoints