A web server hosting a ledger for Minecraft, able to be used from anything that can interact with its Restful API, including ComputerCraft. 

the currency model most Minecraft Servers adopt if any, is resource based, usually diamonds, this model is fraught with issues however:

* the primary issue is minecraft worlds are infinite leading to hyper inflation as everyone accrues more diamonds
* there is no central authority minting the currency, any consumer can introduce more diamonds to the system
* some resources are passively reapable, making the generation of currency a larger focus then of products
* locality is required for transaction
* theft is possible, ownership is possession based

CCash solves these issues and adds a level of abstraction, the main philosophy of CCash is to have fast core operations that other services build on