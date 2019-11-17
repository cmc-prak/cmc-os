#include <unistd.h>

int main()
{
	while (1) {
		char c;
		ssize_t z = read(0, &c, sizeof(c));
		if (z <= 0) break;
		write(1, &c, sizeof(c));
	}
}
