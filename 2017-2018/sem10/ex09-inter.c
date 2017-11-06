#include <stdio.h>

// программа печатает аргументы командной строки
// она будет использоваться в качестве "интерпретатора"

int main(int argc, char *argv[])
{
    pause();
    for (int i = 0; i < argc; ++i) {
        printf("<%s>\n", argv[i]);
    }
}
