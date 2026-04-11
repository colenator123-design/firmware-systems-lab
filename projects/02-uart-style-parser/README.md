# 02 UART-Style Parser

This project builds on the ring buffer and simulates a simple UART-style command
parser.

## Why This Matters

Firmware often receives data one byte at a time:

- UART commands
- AT commands
- console interfaces
- device control messages

A ring buffer stores incoming bytes, and a parser turns them into complete
commands.

## What This Project Covers

- byte-by-byte input handling
- buffering incoming data
- detecting command boundaries
- ignoring `\r` and ending on `\n`
- returning complete commands one at a time

## Data Flow

```mermaid
flowchart LR
    A[Incoming UART bytes] --> B[Ring Buffer]
    B --> C[Parser State]
    C --> D[Complete Command]
    D --> E[Application Logic]
```

## Build

```sh
cd /Users/caita/firmware-systems-lab/projects/02-uart-style-parser
make
```

## Run

```sh
./uart_parser_demo
```

## Files

- `uart_parser.h`: parser API
- `uart_parser.c`: parser logic
- `main.c`: demo that feeds command streams into the parser
