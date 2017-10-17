#include <stdio.h>

typedef void (*myfunc_t)(void);

static myfunc_t func;

void otherfunc(void)
{
    printf("RED ALERT!\n");
}

void setfunc()
{
    func = otherfunc;
}

int main()
{
    func();
}
