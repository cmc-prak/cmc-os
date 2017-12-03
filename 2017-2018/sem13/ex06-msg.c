#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include <sys/ipc.h>
#include <sys/msg.h>

enum { KEY = 0xdeadbeef };

struct MyBuf
{
    long mtype;
    int val;
};

/*
 * "пинг-понг" между двумя процессами с помощью одной очереди сообщений SysV IPC
 * первый процесс ждет сообщения с типом 1, второй процесс - с типом 2
 */
int main()
{
    int msgid = msgget(KEY, 0600 | IPC_CREAT | IPC_EXCL);
    if (msgid < 0) {
        fprintf(stderr, "error: %s\n", strerror(errno));
        exit(1);
    }
    for (int i = 1; i <= 2; ++i) {
        if (!fork()) {
            while (1) {
                struct MyBuf rv;
                int r = msgrcv(msgid, &rv, sizeof(struct MyBuf) - sizeof(long), i, 0);
                if (r < 0) {
                    fprintf(stderr, "error: %d: %s\n", i, strerror(errno));
                    exit(1);
                }
                printf("%d %d\n", i, rv.val);
                ++rv.val;
                rv.mtype = 3 - i;
                msgsnd(msgid, &rv, sizeof(struct MyBuf) - sizeof(long), 0);
            }
            _exit(0);
        }
    }

    msgsnd(msgid, &(struct MyBuf){ 1, 100 }, sizeof(struct MyBuf) - sizeof(long), 0);

    signal(SIGINT, SIG_IGN);

    int r;
    while ((r = wait(NULL)) > 0) {
    }
    fprintf(stderr, "wait: %s\n", strerror(errno));
    msgctl(msgid, IPC_RMID, NULL);
}
