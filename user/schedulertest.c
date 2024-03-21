#include "library/syscalls.h"
#include "library/string.h"
#include "library/stdio.h"

void launch_process(const char *exec, int priority) {
    int argc = 2;
    const char *argv[2];
    argv[0] = exec;
    char priority_str[10];
    sprintf(priority_str, "%d", priority);
    argv[1] = priority_str;
    
    int pfd = syscall_open_file(KNO_STDDIR, exec, 0, 0);
    if (pfd >= 0) {
        int pid = syscall_process_run(pfd, argc, argv);
        if (pid > 0) {
            printf("Started process %d\n", pid);
        } else {
            printf("Couldn't run %s: %s\n", exec, strerror(pid));
        }
        syscall_object_close(pfd);
    } else {
        printf("Couldn't find %s: %s\n", exec, strerror(pfd));
    }
}

int main(const char *argv[], int argc) {
    launch_process("bin/process1.exe", 9);
    launch_process("bin/process2.exe", 7);
    launch_process("bin/process3.exe", 2);
    launch_process("bin/process4.exe", 1);
    launch_process("bin/process5.exe", 5);
    return 0;
}