
#include "library/stdlib.h"
#include "library/syscalls.h"
int atoi(const char *str) {
    int res = 0;
    int sign = 1;
    int i = 0;
    if (str[0] == '-') {
        sign = -1;
        i++;
    }
    for (; str[i] != '\0'; ++i)
        res = res * 10 + str[i] - '0';
    return sign * res;
}
void exit( int code )
{
	syscall_process_exit(code);
	
}

/* Some easy things yet to be implemented. */

int rand();
int srand( int );
int system( const char *str );
int sleep( int seconds );
int usleep( int usec );
int time( int *t );
struct timeval;
int gettimeofday( struct timeval *tval, void *arg );
