CC = cc
CFLAGS = -Wall -Wextra -Werror -std=c11
TARGET = dns_client

.PHONY: all clean run

all: $(TARGET)

$(TARGET): client.c
	$(CC) $(CFLAGS) -o $@ $<

run: $(TARGET)
	./$(TARGET) 8.8.8.8 example.com

clean:
	rm -f $(TARGET)
