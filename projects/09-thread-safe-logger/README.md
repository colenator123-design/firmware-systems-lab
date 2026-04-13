# 09 Thread-Safe Logger

This project is a small thread-safe logger written in C with `pthread_mutex_t`.

## Why This Matters

In systems and firmware-adjacent software, shared logging is a common utility.
If multiple threads write at the same time, logs can become interleaved or
corrupted without synchronization.

## What This Project Covers

- shared resource protection with a mutex
- simple reusable logger API
- multiple worker threads writing safely to one output stream
- timestamped log output

## Build

```sh
cd /Users/caita/firmware-systems-lab/projects/09-thread-safe-logger
make
```

## Run

```sh
./thread_safe_logger_demo
```

## Files

- `logger.h`: logger API
- `logger.c`: synchronized logging implementation
- `main.c`: demo with two worker threads
