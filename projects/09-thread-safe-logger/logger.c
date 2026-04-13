#include "logger.h"

#include <pthread.h>
#include <stdio.h>
#include <time.h>

static pthread_mutex_t logger_mutex = PTHREAD_MUTEX_INITIALIZER;
static FILE *logger_stream = NULL;

int logger_init(FILE *stream)
{
    if (stream == NULL) {
        return -1;
    }

    logger_stream = stream;
    return 0;
}

void logger_log(const char *tag, const char *message)
{
    if (logger_stream == NULL || tag == NULL || message == NULL) {
        return;
    }

    pthread_mutex_lock(&logger_mutex);

    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    if (tm_info != NULL) {
        fprintf(logger_stream, "[%02d:%02d:%02d] [%s] %s\n", tm_info->tm_hour,
                tm_info->tm_min, tm_info->tm_sec, tag, message);
    } else {
        fprintf(logger_stream, "[time-error] [%s] %s\n", tag, message);
    }

    fflush(logger_stream);
    pthread_mutex_unlock(&logger_mutex);
}

void logger_shutdown(void)
{
    pthread_mutex_lock(&logger_mutex);
    logger_stream = NULL;
    pthread_mutex_unlock(&logger_mutex);
}
