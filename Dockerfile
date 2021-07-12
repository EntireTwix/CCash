FROM alpine:3.11

WORKDIR /

RUN apk update && apk add git cmake g++ make protobuf jsoncpp-dev openssl libressl-dev zlib-dev util-linux-dev libtool autoconf automake python3

RUN git clone --recurse-submodules https://github.com/EntireTwix/CCash.git --branch Refractor
RUN mkdir /CCash/build
WORKDIR /CCash/build
RUN cmake -DDROGON_CONFIG_LOC=\"\/CCash\/config\/config.json\" -DUSER_SAVE_LOC=\"\/CCash\/config\/users.json\" ..
RUN make -j$(nproc)

ARG ADMIN_A=admin
ARG SAVE_FREQ=2

RUN ["chmod", "+x", "/CCash/config/ssl.sh"]

CMD ["sh", "-c", "/CCash/config/ssl.sh && /CCash/build/bank ${ADMIN_A} ${SAVE_FREQ}"]
