        .text
/* поскольку используется стандартная библиотека Си, то точка входа - main */
        .global main
main:
        push    $estr - str - 1
        push    $str
        push    $1
        call    write
        add     $12, %esp
        push    $2
        call    _exit           // системный вызов exit в libc вызывается функцией _exit

str:    .asciz  "Hello\n"
estr: 
