#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

void myexitfunc()
{
    fprintf(stderr, "atexit\n");
}

// системный вызов _exit() немедленно завершает процесс
// без сохранения буферов вывода на устройство
// и вызовов функций, зарегистрированных в atexit
int main()
{
    atexit(myexitfunc);
    printf("Hello\n");
    fork();
    _exit(1);
    //raise(SIGABRT);
}
