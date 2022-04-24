#include "kernel/types.h"
#include "user/user.h"
// 主要思路为：递归下降
// 也就是每一个质数 拥有 一个 left 和 right 管道
// 从左边读入，写入到右边，逐渐在过程中排除即可
void solve(int *pl)
{
    close(pl[1]); // 关闭 左边的写

    int prime  = -1;
    if(read(pl[0], &prime, sizeof prime) == 0) return;
    printf("prime %d\n", prime );

    int pr[2];
    pipe(pr);
    if(fork() == 0) {
        solve(pr);
        close(pr[1]);  // 关闭 右边的写 
        close(pl[0]);  // 关闭 左边的读
        exit(0);
    }
    
    close(pr[0]); // 关闭右端的读
    for (int t; read(pl[0], &t, sizeof t) != 0;) 
        if(t % prime  != 0)  // 筛掉倍数
            write(pr[1], &t, sizeof t);
    close(pr[1]); // 关闭右边的写
    
    wait(0);
}
// 对于每个质数，你将安排创建一个进程，该进程通过管道从其左邻居读取数据，并通过另一个管道写入其右邻居
int
main(int argc, char *argv[])
{
    #define N 35
    int pl[2];
    pipe(pl);
    if(fork() != 0) {
        
        close(pl[0]);
        for(int i = 2;i <= N; ++ i) 
            write(pl[1], &i, sizeof i);
        close(pl[1]); // 写完之后 则关闭这个管道

        wait(0);
    }
    else 
        solve(pl);
    exit(0);
}