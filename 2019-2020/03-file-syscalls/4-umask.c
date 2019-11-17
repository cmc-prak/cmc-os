#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int oldmask = umask(0177);
	(void) oldmask;
	int fd = open(argv[1], O_WRONLY | O_APPEND | O_CREAT, 0777);
	if (fd < 0) {
		fprintf(stderr, "%s: %s\n", argv[1], strerror(errno));
		return 1;
	}

	dup2(fd, 1); close(fd);

	int c;
	while ((c = getchar()) != EOF)
		putchar(c);
}
