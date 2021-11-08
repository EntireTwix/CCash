# API endpoints
[PREVIOUS PAGE](APIs.md) | [NEXT PAGE](../existing_services.md)

## KEY
`Jresp` - Json Response, json must be accepted in the `Accept` header, be that via `application/json` or `*/*`, failing to do so results in error `406`

`Jreq` - Json Request, requires `application/json` as `content-type`, failing to do so results in error `406`

`U` - User, requires [basic auth](https://en.wikipedia.org/wiki/Basic_access_authentication) in the header `Authorization`. This credential must be a valid user, failing to do so results in error `401`

`A` - Admin, same as `U` but in addition requires username supplied be equal to the admin account username

:heavy_check_mark:
:heavy_multiplication_x:

## all error responses have JSON string along with them to describe

### Usage endpoints
| name           | purpose                                                                        | json input                       | path                            | HTTP Method | correct status |   return type    |                         return value                          |       Jresp        |           Jreq           |            A             |            U             |
| :------------- | ------------------------------------------------------------------------------ | -------------------------------- | ------------------------------- | :---------: | :------------: | :--------------: | :-----------------------------------------------------------: | :----------------: | :----------------------: | :----------------------: | :----------------------: |
| GetBal         | retrieving the balance of a given user, `{name}`                               | `N/A`                            | api/v1/user/balance?name={name} |    `GET`    |      200       |      uint32      |                      the user's balance                       | :heavy_check_mark: | :heavy_multiplication_x: | :heavy_multiplication_x: | :heavy_multiplication_x: |
| GetLog         | retrieves the logs of a given user, length varies by server configuration      | `N/A`                            | api/v1/user/log                 |    `GET`    |      200       | array of objects | [{"to":string, "from":string, "amount":uint32, "time":int64}] | :heavy_check_mark: | :heavy_multiplication_x: | :heavy_multiplication_x: |    :heavy_check_mark:    |
| SendFunds      | sends funds from the authenticated user to the user `{name}` given in the json | {"name":string, "amount":uint32} | api/v1/user/transfer            |   `POST`    |      200       |      uint32      |           the user's balance after the transaction            | :heavy_check_mark: |    :heavy_check_mark:    | :heavy_multiplication_x: |    :heavy_check_mark:    |
| VerifyPassword | verifies the credentials, used for connected services for ease of use          | `N/A`                            | api/v1/user/verify_password     |   `POST`    |      204       |      `N/A`       |                             `N/A`                             | :heavy_check_mark: | :heavy_multiplication_x: | :heavy_multiplication_x: |    :heavy_check_mark:    |

### Usage enpoint errors
| name           |           400            |           401            |           404            |        406         |
| :------------- | :----------------------: | :----------------------: | :----------------------: | :----------------: |
| GetBal         | :heavy_multiplication_x: | :heavy_multiplication_x: |    :heavy_check_mark:    | :heavy_check_mark: |
| GetLog         | :heavy_multiplication_x: |    :heavy_check_mark:    |    :heavy_check_mark:    | :heavy_check_mark: |
| SendFunds      |    :heavy_check_mark:    |    :heavy_check_mark:    |    :heavy_check_mark:    | :heavy_check_mark: |
| VerifyPassword | :heavy_multiplication_x: |    :heavy_check_mark:    | :heavy_multiplication_x: | :heavy_check_mark: |

### Usage endpoint support
`v` denoting the API version
| name           |         v1         |
| :------------- | :----------------: |
| GetBal         | :heavy_check_mark: |
| GetLog         | :heavy_check_mark: |
| SendFunds      | :heavy_check_mark: |
| VerifyPassword | :heavy_check_mark: |

### Meta Usage endpoints
| name                | purpose                                                                                            | json input                      | path                              | HTTP Method | correct status | return type |          return value          |       Jresp        |        Jreq        |            A             |            U             |
| :------------------ | -------------------------------------------------------------------------------------------------- | ------------------------------- | --------------------------------- | :---------: | :------------: | :---------: | :----------------------------: | :----------------: | :----------------: | :----------------------: | :----------------------: |
| ChangePassword      | changes the password of the Authenticated user                                                     | {"pass":string}                 | api/v1/user/change_password       |   `PATCH`   |      204       |    `N/A`    |             `N/A`              | :heavy_check_mark: | :heavy_check_mark: | :heavy_multiplication_x: |    :heavy_check_mark:    |
| AdminChangePassword | changes the password of a given user `{name}`                                                      | {"name":string,"pass":string}   | api/v1/admin/user/change_password |   `PATCH`   |      204       |    `N/A`    |             `N/A`              | :heavy_check_mark: | :heavy_check_mark: |    :heavy_check_mark:    | :heavy_multiplication_x: |
| SetBal              | sets the balance of a given user `{name}`                                                          | {"name":string,"amount":uint32} | api/v1/admin/set_balance          |   `PATCH`   |      204       |    `N/A`    |             `N/A`              | :heavy_check_mark: | :heavy_check_mark: |    :heavy_check_mark:    | :heavy_multiplication_x: |
| ImpactBal           | modifies the user `{name}`'s balance by `{amount}` if positive itll add, if negative itll subtract | {"name":string,"amount":int64}  | api/v1/admin/impact_balance       |   `POST`    |      200       |   uint32    | new balance after modification | :heavy_check_mark: | :heavy_check_mark: |    :heavy_check_mark:    | :heavy_multiplication_x: |

### Meta Usage endpoint errors
| name                |        400         |        401         |           404            |        406         |
| :------------------ | :----------------: | :----------------: | :----------------------: | :----------------: |
| ChangePassword      | :heavy_check_mark: | :heavy_check_mark: | :heavy_multiplication_x: | :heavy_check_mark: |
| AdminChangePassword | :heavy_check_mark: | :heavy_check_mark: |    :heavy_check_mark:    | :heavy_check_mark: |
| SetBal              | :heavy_check_mark: | :heavy_check_mark: |    :heavy_check_mark:    | :heavy_check_mark: |
| ImpactBal           | :heavy_check_mark: | :heavy_check_mark: |    :heavy_check_mark:    | :heavy_check_mark: |

### Meta Usage endpoint support
| name                |         v1         |
| :------------------ | :----------------: |
| ChangePassword      | :heavy_check_mark: |
| AdminChangePassword | :heavy_check_mark: |
| SetBal              | :heavy_check_mark: |
| ImpactBal           | :heavy_check_mark: |

### Sytem Usage endpoints
| name               | purpose                                                                                                                   | json input                                               | path                           | HTTP Method | correct status | return type |                                     return value                                     |          Jresp           |           Jreq           |            A             |            U             |
| :----------------- | ------------------------------------------------------------------------------------------------------------------------- | -------------------------------------------------------- | ------------------------------ | :---------: | :------------: | :---------: | :----------------------------------------------------------------------------------: | :----------------------: | :----------------------: | :----------------------: | :----------------------: |
| Help               | redirects to GitHub projects Docs                                                                                         | `N/A`                                                    | api/v1/help                    |    `GET`    |      301       |    `N/A`    |                                        `N/A`                                         | :heavy_multiplication_x: | :heavy_multiplication_x: | :heavy_multiplication_x: | :heavy_multiplication_x: |
| Close              | saves & closes the CCash webserver                                                                                        | `N/A`                                                    | api/v1/admin/shutdown          |   `POST`    |      204       |    `N/A`    |                                        `N/A`                                         |    :heavy_check_mark:    | :heavy_multiplication_x: |    :heavy_check_mark:    | :heavy_multiplication_x: |
| Contains           | checks wether a user exists                                                                                               | `N/A`                                                    | api/v1/user/exists?name={name} |    `GET`    |      204       |    `N/A`    |                                        `N/A`                                         |    :heavy_check_mark:    | :heavy_multiplication_x: | :heavy_multiplication_x: | :heavy_multiplication_x: |
| AdminVerifyAccount | checks wether a user is the admin                                                                                         | `N/A`                                                    | api/v1/admin/verify_account    |   `POST`    |      204       |    `N/A`    |                                        `N/A`                                         |    :heavy_check_mark:    | :heavy_multiplication_x: |    :heavy_check_mark:    | :heavy_multiplication_x: |
| PruneUsers         | deletes users with most recent transactions older then `{time}` (if logs are enabled) and have less money then `{amount}` | {"time":int64,"amount":uint32} or just {"amount":uint32} | api/v1/admin/prune_users       |   `POST`    |      200       |   uint64    |                               number of users deleted                                |    :heavy_check_mark:    |    :heavy_check_mark:    |    :heavy_check_mark:    | :heavy_multiplication_x: |
| ApiProperties      | properties of the given instance                                                                                          | `N/A`                                                    | api/properties                 |    `GET`    |      200       | json object | {"version":uint64,"max_log":uint64} and "return_on_del":string if feature is enabled |    :heavy_check_mark:    | :heavy_multiplication_x: | :heavy_multiplication_x: | :heavy_multiplication_x: |

### System Usage endpoin errors
| name               |           401            |           404            |           406            |
| :----------------- | :----------------------: | :----------------------: | :----------------------: |
| Help               | :heavy_multiplication_x: | :heavy_multiplication_x: | :heavy_multiplication_x: |
| Close              |    :heavy_check_mark:    | :heavy_multiplication_x: |    :heavy_check_mark:    |
| Contains           |    :heavy_check_mark:    |    :heavy_check_mark:    |    :heavy_check_mark:    |
| AdminVerifyAccount |    :heavy_check_mark:    | :heavy_multiplication_x: |    :heavy_check_mark:    |
| PruneUsers         |    :heavy_check_mark:    | :heavy_multiplication_x: |    :heavy_check_mark:    |
| ApiProperties      | :heavy_multiplication_x: | :heavy_multiplication_x: | :heavy_multiplication_x: |

### System Usage endpoint support
| name               |         v1         |
| :----------------- | :----------------: |
| Help               | :heavy_check_mark: |
| Close              | :heavy_check_mark: |
| Contains           | :heavy_check_mark: |
| AdminVerifyAccount | :heavy_check_mark: |

### Username Requirements
Valid
* lowercase letters
* numbers
* _
* Length must be atleast 3 and at most 16 characters.

### User Management endpoints
| name         | purpose                                 | json input                                    | path                       | HTTP Method | correct status | return type | return value |       Jresp        |           Jreq           |            A             |            U             |
| :----------- | --------------------------------------- | --------------------------------------------- | -------------------------- | :---------: | :------------: | :---------: | :----------: | :----------------: | :----------------------: | :----------------------: | :----------------------: |
| AddUser      | adding a user with a balance of 0          | {"name":string,"pass":string}                 | api/v1/user/register       |   `POST`    |      204       |    `N/A`    |    `N/A`     | :heavy_check_mark: |    :heavy_check_mark:    | :heavy_multiplication_x: | :heavy_multiplication_x: |
| AdminAddUser | adding a user with an arbitrary balance | {"name":string,"amount":uint32,"pass":string} | api/v1/admin/user/register |   `POST`    |      204       |    `N/A`    |    `N/A`     | :heavy_check_mark: |    :heavy_check_mark:    |    :heavy_check_mark:    | :heavy_multiplication_x: |
| DelSelf      | deletes a user                          | `N/A`                                         | api/v1/user/delete         |  `DELETE`   |      204       |    `N/A`    |    `N/A`     | :heavy_check_mark: | :heavy_multiplication_x: | :heavy_multiplication_x: |    :heavy_check_mark:    |
| AdminDelUser | deletes a given user `{name}`           | {"name":string}                               | api/v1/admin/user/delete   |  `DELETE`   |      204       |    `N/A`    |    `N/A`     | :heavy_check_mark: |    :heavy_check_mark:    |    :heavy_check_mark:    | :heavy_multiplication_x: |

### User Management endpoint errors
| name         |           400            |           401            |           404            |        406         |           409            |
| :----------- | :----------------------: | :----------------------: | :----------------------: | :----------------: | :----------------------: |
| AddUser      |    :heavy_check_mark:    | :heavy_multiplication_x: | :heavy_multiplication_x: | :heavy_check_mark: |    :heavy_check_mark:    |
| AdminAddUser |    :heavy_check_mark:    |    :heavy_check_mark:    | :heavy_multiplication_x: | :heavy_check_mark: |    :heavy_check_mark:    |
| DelSelf      | :heavy_multiplication_x: |    :heavy_check_mark:    | :heavy_multiplication_x: | :heavy_check_mark: | :heavy_multiplication_x: |
| AdminDelUser |    :heavy_check_mark:    |    :heavy_check_mark:    |    :heavy_check_mark:    | :heavy_check_mark: | :heavy_multiplication_x: |

### User Management endpoint support
| name         |         v1         |
| :----------- | :----------------: |
| AddUser      | :heavy_check_mark: |
| AdminAddUser | :heavy_check_mark: |
| DelSelf      | :heavy_check_mark: |
| AdminDelUser | :heavy_check_mark: |
