# Architecture

```mermaid
flowchart LR
    A[Sensor Node 1] --> D[Gateway]
    B[Sensor Node 2] --> D
    C[Sensor Node 3] --> D
    A --> E[Battery Drain]
    B --> E
    C --> E
    D --> F[Delivery Metrics]
```

## Key Ideas

- multiple nodes periodically report data
- a gateway receives or misses reports
- packet loss can be simulated by scenario
- retry can be turned on or off
- metrics summarize delivery ratio and battery impact
