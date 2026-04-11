CC = cc
CFLAGS = -Wall -Wextra -Werror -std=c11
TARGET = packet_parser_demo

.PHONY: all clean run

all: $(TARGET)

$(TARGET): main.c packet_parser.c
	$(CC) $(CFLAGS) -o $@ main.c packet_parser.c

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
