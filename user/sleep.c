#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    if(argc < 2) {
        fprintf(2, "sleep: no time specified\n");
        exit(1);
    }

    int i;
    for(i = 1; i < argc; i++) {
        int t = atoi(argv[i]);
        sleep(t);
    }
    exit(0);
}
