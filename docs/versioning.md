# Versioning

## Endpoint Version
When changing an existing endpoint in a breaking way the version will increment in the path e.g `api/v1/user/log` -> `api/v2/user/log`. If the change is non-breaking then extra parameters will simply be added to the endpoint. If the older version is marked deprecated it will be documented in [endpoints.md](connected_services/how_to/endpoints.md)

## Release Version

Major changes (e.g `v1.0.0` -> `v2.0.0`) denote a breaking change as all previously deprecated endpoints will now be made defunct and wont be built with that release. 

Minor changes (e.g `v1.0.0` -> `v1.1.0`) denote a non-breaking change that adds or changes something about CCash.

Patches (e.g `v1.0.0` -> `v1.0.1`) denote bug fixes.
