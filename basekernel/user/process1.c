#include "library/syscalls.h"
#include "library/string.h"
#include "library/stdio.h"
#include "library/stdlib.h"

void runForSeconds(int seconds) {
    unsigned int startTime;
    syscall_system_time(&startTime);
    unsigned int timeElapsed;
    do {
        syscall_system_time(&timeElapsed);
        timeElapsed -= startTime;
    } while(timeElapsed < seconds);
}

int main(int argc, const char *argv[]) {
    const char *p = argv[1];
    const char *exe = argv[0];

    int pid = syscall_process_self();
    printf("%s (pid %d) started with priority: %s\n", exe, pid, p);
    runForSeconds(3);
    printf("%s (pid %d) finished with priority: %s\n", exe, pid, p);
    return 0;
}