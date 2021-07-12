#!/bin/bash


openssl genrsa -out server.pass.key 2048
openssl rsa -in server.pass.key -out /CCash/config/key.key
rm server.pass.key
openssl req -new -key /CCash/config/key.key -out server.csr \
    -subj "/C=US/ST=CCashland/L=NEW CCASH/O=CCash/OU=Devs/CN=localhost"
openssl x509 -req -days 365 -in server.csr -signkey /CCash/config/key.key -out /CCash/config/cert.cert
