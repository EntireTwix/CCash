[PREVIOUS PAGE](../README.md) | [NEXT PAGE](connected_services/how_to/explanation.md)

CCash is an external ledger for in-game economies, running on a webserver with a RESTful API, exceptionally fast and lightweight written in C++.

While CCash can be used for anything that can interact with its API I think minecraft is a good usecase.

The currency model most Minecraft Servers adopt if any, is resource based, usually diamonds. This model is fraught with issues however:

* The primary issue is minecraft worlds are infinite, leading to high inflation as everyone accrues more diamonds, the value of the currency being dictated by the rate at which the resource is mined.
<!-- * The diminishing marginal value of diamonds makes them a poor store of value, trading something to receive in exchange your 110th diamond is meaningless. -->
* The growth of the economy can never exceed in value the total resources backing the currency.
* Some resources are passively reapable (e.g iron or gold), making the generation of currency a larger focus than that of creating value.
* Locality is required for transactions.
* Theft is possible; ownership is physical possession based.

CCash is a means of solving these issues and adds a level of abstraction. The main philosophy of CCash is to have fast core operations which other services build on, CCash can proccess thousands of requests per second.

The CCash instance can be external to the game server:

![image](external_diagram.png)

Or on localhost:

![image](localhost_diagram.png)

Running it local to the game server reduces latency for connected services and CCash is very lightweight and so will be a tiny proportion of the server's total computation.

**DISCLAIMER: ComputerCraft requires you add `127.0.0.1` to its config section `allowed_domains` if you're interacting with CCash locally**   

As CCash is just a means of keeping track of who has what, the economic system you implement is entirely up to whomever hosts the instance.

I suggest an admin manage the instance so there is less incentive to manipulate balances.

Check out the [FAQ](FAQ.md) if you have any questions.
