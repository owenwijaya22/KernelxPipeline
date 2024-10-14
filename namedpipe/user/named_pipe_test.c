#include "../include/library/syscalls.h"
#include "../include/library/string.h"
#include "../kernel/string.h"
#include "../include/kernel/types.h"

int main() {
    printf("%d: Running named pipe test!\n", syscall_process_self());
    const char *fname = "bin/named_pipe";

    // message to send
    char buffer[] = "Hello World\n";
    int res = syscall_make_named_pipe(fname);

    // create the named pipe
    int pid;
    int fd = syscall_open_named_pipe(fname);
    if(fd >= 0){
        printf("%d: Named pipe successfully open.\n", syscall_process_self());
        pid = syscall_process_fork();

        if(pid==0){
          // child process: sender
          printf("%d: Entering Sending process.\n", syscall_process_self());
          
          syscall_object_write(fd, buffer, strlen(buffer), 0); // write the message to the named pipe
          printf("%d: Message written to named pipe.\n", syscall_process_self());

          syscall_process_sleep(1000);
          return 1; 

        } else if (pid < 0) {
            printf("Fork failed \n");
            return -1;

        } else {
            // parent process: receiver
            char received_message[20];
            int r;

            printf("%d: Entering Reading process\n", syscall_process_self());
            while((r = syscall_object_read(fd, received_message, 20, -1))==0) {
              syscall_process_yield();
            }
            
            if(r > 0 ){
              received_message[r] = '\0';
              printf("%d: Message received: %s\n", syscall_process_self(), received_message);
            } else {
              printf("%d: Error reading the message %s\n", syscall_process_self(), r);
            }

            syscall_object_close(fd); 
            printf("%d: Named pipe closed.\n", syscall_process_self());
            printf("%d: Named pipe test finish.\n", syscall_process_self());
        }

    } else {
        printf("%d: Error opening named pipe.\n", syscall_process_self());
        return 1;
    }

  return 1;
}