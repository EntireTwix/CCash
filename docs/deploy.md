# Deploying CCash

CCash can deployed to a remote machine pretty simply.

A pre-built docker image is supplied in the repos [GitHub Packages](https://github.com/features/packages) container registry [EntireTwix/CCash](https://github.com/EntireTwix/CCash/packages/851105).

It can be run with docker like so:

```
docker pull docker.pkg.github.com/entiretwix/ccash/ccash:latest
```

## Build

The CCash repo provides a GitHub Workflow to build, release and publish the docker image in [.github/workflows/build.yaml](https://github.com/EntireTwix/CCash/blob/main/.github/workflows/build.yaml) to the GitHub Packages container registry.

You can build and publish your own images using this workflow by forking [EntireTwix/CCash](https://github.com/EntireTwix/CCash).

## Deploy

You can deploy this docker image to be run on a remote machine in a few steps. In this case we are using [Debian OS](https://www.debian.org/) running on the [Linode](https://www.linode.com/) cloud provider, but most OS and cloud providers will work, assuming the machine can run an SSH server and Docker.

Similarly, the CCash repo also provides a GitHub Workflow to deploy the latest docker image to a remote machine in [.github/workflows/deploy.yaml](https://github.com/EntireTwix/CCash/blob/main/.github/workflows/deploy.yaml).

### Configure the machine

* Create a machine using your chosen cloud provider.
* Configure DNS to point your chosen domain name to the machines IP address. _(Without this, TLS/SSL will not work)_.
* Create an SSH key-pair by running `ssh-keygen` locally. Make sure you **don't** set a password.
* Add the `*.pub` public key to the servers `~/.ssh/authorized_keys` file.
* Install Docker Engine following [official docs](https://docs.docker.com/engine/install/).
* Add the private key _(not `*.pub`)_ to a [GitHub secret](https://docs.github.com/en/actions/reference/encrypted-secrets) named `CCASH_SSH_KEY` in your CCash repo.
* Add the domain name pointing to the remote machine to a GitHub secret named `CCASH_DOMAIN`.

You are now ready to run the "Deploy" workflow mentioned above. This workflow will SSH in to the `CCASH_DOMAIN` machine, using the `CCASH_SSH_KEY` and `docker run` the latest `entiretwix/ccash` image, binding to port 80 and 443. It will also handle the creation of a TLS/SSL certificate for you.

Visiting `https://$SSH_TARGET/BankF/ping` in your browser should verify that it has been deployed correctly.
