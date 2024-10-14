#include "../include/library/syscalls.h"
#include "../include/library/string.h"

int main() {
    const char *fname = "bin/named_pipe";
    char buffer[100];
    
    int res = syscall_make_named_pipe(fname);

    if (res < 0) {
        printf("%d: Error creating named pipe.\n", syscall_process_self());
    } else {
        int fd = syscall_open_named_pipe(fname);
        if (fd < 0) {
            printf("%d: Error opening pipe.\n", syscall_process_self());
        }
        
        // read from the named pipe
        int bytes_read = syscall_object_read(fd, buffer, sizeof(buffer) - 1, 0); // leave space for null terminator
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0'; // Null-terminate the string
            printf("%s\n", buffer);
        }
        
        syscall_object_close(fd); // close the named pipe
    }
    
    return 0;
}
