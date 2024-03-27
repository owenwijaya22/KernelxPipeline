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

    printf("%s started with priority: %s\n", exe, p);
    runForSeconds(5);
    printf("%s finished with priority: %s\n", exe, p);
    return 0;
}