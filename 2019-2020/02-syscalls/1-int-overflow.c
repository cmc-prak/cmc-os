#include <stdio.h>

int main()
{
	int a, b;
	scanf("%d%d", &a, &b);
	int c;
	_Bool res = __builtin_add_overflow(a, b, &c);
	printf("%d %d\n", res, c);
}

