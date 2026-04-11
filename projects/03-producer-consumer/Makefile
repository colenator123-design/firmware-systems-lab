CC = cc
CFLAGS = -Wall -Wextra -Werror -std=c11
LDFLAGS = -pthread
TARGET = producer_consumer_demo

.PHONY: all clean run

all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ main.c

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
