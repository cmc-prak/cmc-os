#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

/*
Для компиляции потребуется опция -rdynamic
Программа ищет функцию по имени в самой себе.

gcc -Wall -O2 ex09-useself.c -rdynamic -ouseself -ldl
*/

int myfunc(int x)
{
    return x * 2;
}

int main()
{
    void *handle = dlopen(NULL, RTLD_LAZY);
    if (!handle) abort();

    void *func = dlsym(handle, "myfunc");
    if (!func) abort();

    int x;
    while (scanf("%d", &x) == 1) {
        int y = ((int (*)(int)) func)(x);
        printf("%d\n", y);
    }
    dlclose(handle);
    // dlerror
}
