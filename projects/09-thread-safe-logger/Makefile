CC = cc
CFLAGS = -Wall -Wextra -Werror -std=c11
LDFLAGS = -pthread
TARGET = thread_safe_logger_demo

.PHONY: all clean run

all: $(TARGET)

$(TARGET): main.c logger.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ main.c logger.c

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
