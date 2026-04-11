CC = cc
CFLAGS = -Wall -Wextra -Werror -std=c11
TARGET = uart_parser_demo

.PHONY: all clean run

all: $(TARGET)

$(TARGET): main.c uart_parser.c ../01-ring-buffer/ring_buffer.c
	$(CC) $(CFLAGS) -I../01-ring-buffer -o $@ main.c uart_parser.c ../01-ring-buffer/ring_buffer.c

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
