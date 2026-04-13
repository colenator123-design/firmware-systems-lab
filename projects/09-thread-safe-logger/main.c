#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#include "logger.h"

typedef struct {
    const char *tag;
    int count;
} worker_arg_t;

static void *worker_thread(void *arg)
{
    worker_arg_t *worker = (worker_arg_t *)arg;
    char message[64];

    for (int i = 1; i <= worker->count; i++) {
        snprintf(message, sizeof(message), "message %d", i);
        logger_log(worker->tag, message);
        usleep(50000);
    }

    return NULL;
}

int main(void)
{
    if (logger_init(stdout) != 0) {
        fprintf(stderr, "logger_init failed\n");
        return 1;
    }

    pthread_t thread1;
    pthread_t thread2;
    worker_arg_t worker1 = {"worker-A", 5};
    worker_arg_t worker2 = {"worker-B", 5};

    if (pthread_create(&thread1, NULL, worker_thread, &worker1) != 0) {
        fprintf(stderr, "pthread_create thread1 failed\n");
        return 1;
    }

    if (pthread_create(&thread2, NULL, worker_thread, &worker2) != 0) {
        fprintf(stderr, "pthread_create thread2 failed\n");
        pthread_join(thread1, NULL);
        return 1;
    }

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    logger_shutdown();
    return 0;
}
