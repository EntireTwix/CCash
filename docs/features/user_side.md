[PREVIOUS PAGE](../connected_services/existing_services.md) | [NEXT PAGE](implementation.md)

# Features
## Performance
#### Speed
<!-- graphs -->
Capable of processing thousands of requests per second, with no slow down as user size grows to the millions.

#### Lightweight
<!-- specs -->
Low memory usage at 8 Mb baseline, and 203 bytes per new log.

Anecdotally I typically expierenced <1% CPU usage.

Small save files at typically a couple kb, easily shareable.
## Accessibility
#### Connected Services
as explained in earlier docs a ecosystem of connected services allows you many ways to utilize CCash.
#### APIs
for devs who wanna make more connected services, existing APIs exist in multiple lanaguages enabling faster development/maintenance.
#### External
its game indepedent meaning you dont have to be in-game to use it.
## Security
#### HTTPS
OpenSSL is used to secure the HTTP server.
#### Hashed Passwords
if the server's save file is compromised the user passwords will not be derivable.
## Other
#### Logs
each transaction is logged and the last `n` logs are stored, if set to 0 logs will be disabled.
#### Return On Delete
by default this feature is off, but when enabled deleted account's fund will be redirected to a specified account rather then "burned"
#### Configurable
as you can read in [building.md](../building.md) CCash is highly configurable.
