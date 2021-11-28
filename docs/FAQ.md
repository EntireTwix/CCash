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

#### Q : My instance is taking up too much RAM
#### A : If your memory usage is too high I would reduce log size.

#### Q : My instance is slow
#### A : Toggling off logs will increase performance. On some devices multi threading increases performance, on others the overhead isn't worth the benefit. If when the frequency of saving arrives its highly likely changes have occurred (say by high activity or low save frequency) then disabling conservative saving will marginally increase performance.

#### Q : My instance is taking up too much disk space
#### A : Use the purge users endpoint to delete old or superfluous accounts, this will reduce disk space. If you're feeling advanced, setting logs to 0 then restoring it to whatever it was will wipe logs, or simply reducing log size permanently, either of these will reduce disk space.
