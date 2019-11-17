CC = gcc
CFLAGS = -g -Wall -Werror -std=gnu11 -fsanitize=undefined
LDFLAGS = -fsanitize=undefined -g

TARGETS = m
CFILES = m.c r2.c
HFILES = $(wildcard *.h)
OFILES = $(CFILES:.c=.o)

all : $(TARGETS)

m : $(OFILES)

include deps.make

deps.make : $(CFILES) $(HFILES)
	$(CC) -MM $(CFILES) > deps.make

clean :
	-rm -f $(TARGETS) *.o deps.make
