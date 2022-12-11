[PREVIOUS PAGE](building.md) 

#### Q : How do I setup a CCash instance
#### A : You can with [build docs](https://github.com/EntireTwix/CCash/blob/main/docs/building.md) you can build from source or use the Docker package.


#### Q : Why is my username invalid
#### A : Usernames are restricted by minecraft's requirements
* lowercase letters
* numbers
* _
* Length must be atleast 3 and at most 16 characters.


#### Q : Is this a crypto like krist?
#### A : CCash isn't a crypto, the economy you decide to implement is entirely up to you.


#### Q : Why isnt this on a database?
#### A : Because this usecase requires none of the features a database would offer. 


#### Q : People are making too many accounts maliciously to fill up space on my server!
#### A : Consider disabling `ADD_USER_OPEN` in the [build proccess](https://github.com/EntireTwix/CCash/blob/main/docs/building.md).


#### Q : People are making too many transactions filling up space on my server!
#### A : Consider setting the variable `MAX_LOG_SIZE` to 0 in the [build proccess](https://github.com/EntireTwix/CCash/blob/main/docs/building.md).


#### Q : My instance is taking up too much RAM
#### A : Reduce log size.


#### Q : My instance is taking up too much disk spac
#### A : Use the prune users endpoint to dead accounts or you can reduce log size but should be a last resort.


#### Q : My instance is slow
#### A : Toggling off logs will increase performance. On some devices multi threading increases performance, on others the overhead isn't worth the benefit.

