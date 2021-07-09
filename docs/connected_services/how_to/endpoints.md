# API endpoints
[PREVIOUS PAGE](endpoints.md) | [NEXT PAGE](../existing_services.md)

## KEY
`Jresp` - Json Response, json must be accepted in the `Accept` header, be that via `application/json` or `*/*`, failing to do so results in error `406`

`Jreq` - Json Request, requires `application/json` as `content-type`, failing to do so results in error `406`

`U` - User, requires [basic auth](https://en.wikipedia.org/wiki/Basic_access_authentication) in the header `Authorization`. This credential must be a valid user, failing to do so results in error `401`

`A` - Admin, same as `U` but in addition requires username supplied be equal to the admin account username

:heavy_check_mark:
:heavy_multiplication_x:

### Usage endpoints
| name   | purpose                                                                   | json input | path                              | HTTP Method |  return type   |                 return value                 |       Jresp        |           Jreq           |            A             |            U             |
| :----- | ------------------------------------------------------------------------- | ---------- | --------------------------------- | :---------: | :------------: | :------------------------------------------: | :----------------: | :----------------------: | :----------------------: | :----------------------: |
| GetBal | retrieving the balance of a given user, `{name}`                          | `N/A`      | `api/v1/user/balance?name={name}` |    `GET`    |     uint32     |              the user's balance              | :heavy_check_mark: | :heavy_multiplication_x: | :heavy_multiplication_x: | :heavy_multiplication_x: |
| GetLog | retrieves the logs of a given user, length varies by server configuration | `N/A`      | `api/v1/user/log`                 |    `GET`    | array of jsons | {"to":string, "amount":uint32, "time":int64} | :heavy_check_mark: | :heavy_multiplication_x: | :heavy_multiplication_x: |    :heavy_check_mark:    |

### Usage enpoints errors
| name   |           400            |           401            |        404         |        406         |
| :----- | :----------------------: | :----------------------: | :----------------: | :----------------: |
| GetBal | :heavy_multiplication_x: | :heavy_multiplication_x: | :heavy_check_mark: | :heavy_check_mark: |
| GetLog | :heavy_multiplication_x: |    :heavy_check_mark:    | :heavy_check_mark: | :heavy_check_mark: |

### Usage endpoints support
`v` denoting the API version
| name   |         v1         |
| :----- | :----------------: |
| GetBal | :heavy_check_mark: |
| GetLog | :heavy_check_mark: |

