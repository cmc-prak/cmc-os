#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

// неограниченное создание процессов - fork bomb
// запускайте с осторожностью!
int main()
{
    while (1) {
        int pid = fork();
        if (pid < 0) {
            fprintf(stderr, "error: %s\n", strerror(errno));
        }
    }
}
