#include <pthread.h>
#include <stdio.h>

enum { N = 10 };

void *thr_func(void *ptr)
{
    printf("%d\n", *(int*) ptr);
    return NULL;
}

int main()
{
    pthread_t ids[N];

    int i;
    for (i = 0; i < N; ++i) {
        // передается адрес локальной переменной,
        // переменная модифицируется в main
        // и читается в thr_func
        // это Data Race - Undefined Behavior
        pthread_create(&ids[i], NULL, thr_func, &i);
    }

    for (int i = 0; i < N; ++i) {
        pthread_join(ids[i], NULL);
    }
}
