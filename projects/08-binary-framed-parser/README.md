# 08 Binary Framed Protocol Parser

This project is a small binary framed protocol parser written in C.

## Why This Matters

Many firmware and embedded systems do not use line-based text commands. Instead,
they exchange framed binary packets with:

- start-of-frame markers
- message type fields
- payload length fields
- payload bytes
- checksums

## Frame Format

```text
+------+-------+--------+---------+----------+
| SOF  | TYPE  | LENGTH | PAYLOAD | CHECKSUM |
+------+-------+--------+---------+----------+
 1 byte 1 byte  1 byte   N bytes    1 byte
```

- `SOF` is fixed at `0xAA`
- `CHECKSUM` is a simple XOR of `TYPE`, `LENGTH`, and all payload bytes

## What This Project Covers

- state machine parsing
- binary frame recovery after noise
- payload length validation
- checksum validation
- ring buffer plus parser integration

## Build

```sh
cd /Users/caita/firmware-systems-lab/projects/08-binary-framed-parser
make
```

## Run

```sh
./binary_framed_parser_demo
```

## Files

- `framed_parser.h`: frame format and parser state definitions
- `framed_parser.c`: parser implementation
- `main.c`: demo that feeds valid frames, noise, and an invalid checksum case
