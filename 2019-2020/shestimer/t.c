#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

struct Random {
	int fd;

	
	unsigned (*next)(struct Random*);
};

unsigned next_r(struct Random* r) {
	unsigned x;
	read(r->fd, &x, sizeof(x));
	return x;
}

int main(void) {
	srand(time(0));
	const int maxt = 10;

	struct Random r;
	int fd = open("/dev/random", O_RDONLY);
	r.next = next_r;
	r.fd = fd;
	for (int i = 0; i < maxt; ++i) {
		printf("%u\n", r.next(&r)  % 100);
	}
	close(fd);
	return 0;
}
