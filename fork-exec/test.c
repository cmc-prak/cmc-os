#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
   int fd = creat("out", 0666);
   int pid = fork();
 
    if (pid == 0) { 
        int new_fd = dup2(fd, 1);
        execlp("ls", "abacaba", argv[1], NULL);
        perror("q");
    } else {
        int status, p;
        p = wait(&status);
        printf("%d %d %d", status, WIFEXITED(status), WEXITSTATUS(status));
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
            printf("%d %d %d\n", pid, p, status);
    }
}
