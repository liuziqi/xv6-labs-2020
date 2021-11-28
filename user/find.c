#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"

void
find(char *dpath, char *fname) 
{
    char buf[512], *p, *split;
    int fd;
    struct stat st;
    struct dirent de;

    if((fd = open(dpath, 0)) < 0) {
        fprintf(2, "find: cannot open %s\n", dpath);
        close(fd);
        return;
    }

    if(fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", dpath);
        close(fd);
        return;
    }

    if(T_DIR != st.type) {
        // fprintf(2, "find: %s is not a dir path\n", dpath);
        close(fd);
        return;
    }
    
    if(strlen(dpath) + 1 + DIRSIZ + 1 > sizeof(buf)) {
        fprintf(2, "find: path is too long\n");
        close(fd);
        return;
    }

    strcpy(buf, dpath);
    p = buf + strlen(dpath);
    *p++ = '/';
    split = p;

    while(read(fd, &de, sizeof(de)) == sizeof(de)) {
        if(0 == de.inum) {
            continue;
        }
        memmove(p, de.name, DIRSIZ);
        // p[DIRSIZ] = 0;
        if(0 == strcmp(de.name, ".") || 0 == strcmp(de.name, "..")) {
            ;
        } else if(0 == strcmp(de.name, fname)) {
            fprintf(1, "%s\n", buf);
        } else {
            find(buf, fname);
        }
        p = split;
    }
    close(fd);
}

int
main(int argc, char *argv[]) 
{
    if(argc == 2) {
        find(".", argv[1]);
        exit(0);
    }
    for(int i = 2; i < argc; i++) {
        find(argv[1], argv[i]);
    }
    exit(0);
}