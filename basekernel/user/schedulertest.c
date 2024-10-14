#include "library/syscalls.h"
#include "library/string.h"
#include "library/stdio.h"

void int_to_str(int num, char *str) {
    int i = 0;
    int is_negative = 0;

    // Handle 0 explicitely, otherwise empty string is printed for 0
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }

    // In standard `itoa()`, negative numbers are handled only at end
    if (num < 0) {
        is_negative = 1;
        num = -num;
    }

    // Process individual digits
    while (num != 0) {
        int rem = num % 10;
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num/10;
    }

    // If number is negative, append '-'
    if (is_negative) {
        str[i++] = '-';
    }

    str[i] = '\0'; // Append string terminator

    // Reverse the string
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}


void launch_process(const char *exec, int priority) {
    int argc = 2;
    const char *argv[2];
    argv[0] = exec;
    char priority_str[10];
    int_to_str(priority, priority_str );
    argv[1] = priority_str;
    
    int pfd = syscall_open_file(KNO_STDDIR, exec, 0, 0);
    if (pfd >= 0) {
        int pid = syscall_process_run(pfd, argc, argv);
        if (pid > 0) {
            printf("%s with pid: %d is put to ready queue\n", exec, pid);
        } else {
            printf("Couldn't run %s: Error %d\n", exec, pid);
        }
        syscall_object_close(pfd);
    } else {
        printf("Couldn't find %s: Error %d\n", exec, pfd);
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