#include <pthread.h>
#include <stdio.h>

enum { N = 10 };

void *thr_func(void *ptr)
{
    // распечатать свой серийный номер
    printf("%d\n", *(int*) ptr);
    return NULL;
}

int main()
{
    // массив идентификаторов нитей
    pthread_t ids[N];
    // массив серийных номеров нитей
    int nums[N];

    int i;
    for (i = 0; i < N; ++i) {
        nums[i] = i;
        pthread_create(&ids[i], NULL, thr_func, &nums[i]);
    }

    for (int i = 0; i < N; ++i) {
        pthread_join(ids[i], NULL);
    }
}
