#include "kernel/param.h"
#include "kernel/types.h"
#include "user/user.h"

char *getline(int fd)
{
    static char buf[MAXARG];
    char *p = buf;
    for(;;) {
        if(read(fd, p, sizeof (char)) <= 0)
            return 0;
        if(*p == '\n') break;
        ++p;
    }
    *p = 0;
    return buf;
}

int main(int argc, char *argv[])
{
    if(argc < 1) {
        fprintf(2, "xargs: missing operand");
        exit(1);
    }

    for(;; ) {
        char *p = getline(0);
        if(p == 0) break;

        printf("fork %s t\n", p);
        if(fork() == 0) {
            for(int i = 0;i + 1 < argc; ++ i)
                argv[i] = argv[i + 1];
            argv[argc - 1] = p;
            argv[argc] = 0;
            // for(int i = 0;i < argc + 1; ++ i)
            //     printf("%s ", argv[i]);
            // printf("\n");
            exec(argv[0], argv);
            fprintf(2, "exec %s failed\n", argv[1]);
            exit(1);
        }
        else wait(0);
    }
    exit(0);    
}