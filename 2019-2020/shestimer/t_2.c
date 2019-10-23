#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

struct RandomOps {
	void (*destruct)(struct Random*);
	unsigned (*next)(struct Random*);
};


struct Random {
	int count;
	struct RandomOps *ops;
};

struct RandomDev {
	struct Random r;
	int fd;
};

struct RandomST {
	struct Random r;
};

void destructDev(struct Random *w) {
	struct RandomDev *r = w;
	if (r->fd != -1) {
		close(r->fd);
	}
	free(r);
}

void destructST(struct Random *w) {
	struct RandomST *r = w;
	free(r);
}

unsigned next_r(struct Random* w) {
	unsigned x;
	read(r->fd, &x, sizeof(x));
	return x;
}

unsigned next_empty(struct Random* w) {
	struct RandomST *r = w;
	return rand();
}

struct RandomOps dev = { destructDev, next_r };
struct RandomOps st = { destructST, next_empty };

unsigned next(struct Random *r) {
	++r->count;
	return r->ops->next(r);
}

struct Random *create_random(const char* name) {
	if (name && *name) {
		struct RandomDev *r;
		r = calloc(1, sizeof(struct RandomDev));
		int fd = open(name, O_RDONLY);
		r->fd = fd;
		r->r.ops = &dev;
		return r;
	} else {
		struct RandomST* r;
		r = calloc(1, sizeof(struct RandomST));
		r->r.ops = &st;
		return r;
	}
}

int main(int argc, char *argv[]) {
	srand(time(0));
	const int maxt = 10;
	struct Random *r = create_random(argv[1]);
	for (int i = 0; i < maxt; ++i) {
		printf("%u\n", next(r)  % 100);
	}
	r->ops->destruct(r);
	return 0;
}
