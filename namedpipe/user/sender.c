#include "../include/library/syscalls.h"
#include "../kernel/string.h"

int main() {
    const char *fname = "bin/named_pipe";

    printf("%d: Attempting to open named pipe.\n", syscall_process_self());
    int fd = syscall_open_named_pipe(fname); // open the named pipe 

    if (fd < 0) {
        printf("%d: Error opening named pipe.\n", syscall_process_self());
    } else {
        printf("%d: Named pipe opened successfully.\n", syscall_process_self());
    }

    char buffer[] = "Hello from sender!\n";
    syscall_object_write(fd, buffer, strlen(buffer), 0); // write the message to the named pipe
    printf("%d: Message written to named pipe.\n", syscall_process_self());

    syscall_object_close(fd); // close the named pipe
    printf("%d: Named pipe closed.\n", syscall_process_self());

    return 0;
}
