#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dlfcn.h>

typedef struct Random Random;

typedef struct RandomOps {
    int (*next)(Random *);
    Random *(*destroy)(Random *);
} RandomOps;



typedef struct Random {
    int count;
    RandomOps *ops;
} Random;



typedef struct RandomPseudo {
    Random base;
} RandomPseudo;

typedef struct RandomFile {
    Random base;
    int fd;
} RandomFile;

int nextPseudo(Random *r_) {
    r_->count++;
    return rand();
}

int nextFile(Random *r_) {
    r_->count++;
    RandomFile *r = (RandomFile *)r_;
    int res;
    read(r->fd, &res, sizeof(res));
    return res;
}

RandomFile *destroyFile(RandomFile *r) {
    close(r->fd);
    free(r);
    return NULL;
}

RandomPseudo *destroyPseudo(RandomPseudo *r) {
    free(r);
    return  NULL;
}

RandomOps randomFileOps = {nextFile, destroyFile};
RandomOps randomPseudoOps = {nextPseudo, destroyPseudo};

RandomFile *initFile(const char *filename) {
    RandomFile *res = malloc(sizeof(*res));
    res->base.ops = &randomFileOps;
    res->base.count = 0;
    res->fd = open(filename, O_RDONLY);
    return res;
}

RandomPseudo *initPseudo(int t) {
    RandomPseudo *res = malloc(sizeof(*res));
    res->base.ops = &randomPseudoOps;
    res->base.count = 0;
    srand(t);
    return res;
}

int f(Random *r) {
    r->ops->next(r);
}

int main(void) {
    Random *r1 = (Random *)initPseudo(123);
    Random *r2 = (Random *)initFile("/dev/random");
    printf("%d\n", r1->ops->next(r1));
    printf("%d\n", r2->ops->next(r2));
    f(r1); f(r2); 
    r1 = r1->ops->destroy(r1);
    printf("\t%d\n", r2->count); 
    void *dl_handle = dlopen("./lib.so",RTLD_NOW);
    perror(dlerror());
    int (*next)(Random *) = dlsym(dl_handle, "next42");
    perror(dlerror());
    printf("%d\n", next(NULL));
    dlclose(dl_handle);
}
