CC = cc
CFLAGS = -Wall -Wextra -Werror -std=c11
TARGETS = udp_echo_server udp_echo_client

.PHONY: all clean

all: $(TARGETS)

udp_echo_server: server.c
	$(CC) $(CFLAGS) -o $@ $<

udp_echo_client: client.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)
