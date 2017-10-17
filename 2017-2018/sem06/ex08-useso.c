#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

/*
Программа подгружает динамичесий модуль и вызывает из него функцию
*/

int main()
{
    void *handle = dlopen("./myso.so", RTLD_LAZY);
    if (!handle) abort();

    void *func = dlsym(handle, "myfunc");
    if (!func) abort();

    int x;
    while (scanf("%d", &x) == 1) {
        int y = ((int (*)(int)) func)(x);
        printf("%d\n", y);
    }
}
