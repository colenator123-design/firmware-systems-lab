# Resume Notes

This file is a job-search helper for describing the firmware-focused projects in
this repository.

## Best Projects To Highlight

### 1. Ring Buffer

Path:

- `projects/01-ring-buffer`

Why it is strong:

- core embedded data structure
- shows buffer indexing and wrap-around logic
- easy to discuss in interviews

### 2. Producer Consumer

Path:

- `projects/03-producer-consumer`

Why it is strong:

- shows thread synchronization
- shows queue coordination
- connects firmware-style buffering with OS concepts

### 3. DNS Client

Path:

- `projects/06-dns-client`

Why it is strong:

- shows real protocol packet construction
- shows UDP and binary parsing
- looks more low-level than a generic socket demo

### 4. Reliable UDP Demo

Path:

- `projects/07-reliable-udp-demo`

Why it is strong:

- shows sequence / ack / retry thinking
- demonstrates understanding of what UDP does not guarantee

### 5. WSN Sensor Gateway Simulator

Path:

- `projects/10-wsn-sensor-gateway-simulator`

Why it is strong:

- aligns well with sensing and reliable networking themes
- shows multi-node reporting and gateway collection flow
- adds packet loss and battery-awareness to the project story

### 6. Binary Framed Parser

Path:

- `projects/08-binary-framed-parser`

Why it is strong:

- shows binary protocol parsing rather than only text commands
- demonstrates state-machine parsing
- connects well to firmware communication scenarios

### 7. Thread-Safe Logger

Path:

- `projects/09-thread-safe-logger`

Why it is strong:

- shows reusable systems-style component design
- demonstrates synchronization around a shared resource
- easy to discuss in OS / concurrency interviews

## Chinese Resume Version

以 C 語言實作一系列偏韌體與底層系統的練習專案，涵蓋 ring buffer、
UART-style parser、producer-consumer synchronization、IPv4 packet parsing、
UDP echo、DNS client、基於 UDP 的簡易可靠傳輸機制，以及無線感測網路風格的
sensor-to-gateway 模擬。過程中實作固定大小 buffer 管理、byte stream
parsing、thread synchronization、binary protocol field extraction、UDP 傳輸、
簡化版 seq/ack/retry 流程，以及節點回報、封包遺失與 gateway 資料彙整邏輯。
另外也包含 binary framed protocol parser 與 thread-safe logger，補強狀態機解析與共享資源同步能力。

## English Resume Version

Built a firmware-oriented C project portfolio covering a ring buffer,
UART-style parser, producer-consumer synchronization, IPv4 packet parsing, UDP
echo, a DNS client, a simple reliable-UDP mechanism, and a wireless sensor
network style sensor-to-gateway simulator. Implemented fixed-size buffer
management, byte stream parsing, thread synchronization, binary protocol field
extraction, UDP communication, simplified sequence/ack/retry logic, and
multi-node reporting with packet loss simulation. Also includes a binary framed
protocol parser and a thread-safe logger to strengthen state-machine parsing and
shared-resource synchronization skills.

## Short Resume Bullet Variants

- Built firmware-oriented C projects covering ring buffers, UART-style parsing,
  synchronized queues, packet parsing, UDP, and DNS.
- Practiced low-level systems programming through C projects involving buffer
  management, binary protocol parsing, and thread synchronization.
- Implemented small firmware-style and network-protocol exercises in C,
  including a reliable UDP demo with sequence, ACK, timeout, and retry logic.
- Simulated a wireless sensor network style node-to-gateway reporting flow with
  packet loss and simple reliability metrics.
- Built a binary framed protocol parser with state-machine logic, payload length
  validation, and checksum checking.
- Implemented a thread-safe logger in C using `pthread_mutex_t` for safe
  multi-threaded logging.

## Interview Talking Points

### What this repository is

This is a firmware and low-level systems practice repository focused on the kind
of foundations that support embedded networking and systems programming:
buffers, parsers, synchronization, raw bytes, UDP, and lightweight protocol
logic.

### What I learned

- how ring buffers work internally
- how byte streams become commands
- how to coordinate producer and consumer threads safely
- how to parse protocol fields from raw network data
- how UDP differs from TCP
- how reliability ideas like ACK and retransmission can be built on top of UDP
- how sensing nodes and a gateway can be modeled in a simple reliable networking flow
- how binary framed protocols can be parsed safely with a state machine
- how to protect a shared utility with synchronization primitives

## Suggested Projects To Put On Resume First

1. `projects/01-ring-buffer`
2. `projects/03-producer-consumer`
3. `projects/06-dns-client`
4. `projects/07-reliable-udp-demo`
5. `projects/10-wsn-sensor-gateway-simulator`
6. `projects/08-binary-framed-parser`
7. `projects/09-thread-safe-logger`
