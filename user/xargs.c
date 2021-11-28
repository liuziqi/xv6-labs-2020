#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    char buf[64], *p;
    char *_argv[MAXARG] = {0};

    p = buf;
    for(int i = 1; i < argc; i++) {
        _argv[i - 1] = argv[i];
    }

    while(read(0, p, 1) > 0) {
        if(p == buf + 63) {
            fprintf(2, "xargs: argv is to long\n");
            exit(1);
        }
        if(*p != '\n') {
            p++;
            continue;
        }
        *p = 0;
        _argv[argc - 1] = buf;

        if(fork() == 0) {
            exec(_argv[0], _argv);
        } else {
            wait(0);
        }
        p = buf;
    }
    exit(0);
}