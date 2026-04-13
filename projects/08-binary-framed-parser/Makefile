CC = cc
CFLAGS = -Wall -Wextra -Werror -std=c11
TARGET = binary_framed_parser_demo

.PHONY: all clean run

all: $(TARGET)

$(TARGET): main.c framed_parser.c ../01-ring-buffer/ring_buffer.c
	$(CC) $(CFLAGS) -I../01-ring-buffer -o $@ main.c framed_parser.c ../01-ring-buffer/ring_buffer.c

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
