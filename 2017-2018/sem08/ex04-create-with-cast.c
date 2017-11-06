#include <pthread.h>
#include <stdio.h>

enum { N = 10 };

void *thr_func(void *ptr)
{
    // в значении указателя передаем серийный номер нити
    // intptr_t - целый тип размера, равного размеру указателя
    // иначе на 64-битных платформах будет ошибка компиляции
    printf("%d\n", (int) (intptr_t) ptr);
    return NULL;
}

int main()
{
    pthread_t ids[N];

    int i;
    for (i = 0; i < N; ++i) {
        pthread_create(&ids[i], NULL, thr_func, (void*) (intptr_t) i);
    }

    for (int i = 0; i < N; ++i) {
        pthread_join(ids[i], NULL);
    }
}
