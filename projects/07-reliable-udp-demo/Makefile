CC = cc
CFLAGS = -Wall -Wextra -Werror -std=c11
TARGETS = reliable_udp_server reliable_udp_client

.PHONY: all clean

all: $(TARGETS)

reliable_udp_server: server.c
	$(CC) $(CFLAGS) -o $@ $<

reliable_udp_client: client.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)
