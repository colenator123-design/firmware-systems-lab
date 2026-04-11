# 03 Producer Consumer

This project is a classic producer-consumer demo built with `pthread`,
`mutex`, and `condition variable`.

## Why This Matters

This pattern shows up in both systems and firmware-adjacent work:

- interrupt or I/O side produces data
- worker side consumes data
- shared queues need synchronization

## What This Project Covers

- threads with `pthread_create`
- mutual exclusion with `pthread_mutex_t`
- waiting and signaling with `pthread_cond_t`
- fixed-size queue behavior
- coordination between producer and consumer threads

## Build

```sh
cd /Users/caita/firmware-systems-lab/projects/03-producer-consumer
make
```

## Run

```sh
./producer_consumer_demo
```

## Files

- `main.c`: queue, producer thread, consumer thread, and synchronization logic
