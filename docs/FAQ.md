[PREVIOUS PAGE](building.md) 

### How do I setup a CCash instance
You can with [build docs](https://github.com/EntireTwix/CCash/blob/main/docs/building.md) you can build from source or use the Docker package.
### Why is my username invalid
Usernames are restricted by minecraft's requirements
* lowercase letters
* numbers
* _
* length must be atleast 3 and at most 16 characters.
### Is this crypto like krist?
CCash isn't a crypto, simply a ledger keeping track of who owns what.
### Why isnt this on a database?
Because this usecase requires none of the features a database would offer. 
### People are making too many accounts maliciously to fill up space on my server!
Consider disabling `ADD_USER_OPEN` in the [build proccess](https://github.com/EntireTwix/CCash/blob/main/docs/building.md).
### My instance is taking up too much storage or RAM
Reduce log size and/or use the prune users endpoint to remove dead accounts.
### How do I start an economy
I believe setting up a commodities exchange is the best way to get an economy going initially. 
### Why not use an economy mod
Speed of operations, CCash being external to MC (and so compatible with any version/configuration), and the API are the main advantages to an economy mod.