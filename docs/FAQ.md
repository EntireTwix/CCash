[PREVIOUS PAGE](building.md) 

### How do I setup a CCash instance
You can with [build docs](https://github.com/EntireTwix/CCash/blob/main/docs/building.md) you can build from source or use the Docker package.
### Why is my username invalid
Usernames are restricted by minecraft's requirements
* lowercase letters
* numbers
* _
* Length must be atleast 3 and at most 16 characters.
### Is this crypto like krist?
CCash isn't a crypto, simply a ledger keeping track of who owns what.
### Why isnt this on a database?
Because this usecase requires none of the features a database would offer. 
### People are making too many accounts maliciously to fill up space on my server!
Consider disabling `ADD_USER_OPEN` in the [build proccess](https://github.com/EntireTwix/CCash/blob/main/docs/building.md).
### My instance is taking up too much RAM
Reduce log size.
### My instance is taking up too much disk spac
Use the prune users endpoint to dead accounts or you can reduce log size but should be a last resort.
### My instance is slow
Toggling off logs will increase performance. On some devices multi threading increases performance, on others the overhead isn't worth the benefit.
