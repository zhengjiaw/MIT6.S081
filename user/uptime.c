#include "kernel/types.h"
#include "user/user.h"

int main() 
{
	printf("%d\n", uptime());
	exit(0);
}