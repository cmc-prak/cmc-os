CC = gcc
CFLAGS = -g -Wall -Werror -std=gnu11 -m32
LDFLAGS = -g -m32

TARGETS = m rand_stdlib.so rand_urandom.so
CFILES = m.c r.c r2.c
HFILES = $(wildcard *.h)
OFILES = $(CFILES:.c=.o)

all : $(TARGETS)

m : m.o
	$(CC) $(LDFLAGS) $^ -o$@ -ldl

rand_stdlib.so : r.o
	$(CC) $(LDFLAGS) -shared -fPIC -DPIC $^ -o$@

rand_urandom.so : r2.o
	$(CC) $(LDFLAGS) -shared -fPIC -DPIC $^ -o$@

r.o : r.c
	$(CC) $(CFLAGS) -fPIC -DPIC -c r.c -o$@

r2.o : r2.c
	$(CC) $(CFLAGS) -fPIC -DPIC -c r2.c -o$@

include deps.make

deps.make : $(CFILES) $(HFILES)
	$(CC) -MM $(CFILES) > deps.make

clean :
	-rm -f $(TARGETS) *.o deps.make
