[PREVIOUS PAGE](how_to/endpoints.md) | [NEXT PAGE](../features/user_side.md)

## Key
|      description      | symbol             |
| :-------------------: | :----------------- |
|       supported       | :heavy_check_mark: |
| calls older endpoints | ⚠                  |
| calls newer endpoints | :x:                |

Just because a service calls old endpoints, doesn't mean the service will not work under that version of CCash. As long as the `MIN_API_SUPPORT` is less than or equal to the required version's endpoints, then the connected service can call the old CCash endpoints. **By default `MIN_API_SUPPORT` is 1 (complete backwards compatibility) making all ⚠ effectively :heavy_check_mark:**.

## General
| author                                  | name                                                        |         v1         |  v2   |                                                      image                                                      |
| :-------------------------------------- | ----------------------------------------------------------- | :----------------: | :---: | :-------------------------------------------------------------------------------------------------------------: |
| [Expand](https://github.com/Expand-sys) | [Web Frontend](https://github.com/Expand-sys/ccashfrontend) | :heavy_check_mark: |   ⚠   | ![image](https://user-images.githubusercontent.com/31377881/121337724-afe9fe80-c8d1-11eb-8851-23ec5e74cd26.png) |
| [Expand](https://github.com/Expand-sys) | [Discord Bot](https://github.com/Expand-sys/ccashbot)       | :heavy_check_mark: |   ⚠   |                                                                                                                 |
| [ArcNyxx](https://github.com/ArcNyxx)   | [CCash CLI](https://github.com/ArcNyxx/ccash_cmd)           | :heavy_check_mark: |   ⚠   |                                                                                                                 |

## Minecraft
| author                                      | name                                                                        |         v1         |         v2         |                                                      image                                                      |
| :------------------------------------------ | --------------------------------------------------------------------------- | :----------------: | :----------------: | :-------------------------------------------------------------------------------------------------------------: |
| [Reactified](https://github.com/Reactified) | [Shop](https://github.com/Reactified/rpm/tree/main/packages/ccash-shop)     | :heavy_check_mark: |         ⚠          | ![image](https://user-images.githubusercontent.com/31377881/120050327-de163700-bfd1-11eb-9d5a-f75c003e867c.png) |
| [Reactified](https://github.com/Reactified) | [Wallet](https://github.com/Reactified/rpm/tree/main/packages/ccash-wallet) | :heavy_check_mark: |         ⚠          | ![image](https://user-images.githubusercontent.com/31377881/121338034-fb041180-c8d1-11eb-8640-b18c141eb980.png) |
| [Reactified](https://github.com/Reactified) | [ATM](https://github.com/Reactified/rpm/tree/main/packages/ccash-bank)      | :heavy_check_mark: |         ⚠          | ![image](https://user-images.githubusercontent.com/31377881/121277361-4d6b1100-c885-11eb-87c8-cfebcf58da4f.png) |
| [STBoyden](https://github.com/STBoyden)     | Commodities Exchange (in-development)                                       |        :x:         | :heavy_check_mark: |                                                                                                                 |

## Desired
|   idea   | description                                                    |
| :------: | :------------------------------------------------------------- |
| Gambling | physical or digital casino.                                    |
| Shipping | the infastructure to quickly send items across long distances. |
|   Mod    | a server-side mod                                              |