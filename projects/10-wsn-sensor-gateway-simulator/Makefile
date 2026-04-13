CC = cc
CFLAGS = -Wall -Wextra -Werror -std=c11
TARGET = wsn_gateway_demo

.PHONY: all clean run

all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) -o $@ $<

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
