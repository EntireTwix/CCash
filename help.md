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

- all endpoints respond with **JSON** file type
- "**A**" denotes requiring Authentication in the form of a header titled "**Password**"

# Usage

|      Name      | Path                                   | Method |   A   | Description                                                                                        |
| :------------: | :------------------------------------- | :----: | :---: | -------------------------------------------------------------------------------------------------- |
|     GetBal     | BankF/{name}/bal                       |  GET   | false | returns the balance of a given user `{name}`                                                       |
|     GetLog     | BankF/{name}/log                       |  GET   | true  | returns a list of last `n` number of transactions (configurable in CCash webserver) of a given user `{name}` |
|   SendFunds    | BankF/{name}/send/{to}/amount={amount} |  POST  | true  | sends `{amount}` from user `{name}` to user `{to}`                                                 |
| VerifyPassword | BankF/{name}/pass/verify               |  GET   | true  | returns `1` if the supplied user `{name}`'s password matches the password supplied in the header   |

# Meta Usage

|      Name      | Path                                   | Method |  A   | Description                                                                                                                                                                                            |
| :------------: | :------------------------------------- | :----: | :--: | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| ChangePassword | BankF/{name}/pass/change               | PATCH  | true | if the password supplied in the header matches the user `{name}`'s password, the user's password is changed to the one given in the body, with a key of `password`. i.e. { "password": "NewPassword" } |
|     SetBal     | BankF/admin/{name}/bal/amount={amount} | PATCH  | true | sets the balance of a give user `{name}` if the supplied password matches the admin password                                                                                                           |

# System Usage

|      Name       | Path                  | Method |   A   | Description                                                                           |
| :-------------: | :-------------------- | :----: | :---: | ------------------------------------------------------------------------------------- |
|      Help       | BankF/help            |  GET   | false | the page you're looking at right now!                                                 |
|      Ping       | BankF/ping            |  GET   | false | for pinging the server to see if its online                                           |
|      Close      | BankF/admin/close     |  POST  | true  | saves and then closes the program if the supplied password matches the admin password |
|    Contains     | BankF/contains/{name} |  GET   | false | returns `1` if the supplied user `{name}` exists                                      |
| AdminVerifyPass | BankF/admin/verify    |  GET   | true  | returns `1` if the password supplied in the header matches the admin password         |

# User Management

|     Name     | Path                                        | Method |  A   | Description                                                                                                                                                                                                                              |
| :----------: | :------------------------------------------ | :----: | :--: | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
|   AddUser    | BankF/user/{name}                           |  POST  | true | registers a user with the name `{name}`, balance of 0 and a password of the password supplied in the header                                                                                                                              |
| AdminAddUser | BankF/admin/user/{name}?init_bal={init_bal} |  POST  | true | if the password supplied in the header matches the admin password, then it registers a user with the name `{name}`, balance of `init_bal` and a password that is supplied in the body of the request. i.e. { "password": "ThePassword" } |
|   DelUser    | BankF/user/{name}                           | DELETE | true | if the password supplied in the header matches the user `{name}`'s password, then the user is deleted                                                                                                                                    |
| AdminDelUser | BankF/admin/user/{name}                     | DELETE | true | if the password supplied in the header matches the admin password, then the user is deleted                                                                                                                                              |
