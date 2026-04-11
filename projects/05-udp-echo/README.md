# 05 UDP Echo

This project is a minimal UDP echo server/client pair written in C.

## Why This Matters

UDP is different from TCP:

- no connection handshake
- no built-in delivery guarantee
- message-oriented communication

Understanding UDP is important for networking, embedded communication, and
low-latency systems.

## What This Project Covers

- `SOCK_DGRAM`
- `recvfrom()` and `sendto()`
- UDP client/server flow
- differences between connection-oriented and connectionless communication

## Build

```sh
cd /Users/caita/firmware-systems-lab/projects/05-udp-echo
make
```

## Run

Start the server:

```sh
./udp_echo_server 9000
```

Start the client:

```sh
./udp_echo_client 127.0.0.1 9000
```
