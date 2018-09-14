    .text
    .global main
main:
    push    $strend-str
    push    $str
    push    $1
    call    write
    push    $0
    call    _exit
str:
    .ascii  "Hello\n"
strend:
