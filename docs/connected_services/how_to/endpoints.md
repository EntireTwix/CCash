# API endpoints
[PREVIOUS PAGE](APIs.md) | [NEXT PAGE](../existing_services.md)

## KEY
`Jresp` - Json Response, json must be accepted in the `Accept` header, be that via `application/json` or `*/*`, failing to do so results in error `406`

`Jreq` - Json Request, requires `application/json` as `content-type`, failing to do so results in error `406`

`U` - User, requires [basic auth](https://en.wikipedia.org/wiki/Basic_access_authentication) in the header `Authorization`. This credential must be a valid user, failing to do so results in error `401`

`A` - Admin, same as `U` but in addition requires username supplied be equal to the admin account username

⚠ - Deprecated endpoint

:no_entry: - Defunct endpoint

## all error responses have JSON string along with them to describe

### Usage endpoints
| name           | added on | purpose                                                                        | json input                       | path                            | HTTP Method | correct status |   return type    |                                return value                                |       Jresp        |        Jreq        |   A   |         U          |
| :------------- | :------: | ------------------------------------------------------------------------------ | -------------------------------- | ------------------------------- | :---------: | :------------: | :--------------: | :------------------------------------------------------------------------: | :----------------: | :----------------: | :---: | :----------------: |
| GetBal         | `v2.3.0` | retrieving the balance of a given user, `{name}`                               | `N/A`                            | api/v1/user/balance?name={name} |    `GET`    |      200       |      uint32      |                             the user's balance                             | :heavy_check_mark: |        :x:         |  :x:  |        :x:         |
| GetLog         | `v2.3.0` | retrieves the logs of a given user, length varies by server configuration      | `N/A`                            | ⚠ api/v1/user/log               |    `GET`    |      200       | array of objects |       [{"to":string, "from":string, "amount":uint32, "time":int64}]        | :heavy_check_mark: |        :x:         |  :x:  | :heavy_check_mark: |
| GetLogV2       | `v2.5.1` | retrieves the logs of a given user, length varies by server configuration      | `N/A`                            | api/v2/user/log                 |    `GET`    |      200       | array of objects | [{"counterparty":string, "receiving":bool, "amount":uint32, "time":int64}] | :heavy_check_mark: |        :x:         |  :x:  | :heavy_check_mark: |
| SendFunds      | `v2.3.0` | sends funds from the authenticated user to the user `{name}` given in the json | {"name":string, "amount":uint32} | api/v1/user/transfer            |   `POST`    |      200       |      uint32      |                  the user's balance after the transaction                  | :heavy_check_mark: | :heavy_check_mark: |  :x:  | :heavy_check_mark: |
| VerifyPassword | `v2.3.0` | verifies the credentials, used for connected services for ease of use          | `N/A`                            | api/v1/user/verify_password     |   `POST`    |      204       |      `N/A`       |                                   `N/A`                                    | :heavy_check_mark: |        :x:         |  :x:  | :heavy_check_mark: |

### Usage enpoint errors
| name           |        400         |        401         |        404         |        406         |
| :------------- | :----------------: | :----------------: | :----------------: | :----------------: |
| GetBal         |        :x:         |        :x:         | :heavy_check_mark: | :heavy_check_mark: |
| GetLog         |        :x:         | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: |
| GetLogV2       |        :x:         | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: |
| SendFunds      | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: |
| VerifyPassword |        :x:         | :heavy_check_mark: |        :x:         | :heavy_check_mark: |

### Meta Usage endpoints
| name                | added on | purpose                                                                                            | json input                      | path                              | HTTP Method | correct status | return type |          return value          |       Jresp        |        Jreq        |         A          |         U          |
| :------------------ | :------: | -------------------------------------------------------------------------------------------------- | ------------------------------- | --------------------------------- | :---------: | :------------: | :---------: | :----------------------------: | :----------------: | :----------------: | :----------------: | :----------------: |
| ChangePassword      | `v2.3.0` | changes the password of the Authenticated user                                                     | {"pass":string}                 | api/v1/user/change_password       |   `PATCH`   |      204       |    `N/A`    |             `N/A`              | :heavy_check_mark: | :heavy_check_mark: |        :x:         | :heavy_check_mark: |
| AdminChangePassword | `v2.3.0` | changes the password of a given user `{name}`                                                      | {"name":string,"pass":string}   | api/v1/admin/user/change_password |   `PATCH`   |      204       |    `N/A`    |             `N/A`              | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: |        :x:         |
| SetBal              | `v2.3.0` | sets the balance of a given user `{name}`                                                          | {"name":string,"amount":uint32} | api/v1/admin/set_balance          |   `PATCH`   |      204       |    `N/A`    |             `N/A`              | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: |        :x:         |
| ImpactBal           | `v2.3.0` | modifies the user `{name}`'s balance by `{amount}` if positive itll add, if negative itll subtract | {"name":string,"amount":int64}  | api/v1/admin/impact_balance       |   `POST`    |      200       |   uint32    | new balance after modification | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: |        :x:         |

### Meta Usage endpoint errors
| name                |        400         |        401         |        404         |        406         |
| :------------------ | :----------------: | :----------------: | :----------------: | :----------------: |
| ChangePassword      | :heavy_check_mark: | :heavy_check_mark: |        :x:         | :heavy_check_mark: |
| AdminChangePassword | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: |
| SetBal              | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: |
| ImpactBal           | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: |

### Sytem Usage endpoints
| name               | added on | purpose                                                                                                                   | json input                                               | path                           | HTTP Method | correct status | return type |                                                return value                                                 |       Jresp        |        Jreq        |         A          |   U   |
| :----------------- | :------: | ------------------------------------------------------------------------------------------------------------------------- | -------------------------------------------------------- | ------------------------------ | :---------: | :------------: | :---------: | :---------------------------------------------------------------------------------------------------------: | :----------------: | :----------------: | :----------------: | :---: |
| Help               | `v2.3.0` | redirects to GitHub projects Docs                                                                                         | `N/A`                                                    | api/help                       |    `GET`    |      301       |    `N/A`    |                                                    `N/A`                                                    |        :x:         |        :x:         |        :x:         |  :x:  |
| Close              | `v2.3.0` | saves & closes the CCash webserver                                                                                        | `N/A`                                                    | api/v1/admin/shutdown          |   `POST`    |      204       |    `N/A`    |                                                    `N/A`                                                    | :heavy_check_mark: |        :x:         | :heavy_check_mark: |  :x:  |
| Contains           | `v2.3.0` | checks wether a user exists                                                                                               | `N/A`                                                    | api/v1/user/exists?name={name} |    `GET`    |      204       |    `N/A`    |                                                    `N/A`                                                    | :heavy_check_mark: |        :x:         |        :x:         |  :x:  |
| AdminVerifyAccount | `v2.3.0` | checks wether a user is the admin                                                                                         | `N/A`                                                    | api/v1/admin/verify_account    |   `POST`    |      204       |    `N/A`    |                                                    `N/A`                                                    | :heavy_check_mark: |        :x:         | :heavy_check_mark: |  :x:  |
| PruneUsers         | `v2.3.0` | deletes users with most recent transactions older then `{time}` (if logs are enabled) and have less money then `{amount}` | {"time":int64,"amount":uint32} or just {"amount":uint32} | api/v1/admin/prune_users       |   `POST`    |      200       |   uint64    |                                           number of users deleted                                           | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: |  :x:  |
| ApiProperties      | `v2.3.0` | properties of the given instance                                                                                          | `N/A`                                                    | api/properties                 |    `GET`    |      200       | json object | {"version":uint64,"min_version":uint64_t,"max_log":uint64} and "return_on_del":string if feature is enabled | :heavy_check_mark: |        :x:         |        :x:         |  :x:  |

### System Usage endpoin errors
| name               |        401         |        404         |        406         |
| :----------------- | :----------------: | :----------------: | :----------------: |
| Help               |        :x:         |        :x:         |        :x:         |
| Close              | :heavy_check_mark: |        :x:         | :heavy_check_mark: |
| Contains           | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: |
| AdminVerifyAccount | :heavy_check_mark: |        :x:         | :heavy_check_mark: |
| PruneUsers         | :heavy_check_mark: |        :x:         | :heavy_check_mark: |
| ApiProperties      |        :x:         |        :x:         |        :x:         |

### Username Requirements
Valid
* lowercase letters
* numbers
* _
* Length must be atleast 3 and at most 16 characters.

### User Management endpoints
| name         | added on | purpose                                 | json input                                    | path                       | HTTP Method | correct status | return type | return value |       Jresp        |        Jreq        |         A          |         U          |
| :----------- | :------: | --------------------------------------- | --------------------------------------------- | -------------------------- | :---------: | :------------: | :---------: | :----------: | :----------------: | :----------------: | :----------------: | :----------------: |
| AddUser      | `v2.3.0` | adding a user with a balance of 0       | {"name":string,"pass":string}                 | api/v1/user/register       |   `POST`    |      204       |    `N/A`    |    `N/A`     | :heavy_check_mark: | :heavy_check_mark: |        :x:         |        :x:         |
| AdminAddUser | `v2.3.0` | adding a user with an arbitrary balance | {"name":string,"amount":uint32,"pass":string} | api/v1/admin/user/register |   `POST`    |      204       |    `N/A`    |    `N/A`     | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: |        :x:         |
| DelSelf      | `v2.3.0` | deletes a user                          | `N/A`                                         | api/v1/user/delete         |  `DELETE`   |      204       |    `N/A`    |    `N/A`     | :heavy_check_mark: |        :x:         |        :x:         | :heavy_check_mark: |
| AdminDelUser | `v2.3.0` | deletes a given user `{name}`           | {"name":string}                               | api/v1/admin/user/delete   |  `DELETE`   |      204       |    `N/A`    |    `N/A`     | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: |        :x:         |

### User Management endpoint errors
| name         |        400         |        401         |        404         |        406         |        409         |
| :----------- | :----------------: | :----------------: | :----------------: | :----------------: | :----------------: |
| AddUser      | :heavy_check_mark: |        :x:         | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: |
| AdminAddUser | :heavy_check_mark: | :heavy_check_mark: |        :x:         | :heavy_check_mark: | :heavy_check_mark: |
| DelSelf      |        :x:         | :heavy_check_mark: |        :x:         | :heavy_check_mark: |        :x:         |
| AdminDelUser | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: | :heavy_check_mark: |        :x:         |
