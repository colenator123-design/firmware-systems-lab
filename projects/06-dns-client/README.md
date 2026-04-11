# 06 DNS Client

This project is a minimal DNS client written in C over UDP.

## Why This Matters

DNS is a real protocol that combines:

- UDP transport
- binary packet layout
- network byte order
- protocol field parsing

It is a strong practice project for lower-level networking work.

## What This Project Covers

- building a DNS query packet by hand
- sending the query over UDP
- receiving and parsing a DNS response
- extracting IPv4 `A` records

## Build

```sh
cd /Users/caita/firmware-systems-lab/projects/06-dns-client
make
```

## Run

Example:

```sh
./dns_client 8.8.8.8 example.com
```

## Notes

This version focuses on:

- one question
- IPv4 `A` record answers
- simple response parsing

It does not yet support every DNS feature.
