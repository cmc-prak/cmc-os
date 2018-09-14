#include <asm/unistd_32.h>
    .text
    .global _start
_start:
    mov     $__NR_write, %eax
    mov     $1, %ebx
    mov     $str, %ecx
    mov     $strend-str, %edx 
    int     $0x80
    mov     $__NR_exit, %eax
    mov     $0, %ebx
    int     $0x80 
str:
    .ascii  "Hello\n"
strend:
