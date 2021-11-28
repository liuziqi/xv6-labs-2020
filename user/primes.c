#include "kernel/types.h"
#include "user/user.h"


void recur_fork();

int
main()
{   
    int p[2];
    pipe(p);
    if(0 != fork()) {
        close(p[0]);
        for(int i = 2; i <= 35; i++) {
            write(p[1], &i, sizeof(int));
        }
        close(p[1]);
        wait(0);
    } else {
        recur_fork(p);
    }
    exit(0);
}

void recur_fork(int p[]) {
    int prime, n;
    int child_p[2];

    close(p[1]);
    if(read(p[0], &prime, sizeof(int))) {
        fprintf(1, "prime %d\n", prime);
        pipe(child_p);
        if(0 != fork()) {
            close(child_p[0]);
            while(read(p[0], &n, sizeof(int))) {
                if(n % prime != 0) {
                    write(child_p[1], &n, sizeof(int));
                }
            }
            close(p[0]);
            close(child_p[1]);
            wait(0);
        } else {
            recur_fork(child_p);
        }
    }
}