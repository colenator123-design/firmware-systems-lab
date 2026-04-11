# Firmware Systems Lab

This repository is a firmware-focused C practice portfolio built around memory,
buffers, protocol parsing, synchronization, UDP, and low-level systems concepts.

It is designed to complement a higher-level TCP/IP networking portfolio by
focusing on the kind of fundamentals that show up in firmware, embedded
software, and low-level systems interviews.

## Architecture Overview

```mermaid
flowchart LR
    A[01 Ring Buffer] --> B[02 UART-Style Parser]
    B --> C[03 Producer Consumer]
    C --> D[04 Packet Parser]
    D --> E[05 UDP Echo]
    E --> F[06 DNS Client]
    F --> G[07 Reliable UDP Demo]
```

## Firmware Data Flow Themes

```mermaid
flowchart LR
    A[Raw Bytes] --> B[Buffer]
    B --> C[Parser]
    C --> D[Command or Packet Fields]
    D --> E[State or Protocol Logic]
```

## What This Repo Covers

- ring buffer design
- byte stream parsing
- producer-consumer synchronization
- raw packet field extraction
- UDP client/server behavior
- DNS packet construction and parsing
- basic reliability ideas built on top of UDP

## Project Roadmap

### 01. Ring Buffer

Path:

- `projects/01-ring-buffer`

Concepts:

- fixed-capacity storage
- head/tail index movement
- wrap-around behavior
- full vs empty handling

### 02. UART-Style Parser

Path:

- `projects/02-uart-style-parser`

Concepts:

- byte-by-byte input
- line-based command parsing
- ring buffer plus parser flow

### 03. Producer Consumer

Path:

- `projects/03-producer-consumer`

Concepts:

- `pthread`
- `mutex`
- `condition variable`
- synchronized queue access

### 04. Packet Parser

Path:

- `projects/04-packet-parser`

Concepts:

- raw byte buffers
- big-endian field parsing
- IPv4 header structure

### 05. UDP Echo

Path:

- `projects/05-udp-echo`

Concepts:

- `SOCK_DGRAM`
- `recvfrom()` / `sendto()`
- connectionless communication

### 06. DNS Client

Path:

- `projects/06-dns-client`

Concepts:

- DNS query packet layout
- UDP transport
- binary protocol parsing
- `A` record extraction

### 07. Reliable UDP Demo

Path:

- `projects/07-reliable-udp-demo`

Concepts:

- sequence number
- ACK
- timeout and retry
- stop-and-wait reliability

## Suggested Reading Order

1. `projects/01-ring-buffer`
2. `projects/02-uart-style-parser`
3. `projects/03-producer-consumer`
4. `projects/04-packet-parser`
5. `projects/05-udp-echo`
6. `projects/06-dns-client`
7. `projects/07-reliable-udp-demo`

## Why This Repo Exists

This repo is meant to strengthen firmware-style foundations:

- buffer management
- memory layout thinking
- state-driven logic
- embedded-friendly data structures
- protocol parsing
- systems programming basics

## Good Resume Keywords

- C
- Firmware
- Embedded Systems
- Ring Buffer
- UART Parser
- Packet Parsing
- UDP
- DNS
- Producer Consumer
- `pthread`
