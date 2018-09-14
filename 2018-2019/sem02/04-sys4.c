#include <asm/unistd_32.h>

void _start()
{
    const static char msg[] = "Hello\n";

    asm volatile("mov %0, %%eax; mov %1, %%ebx; mov %2, %%ecx; mov %3, %%edx; int $0x80" : : "g"(__NR_write), "g"(1), "g"(msg), "g"(sizeof(msg) - 1));
    asm volatile("mov %0, %%eax; mov %1, %%ebx; int $0x80" :: "g"(__NR_exit), "g"(1));
}
