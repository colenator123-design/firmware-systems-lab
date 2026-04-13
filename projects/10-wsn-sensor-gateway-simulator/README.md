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
- retry on/off scenario comparison
- gateway-side packet accounting
- simple battery drain tracking
- experiment result generation

## Build

```sh
cd /Users/caita/firmware-systems-lab/projects/10-wsn-sensor-gateway-simulator
make
```

## Run

```sh
./wsn_gateway_demo
```

The simulator writes:

```sh
results/scenario_metrics.csv
```

## Experiment Snapshot

| Scenario | Delivery Ratio | Avg Battery |
| --- | ---: | ---: |
| low_loss_no_retry | 91.7% | 90.4% |
| medium_loss_no_retry | 80.6% | 90.4% |
| high_loss_no_retry | 66.7% | 90.4% |
| medium_loss_with_retry | 91.7% | 89.9% |
| high_loss_with_retry | 91.7% | 89.6% |

Key finding:

- retry recovers delivery ratio under medium and high loss
- retry improves reliability but consumes slightly more battery

## Files

- `main.c`: scenario runner, node simulation, retry logic, gateway accounting
- `docs/ARCHITECTURE.md`: system view
- `docs/EXPERIMENTS.md`: experiment setup and chart template
- `results/scenario_metrics.csv`: generated experiment metrics

## Good Next Upgrades

1. add per-node reporting intervals
2. add gateway-side anomaly detection
3. add variable node counts from config
4. add latency metrics
