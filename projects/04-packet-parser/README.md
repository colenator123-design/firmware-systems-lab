# 04 Packet Parser

This project is a small IPv4 header parser written in C.

## Why This Matters

For lower-level networking and firmware-adjacent work, it is important to be
comfortable reading raw bytes and turning them into meaningful protocol fields.

## What This Project Covers

- parsing raw bytes from a buffer
- big-endian field extraction
- IPv4 header field layout
- converting integer IP fields into dotted-decimal output

## Build

```sh
cd /Users/caita/firmware-systems-lab/projects/04-packet-parser
make
```

## Run

```sh
./packet_parser_demo
```

## Files

- `packet_parser.h`: header structure and API
- `packet_parser.c`: parsing and printing logic
- `main.c`: demo using a sample IPv4 header
