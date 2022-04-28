#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
int x = 111;
void main()
{
    asm volatile("ld a2,x");
    printf("x=%d y=%d\n", 3,123);
    exit(0);
}
