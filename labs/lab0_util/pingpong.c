#include "kernel/types.h"
#include "user/user.h"

int 
main(int argc, char *argv[])
{
    int p1[2], p2[2];
    
    pipe(p1);
    pipe(p2);
    char byte = 1;
    int pid = fork();
    if (pid == 0) {
        close(p1[1]);
        close(p2[0]);
        read(p1[0], &byte, sizeof byte);
        printf("%d: received ping\n", getpid());
        write(p2[1], &byte, sizeof byte);
    } else {
        close(p1[0]);
        close(p2[1]);
        write(p1[1], &byte, sizeof byte);
        read(p2[0], &byte, sizeof byte);
        printf("%d: received pong\n", getpid());
        wait(0);
    }
    exit(0);
}