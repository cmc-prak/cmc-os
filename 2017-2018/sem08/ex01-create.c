#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sched.h>

void *thr_func(void *ptr)
{
    // печатаем свой идентификатор
    printf("%lu\n", pthread_self());
    return NULL;
}

int main()
{
    // переменные для хранения идентификаторов нитей
    pthread_t id1 = 0, id2 = 0;

    // создание нитей
    pthread_create(&id1, NULL, thr_func, NULL);
    pthread_create(&id2, NULL, thr_func, NULL);

    // завершаем "главную" нить, процесс завершится,
    // когда завершатся все запущенные нити
    pthread_exit(NULL);

    //pthread_join(id1, NULL);
    //pthread_join(id1, NULL);
}
