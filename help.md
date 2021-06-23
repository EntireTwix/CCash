# Error Responses

| #   | meaning           |
| --- | ----------------- |
| -1  | UserNotFound      |
| -2  | WrongPassword     |
| -3  | InvalidRequest    |
| -4  | NameTooLong       |
| -5  | UserAlreadyExists |
| -6  | InsufficientFunds |

# Things of Note
* all endpoints respond with **JSON** file type
* "**A**" denotes requiring Authentication in the form of a header titled "**Password**"

# Usage
|      Name      | Path                                   | Method |   A   | Description                                                                                                                     |
| :------------: | :------------------------------------- | :----: | :---: | ------------------------------------------------------------------------------------------------------------------------------- |
|     GetBal     | api/{name}/bal                       |  GET   | false | returns the balance of a given user `{name}`                                                                                    |
|     GetLog     | api/{name}/log                       |  GET   | true  | returns a list of last `n` number of transactions (a configurable amount when the program is compiled) of a given user `{name}` |
|   SendFunds    | api/{name}/send/{to}?amount={amount} |  POST  | true  | sends `{amount}` from user `{name}` to user `{to}`                                                                              |
| VerifyPassword | api/{name}/pass/verify               |  GET   | true  | returns `1` if the supplied user `{name}`'s password matches the password supplied in the header                                |

# Meta Usage
|      Name      | Path                                   | Method |   A   | Description                                                                                                                              |
| :------------: | :------------------------------------- | :----: | :---: | ---------------------------------------------------------------------------------------------------------------------------------------- |
| ChangePassword | api/{name}/pass/change               | PATCH  | true  | if the password supplied in the header matches the user `{name}`'s password, the user's password is changed to the one given in the body |
|     SetBal     | api/admin/{name}/bal?amount={amount} | PATCH  | true  | sets the balance of a give user `{name}` if the supplied password matches the admin password                                             |

# System Usage
|      Name       | Path                  | Method |   A   | Description                                                                           |
| :-------------: | :-------------------- | :----: | :---: | ------------------------------------------------------------------------------------- |
|      Help       | api/help            |  GET   | false | the page you're looking at right now!                                                 |
|      Ping       | api/ping            |  GET   | false | for pinging the server to see if its online                                           |
|      Close      | api/admin/close     |  POST  | true  | saves and then closes the program if the supplied password matches the admin password |
|    Contains     | api/contains/{name} |  GET   | false | returns `1` if the supplied user `{name}` exists                                      |
| AdminVerifyPass | api/admin/verify    |  GET   | true  | returns `1` if the password supplied in the header matches the admin password         |

# User Management
|     Name     | Path                                        | Method |   A   | Description                                                                                                                                                                                  |
| :----------: | :------------------------------------------ | :----: | :---: | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
|   AddUser    | api/user/{name}                           |  POST  | true  | registers a user with the name `{name}`, balance of 0 and a password of the password supplied in the header                                                                                  |
| AdminAddUser | api/admin/user/{name}?init_bal={init_bal} |  POST  | true  | if the password supplied in the header matches the admin password, then it registers a user with the name `{name}`, balance of `init_bal` and a password supplied by the body of the request |
|   DelUser    | api/user/{name}                           | DELETE | true  | if the password supplied in the header matches the user `{name}`'s password, then the user is deleted                                                                                        |
| AdminDelUser | api/admin/user/{name}                     | DELETE | true  | if the password supplied in the header matches the admin password, then the user is deleted                                                                                                  |
