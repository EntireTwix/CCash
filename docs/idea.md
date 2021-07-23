[PREVIOUS PAGE](../README.md) | [NEXT PAGE](connected_services/how_to/explanation.md)

CCash is an external ledger for in-game economies, running on a webserver with a RESTful API, exceptionally fast and lightweight written in C++.

While CCash can be used for anything that can interact with its API I think minecraft is a good usecase.

The currency model most Minecraft Servers adopt if any, is resource based, usually diamonds. This model is fraught with issues however:

* The primary issue is minecraft worlds are infinite leading to hyper inflation as everyone accrues more diamonds.
* There is no central authority minting the currency. Any consumer can introduce more diamonds to the system.
* Some resources are passively reapable, making the generation of currency a larger focus than that of creating products.
* Locality is required for transactions.
* Theft is possible; ownership is possession based.

CCash solves these issues and adds a level of abstraction. The main philosophy of CCash is to have fast core operations which other services build on.

The CCash instance can be external to the game server.

![image](external_diagram.png)

or on localhost:

![image](localhost_diagram.png)

Running it local to the game server reduces latency for connected services. Fortunately CCash is sufficiently lightweight as to not impact performance on most setups.

**DISCLAIMER: ComputerCraft requires you add `127.0.0.1` to its config section `allowed_domains` if you're interacting with CCash locally**   

As CCash is just a means of keeping track of who has what, the economic system you use is entirely up to whomever hosts the instance.

It's suggested an admin should manage the instance to lower incentive to manipulate.
