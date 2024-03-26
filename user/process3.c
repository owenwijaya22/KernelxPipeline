#include "library/syscalls.h"
#include "library/string.h"
#include "library/stdio.h"
#include "library/stdlib.h"

void runForSeconds(int seconds)
{
    unsigned int startTime;
    syscall_system_time(&startTime);
    unsigned int timeElapsed;
    do
    {
        syscall_system_time(&timeElapsed);
        timeElapsed -= startTime;
    } while (timeElapsed < seconds);
}

int main(const char *argv[], int argc)
{
    int pid = syscall_process_self();
    int priority = atoi(argv[1]);
    printf("Process %d started with priority %d\n", pid, priority);
    runForSeconds(5);
    printf("Process %d exiting\n", pid);
    return 0;
}