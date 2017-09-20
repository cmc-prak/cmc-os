/* заголовочный файл с номерами системных вызовов */
#include <asm/unistd_32.h>
        .text
        .global _start
_start:
        mov     $__NR_write, %eax
        mov     $1, %ebx                // номер стандартного потока вывода - 1
        mov     $str, %ecx              // адрес выводимой строки
        mov     $estr - str - 1, %edx   // размер выводимой строки
        int     $0x80                   // системный вызов
        mov     $__NR_exit, %eax
        mov     $1, %ebx                // код завершения программы - 1
        int     $0x80                   // системный вызов

str:    .asciz  "Hello\n"
estr:

