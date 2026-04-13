#include <stdio.h>
#include <stdlib.h>

#define NODE_COUNT 3
#define TICKS 12

typedef struct {
    const char *name;
    int drop_mod;
    int retry_enabled;
} scenario_t;

typedef struct {
    int node_id;
    float battery;
    float last_value;
    unsigned int successful_reports;
    unsigned int dropped_reports;
    unsigned int retry_attempts;
    unsigned int retry_successes;
} sensor_node_t;

typedef struct {
    unsigned int total_received;
    unsigned int total_dropped;
} gateway_t;

static float generate_sensor_value(int node_id, int tick)
{
    return 20.0f + (float)(node_id * 5) + (float)(tick % 4) * 0.7f;
}

static int initial_drop(const scenario_t *scenario, int node_id, int tick)
{
    return ((node_id + tick) % scenario->drop_mod) == 0;
}

static int retry_still_fails(const scenario_t *scenario, int node_id, int tick)
{
    return ((node_id * 2 + tick) % (scenario->drop_mod + 1)) == 0;
}

static void init_nodes(sensor_node_t nodes[])
{
    for (int i = 0; i < NODE_COUNT; i++) {
        nodes[i].node_id = i + 1;
        nodes[i].battery = 100.0f;
        nodes[i].last_value = 0.0f;
        nodes[i].successful_reports = 0;
        nodes[i].dropped_reports = 0;
        nodes[i].retry_attempts = 0;
        nodes[i].retry_successes = 0;
    }
}

static void send_sensor_report(sensor_node_t *node, gateway_t *gateway,
                               const scenario_t *scenario, int tick)
{
    node->last_value = generate_sensor_value(node->node_id, tick);
    node->battery -= 0.8f;

    if (!initial_drop(scenario, node->node_id, tick)) {
        node->successful_reports++;
        gateway->total_received++;
        printf("[%s][tick %d] node-%d -> gateway value=%.2f battery=%.1f%%\n",
               scenario->name, tick, node->node_id, node->last_value, node->battery);
        return;
    }

    if (scenario->retry_enabled) {
        node->retry_attempts++;
        node->battery -= 0.2f;

        if (!retry_still_fails(scenario, node->node_id, tick)) {
            node->retry_successes++;
            node->successful_reports++;
            gateway->total_received++;
            printf("[%s][tick %d] node-%d retry success value=%.2f battery=%.1f%%\n",
                   scenario->name, tick, node->node_id, node->last_value, node->battery);
            return;
        }
    }

    node->dropped_reports++;
    gateway->total_dropped++;
    printf("[%s][tick %d] node-%d report dropped value=%.2f battery=%.1f%%\n",
           scenario->name, tick, node->node_id, node->last_value, node->battery);
}

static void print_summary(const sensor_node_t nodes[], const gateway_t *gateway,
                          const scenario_t *scenario)
{
    float delivery_ratio = 0.0f;
    unsigned int total_attempted = gateway->total_received + gateway->total_dropped;
    if (total_attempted > 0) {
        delivery_ratio = (100.0f * (float)gateway->total_received) / (float)total_attempted;
    }

    printf("\n=== %s summary ===\n", scenario->name);
    printf("gateway received=%u dropped=%u delivery_ratio=%.1f%%\n",
           gateway->total_received, gateway->total_dropped, delivery_ratio);

    for (int i = 0; i < NODE_COUNT; i++) {
        printf("node-%d success=%u dropped=%u retry_attempts=%u retry_success=%u battery=%.1f%% last=%.2f\n",
               nodes[i].node_id, nodes[i].successful_reports, nodes[i].dropped_reports,
               nodes[i].retry_attempts, nodes[i].retry_successes, nodes[i].battery,
               nodes[i].last_value);
    }
}

static void append_result_csv(FILE *csv, const sensor_node_t nodes[], const gateway_t *gateway,
                              const scenario_t *scenario)
{
    unsigned int total_attempted = gateway->total_received + gateway->total_dropped;
    float delivery_ratio = 0.0f;
    float average_battery = 0.0f;
    unsigned int total_retry_attempts = 0;
    unsigned int total_retry_successes = 0;

    if (total_attempted > 0) {
        delivery_ratio = (100.0f * (float)gateway->total_received) / (float)total_attempted;
    }

    for (int i = 0; i < NODE_COUNT; i++) {
        average_battery += nodes[i].battery;
        total_retry_attempts += nodes[i].retry_attempts;
        total_retry_successes += nodes[i].retry_successes;
    }
    average_battery /= (float)NODE_COUNT;

    fprintf(csv, "%s,%u,%u,%.1f,%u,%u,%.1f\n", scenario->name, gateway->total_received,
            gateway->total_dropped, delivery_ratio, total_retry_attempts,
            total_retry_successes, average_battery);
}

static void run_scenario(const scenario_t *scenario, FILE *csv)
{
    sensor_node_t nodes[NODE_COUNT];
    gateway_t gateway = {0, 0};
    init_nodes(nodes);

    for (int tick = 1; tick <= TICKS; tick++) {
        for (int i = 0; i < NODE_COUNT; i++) {
            if (nodes[i].battery > 0.0f) {
                send_sensor_report(&nodes[i], &gateway, scenario, tick);
            }
        }
    }

    print_summary(nodes, &gateway, scenario);
    append_result_csv(csv, nodes, &gateway, scenario);
}

int main(void)
{
    const scenario_t scenarios[] = {
        {"low_loss_no_retry", 8, 0},
        {"medium_loss_no_retry", 5, 0},
        {"high_loss_no_retry", 3, 0},
        {"medium_loss_with_retry", 5, 1},
        {"high_loss_with_retry", 3, 1},
    };

    FILE *csv = fopen("results/scenario_metrics.csv", "w");
    if (csv == NULL) {
        perror("fopen");
        return EXIT_FAILURE;
    }

    fprintf(csv,
            "scenario,received,dropped,delivery_ratio,retry_attempts,retry_successes,average_battery\n");

    for (size_t i = 0; i < sizeof(scenarios) / sizeof(scenarios[0]); i++) {
        run_scenario(&scenarios[i], csv);
        printf("\n");
    }

    fclose(csv);
    printf("metrics written to results/scenario_metrics.csv\n");
    return EXIT_SUCCESS;
}
