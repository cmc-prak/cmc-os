#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <locale.h>
#include <stdlib.h>

extern char **environ;

int main(int argc, char **argv)
{
    // печать аргументов командной строки
    for (int i = 1; i < argc; ++i) {
        printf("%d: %s\n", i, argv[i]);
    }
    // печать переменных окружения
    for (int i = 0; environ[i]; ++i) {
        printf("%s\n", environ[i]);
    }
    // установка локали в зависимости от окружения
    setlocale(LC_ALL, "");
    // запрос значения переменной окружения
    char *s = getenv(argv[1]);
    if (s) {
        printf("%s\n", s);
    }
    FILE *f = fopen(argv[2], "r");
    if (!f) {
        // сообщение об ошибке выводится на stderr
        // в конце вывода обязателен \n
        fprintf(stderr, "error: %s\n", strerror(errno));
        // ненулевой код завершения программы
        return 1;
    }
}
