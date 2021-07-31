# Deploying CCash

CCash can deployed to a remote machine pretty simply.

A pre-built docker image is supplied in the repos [GitHub Packages](https://github.com/features/packages) container registry [EntireTwix/CCash](https://github.com/EntireTwix/CCash/packages/851105).

It can be run with docker like so:

```
docker run -itp 443:443 -v ccashconfig:/ccash/config -e ADMIN_A=<admin-username> -e SAVE_FREQ=<in minutes> expandsys/ccash
```

## Build

The CCash repo provides a GitHub Workflow to build, release and publish the docker image in [.github/workflows/build.yaml](https://github.com/EntireTwix/CCash/blob/main/.github/workflows/build.yaml) to the GitHub Packages container registry.

You can build and publish your own images using this workflow by forking [EntireTwix/CCash](https://github.com/EntireTwix/CCash).

## Deploy

You can deploy this docker image to be run on a remote machine in a few steps. In this case we are using [Debian OS](https://www.debian.org/) running on the [Linode](https://www.linode.com/) cloud provider, but most OS and cloud providers will work, assuming the machine can run an SSH server and Docker.

Similarly, the CCash repo also provides a GitHub Workflow to deploy the latest docker image to a remote machine in [.github/workflows/deploy.yaml](https://github.com/EntireTwix/CCash/blob/main/.github/workflows/deploy.yaml).

### Configure the machine

1. Create a machine using your chosen cloud provider
1. Configure DNS to point your chosen domain name to the machines IP address. _(Without this, TLS/SSL will not work)_
1. Create an SSH key-pair by running `ssh-keygen` locally. Make sure you **don't** set a password
1. Add the `*.pub` public key to the servers `~/.ssh/authorized_keys` file
1. Install Docker Engine on the remote machine following [official docs](https://docs.docker.com/engine/install/)
1. Generate SSL/TLS certificate (Using [certbot](https://certbot.eff.org/lets-encrypt/debianbuster-other) is recommended)
1. Configure [GitHub secrets](https://docs.github.com/en/actions/reference/encrypted-secrets) for the repo
  * `CCASH_SSH_KEY` - The private key _(not `*.pub`)_ created earlier
  * `CCASH_DOMAIN` - The domain name pointing to the remote machine
  * `CCASH_CONFIG_JSON` - A config.json file that will be written every deploy _(https config cert path should be `/ccash/cert` and key path should be `/ccash/key`)_
  * `CCASH_USERS_JSON` - A users.json file that will be written only on first deploy
  * `CCASH_ADMIN_PASSWORD` - A CCash server admin account password
  * `CCASH_SAVE_FREQUENCY` - A number representing the frequency to save to users.json (in minutes)
  * `CCASH_THREAD_COUNT` - A number representing the number of threads to use
  * `CCASH_TLS_CERT_PATH` - The path to the TLS/SSL certificate on the host machine
  * `CCASH_TLS_KEY_PATH` - The path to the TLS/SSL key on the host machine
  * `CCASH_DEPLOY_TOKEN` - A [GitHub personal access token](https://docs.github.com/en/github/authenticating-to-github/keeping-your-account-and-data-secure/creating-a-personal-access-token), used to trigger the deploy workflow automatically when the build workflow is successful. _(Leave empty to disable automatic deploys.)_

You are now ready to run the "Deploy" workflow mentioned above. This workflow will SSH in to the `CCASH_DOMAIN` machine, using the `CCASH_SSH_KEY` and `docker run` the latest `entiretwix/ccash` image, binding to port 80 and 443, setting the appropriate volumes and environment variables.

Run `curl https://$CCASH_DOMAIN/BankF/ping` to verify that the server has been deployed correctly.
