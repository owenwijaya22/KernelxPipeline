#include "library/syscalls.h"
#include "library/string.h"
#include "library/stdio.h"
int atoi(const char *str)
{
    int res = 0;
    int sign = 1;
    int i = 0;
    if (str[0] == '-')
    {
        sign = -1;
        i++;
    }
    for (; str[i] != '\0'; ++i)
        res = res * 10 + str[i] - '0';
    return sign * res;
}

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