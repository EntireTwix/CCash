[PREVIOUS PAGE](building.md) 

### How do I setup a CCash instance
You can with [build docs](https://github.com/EntireTwix/CCash/blob/main/docs/building.md) you can build from source or use the Docker package.
### Why is my username invalid
Usernames are restricted by minecraft's requirements
* letters
* numbers
* _
* length must be atleast 3 and at most 16 characters.
### Why isnt this on a database?
Because this usecase requires none of the features a database would offer. 
### People are maliciously making too many accounts!
Consider disabling `ADD_USER_OPEN` in the [build proccess](https://github.com/EntireTwix/CCash/blob/main/docs/building.md).
### My instance is taking up too much storage or RAM
Reduce log size and/or use the prune users endpoint to remove dead accounts.

## Minecraft
### Are mods required to use this
No, as this is API based, services can interact with the ledger outside of the game.
### Is this crypto like krist?
CCash isn't a crypto, simply a ledger keeping track of who owns what.
### Why not use an economy mod
Speed of operations, CCash being external to MC (and so compatible with any version/configuration), and the API are the main advantages over using an economy mod.
