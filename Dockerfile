FROM debian:latest

WORKDIR /ccash

RUN apt update && apt -y install build-essential g++ cmake protobuf-compiler libjsoncpp-dev uuid-dev openssl libssl-dev zlib1g-dev

COPY . .

RUN mkdir build

WORKDIR /ccash/build

RUN cmake ..
RUN make -j$(nproc)

ENTRYPOINT ["/ccash/build/bank"]
