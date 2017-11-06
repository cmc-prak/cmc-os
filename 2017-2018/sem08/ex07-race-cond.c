#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

enum { N = 10 };

void *thr_func(void *ptr)
{
    int c;
    // пара чтение символа-запись символа не атомарна - Race Condition
    while ((c = getchar()) != EOF) {
        putchar(c);
    }
    return NULL;
}

int main()
{
    pthread_t ids[N];

    pthread_attr_t attr;

    pthread_attr_init(&attr);

    pthread_attr_setstacksize(&attr, 16384);
    pthread_attr_setguardsize(&attr, 0);

    int i;
    for (i = 0; i < N; ++i) {
        int err;
        err = pthread_create(&ids[i], &attr, thr_func, (void*) (intptr_t) i);
        if (err) {
            fprintf(stderr, "error: %d %s\n", i, strerror(err));
            pause();
        }
    }

    pthread_attr_destroy(&attr);

    for (int i = 0; i < N; ++i) {
        pthread_join(ids[i], NULL);
    }
}
