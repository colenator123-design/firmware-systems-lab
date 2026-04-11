#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define QUEUE_SIZE 8
#define TOTAL_ITEMS 20

typedef struct {
    int data[QUEUE_SIZE];
    int head;
    int tail;
    int count;
    int producer_done;
    pthread_mutex_t mutex;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
} queue_t;

static void queue_init(queue_t *queue)
{
    queue->head = 0;
    queue->tail = 0;
    queue->count = 0;
    queue->producer_done = 0;
    pthread_mutex_init(&queue->mutex, NULL);
    pthread_cond_init(&queue->not_empty, NULL);
    pthread_cond_init(&queue->not_full, NULL);
}

static void queue_destroy(queue_t *queue)
{
    pthread_mutex_destroy(&queue->mutex);
    pthread_cond_destroy(&queue->not_empty);
    pthread_cond_destroy(&queue->not_full);
}

static void queue_push(queue_t *queue, int value)
{
    pthread_mutex_lock(&queue->mutex);

    while (queue->count == QUEUE_SIZE) {
        pthread_cond_wait(&queue->not_full, &queue->mutex);
    }

    queue->data[queue->head] = value;
    queue->head = (queue->head + 1) % QUEUE_SIZE;
    queue->count++;

    pthread_cond_signal(&queue->not_empty);
    pthread_mutex_unlock(&queue->mutex);
}

static int queue_pop(queue_t *queue, int *value)
{
    pthread_mutex_lock(&queue->mutex);

    while (queue->count == 0 && !queue->producer_done) {
        pthread_cond_wait(&queue->not_empty, &queue->mutex);
    }

    if (queue->count == 0 && queue->producer_done) {
        pthread_mutex_unlock(&queue->mutex);
        return -1;
    }

    *value = queue->data[queue->tail];
    queue->tail = (queue->tail + 1) % QUEUE_SIZE;
    queue->count--;

    pthread_cond_signal(&queue->not_full);
    pthread_mutex_unlock(&queue->mutex);
    return 0;
}

static void *producer_thread(void *arg)
{
    queue_t *queue = (queue_t *)arg;

    for (int i = 1; i <= TOTAL_ITEMS; i++) {
        queue_push(queue, i);
        printf("producer pushed %d\n", i);
        usleep(50000);
    }

    pthread_mutex_lock(&queue->mutex);
    queue->producer_done = 1;
    pthread_cond_broadcast(&queue->not_empty);
    pthread_mutex_unlock(&queue->mutex);

    return NULL;
}

static void *consumer_thread(void *arg)
{
    queue_t *queue = (queue_t *)arg;

    while (1) {
        int value = 0;
        if (queue_pop(queue, &value) != 0) {
            break;
        }
        printf("consumer popped %d\n", value);
        usleep(80000);
    }

    return NULL;
}

int main(void)
{
    queue_t queue;
    pthread_t producer;
    pthread_t consumer;

    queue_init(&queue);

    if (pthread_create(&producer, NULL, producer_thread, &queue) != 0) {
        fprintf(stderr, "pthread_create producer failed\n");
        queue_destroy(&queue);
        return 1;
    }

    if (pthread_create(&consumer, NULL, consumer_thread, &queue) != 0) {
        fprintf(stderr, "pthread_create consumer failed\n");
        pthread_join(producer, NULL);
        queue_destroy(&queue);
        return 1;
    }

    pthread_join(producer, NULL);
    pthread_join(consumer, NULL);
    queue_destroy(&queue);
    return 0;
}
