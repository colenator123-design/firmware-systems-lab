#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>

int logger_init(FILE *stream);
void logger_log(const char *tag, const char *message);
void logger_shutdown(void);

#endif
