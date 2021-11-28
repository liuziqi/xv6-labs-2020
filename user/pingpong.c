#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, int *argv)
{
    int p1[2];  // parent to child
    int p2[2];  // child to parent

    // creates a new pipe and 
    // records the read and write file descriptors 
    // in the array p
    pipe(p1);
    pipe(p2);

    if(fork() != 0) {
        // 父进程
        close(p1[0]);
        close(p2[1]);

        write(p1[1], "a", 1);
        close(p1[1]);

        char buf[2];
        if(0 == read(p2[0], buf, 1)) {
            fprintf(2, "pingpong: c2p no available character\n");
            exit(1);
        }
        close(p2[0]);

        int pid = getpid();
        fprintf(1, "%d: received pong\n", pid);
        exit(0);
    } else {
        // 子进程
        close(p1[1]);  
        close(p2[0]);

        char buf[2];
        if(0 == read(p1[0], buf, 1)) {
            fprintf(2, "pingpong: p2c no available character\n");
            exit(1);
        }
        close(p1[0]);

        int pid = getpid();
        fprintf(1, "%d: received ping\n", pid);
        write(p2[1], buf, 1);
        close(p2[1]);
        exit(0);
    }
}