// #include "kernel/types.h"
// #include "user/user.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
int n = 35;
int byte;
#define N 35
int ps[N][2];
void solve()
{
    // 父进程结束， 子进程继续往后传递
    printf("ttttttt\n" );
    int i = 0;
    byte = -1;
    for(;i < N; ++ i) {
        printf("%d\n", i);
        int readRes = read(ps[i][0], &byte, sizeof byte);
        printf("%d\n", readRes);
        if (readRes != 0 && readRes != -1) 
            break;
    }
    if(byte == -1) return;
    printf("prime %d\n", byte);
    char first = 1;
    close(ps[i][0]);
    close(ps[i][1]);
    // 筛选
    // 父进程 close 掉所有文件描述符，如果是可能是质数的 fd 就 fork 之后再close
    for (++i;i < N; ++ i) {
        int t;
        // 文件描述符已经关闭
        int readRes = read(ps[i][0], &byte, sizeof byte);
        if (readRes == 0 || readRes == -1)
            continue;
        // 第一次 发现有 可能的素数，就打开一个新进程
        if(t % byte != 0) {
            write(ps[i][1], &t, sizeof t);
            if(first) {
                if (fork() == 0)  {
                    solve();
                    exit(0);
                } 
                first = 0;
            } 
        }
        // 父进程关掉这个管道
        close(ps[i][1]);
        close(ps[i][0]);                                
        // 筛掉倍数
    }
    wait(0);
}

int
main(int argc, char *argv[])
{
    setbuf(stdout,NULL);
    for(int i = 0;i < N; ++ i) {
        pipe(ps[i]);
        byte = i + 2;
        write(ps[i][1], &byte, sizeof byte);
    }
    solve();
    exit(0);
}