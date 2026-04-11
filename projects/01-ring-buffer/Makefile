CC = cc
CFLAGS = -Wall -Wextra -Werror -std=c11
TARGET = ring_buffer_demo

.PHONY: all clean run

all: $(TARGET)

$(TARGET): main.c ring_buffer.c
	$(CC) $(CFLAGS) -o $@ main.c ring_buffer.c

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
