# 10 WSN Sensor Gateway Simulator

This project is a simple wireless sensor network style simulator written in C.

## Why This Matters

It connects several ideas that match reliable networking and sensing systems:

- multiple sensor nodes
- a gateway collecting reports
- packet loss simulation
- battery-aware node behavior
- basic reliability metrics

## What This Project Covers

- periodic sensor reporting
- node-to-gateway communication flow
- dropped packet simulation
- gateway-side packet accounting
- simple battery drain tracking

## Build

```sh
cd /Users/caita/firmware-systems-lab/projects/10-wsn-sensor-gateway-simulator
make
```

## Run

```sh
./wsn_gateway_demo
```

## Files

- `main.c`: sensor node simulation, packet loss logic, gateway accounting

## Good Next Upgrades

1. add retransmission logic
2. add per-node reporting intervals
3. add gateway-side anomaly detection
4. export metrics to a log file
