#include <stdio.h>
#include <stdlib.h>

#define NODE_COUNT 3
#define TICKS 10

typedef struct {
    int node_id;
    float battery;
    float last_value;
    unsigned int sent_packets;
    unsigned int dropped_packets;
} sensor_node_t;

typedef struct {
    unsigned int received_packets;
    unsigned int missing_packets;
} gateway_t;

static float generate_sensor_value(int node_id, int tick)
{
    return 20.0f + (float)(node_id * 5) + (float)(tick % 4) * 0.7f;
}

static int should_drop_packet(int node_id, int tick)
{
    return ((node_id + tick) % 5) == 0;
}

static void send_sensor_report(sensor_node_t *node, gateway_t *gateway, int tick)
{
    node->last_value = generate_sensor_value(node->node_id, tick);
    node->battery -= 0.8f;

    if (should_drop_packet(node->node_id, tick)) {
        node->dropped_packets++;
        gateway->missing_packets++;
        printf("[tick %d] node-%d report dropped value=%.2f battery=%.1f%%\n", tick,
               node->node_id, node->last_value, node->battery);
        return;
    }

    node->sent_packets++;
    gateway->received_packets++;
    printf("[tick %d] node-%d -> gateway value=%.2f battery=%.1f%%\n", tick,
           node->node_id, node->last_value, node->battery);
}

static void print_summary(const sensor_node_t nodes[], const gateway_t *gateway)
{
    printf("\n=== summary ===\n");
    printf("gateway received=%u missing=%u\n", gateway->received_packets,
           gateway->missing_packets);

    for (int i = 0; i < NODE_COUNT; i++) {
        printf("node-%d sent=%u dropped=%u battery=%.1f%% last=%.2f\n", nodes[i].node_id,
               nodes[i].sent_packets, nodes[i].dropped_packets, nodes[i].battery,
               nodes[i].last_value);
    }
}

int main(void)
{
    sensor_node_t nodes[NODE_COUNT];
    gateway_t gateway = {0, 0};

    for (int i = 0; i < NODE_COUNT; i++) {
        nodes[i].node_id = i + 1;
        nodes[i].battery = 100.0f;
        nodes[i].last_value = 0.0f;
        nodes[i].sent_packets = 0;
        nodes[i].dropped_packets = 0;
    }

    for (int tick = 1; tick <= TICKS; tick++) {
        for (int i = 0; i < NODE_COUNT; i++) {
            if (nodes[i].battery > 0.0f) {
                send_sensor_report(&nodes[i], &gateway, tick);
            }
        }
    }

    print_summary(nodes, &gateway);
    return EXIT_SUCCESS;
}
