/*
gcc -Wall -O2 -fPIC -DPIC -shared ex08-myso.c -omyso.so
*/

int myfunc(int x)
{
    return x + 1;
}
