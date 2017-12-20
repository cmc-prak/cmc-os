CC = gcc
CFLAGS = -Wall -O2 -Werror -std=gnu11

TARGETS = client server

all : $(TARGETS)

client : client.c

server : server.c

.PHONY : clean

clean :
	-rm -f $(TARGETS)
