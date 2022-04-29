#include <stdio.h>
#include <arpa/inet.h>

void test_Endian()
{
	unsigned int a = 0x12345678;
	unsigned char *p = (unsigned char *)&a;	//只取一个字节
	if(0x12 == *p){
		printf("Big-Endian\n");
	}else if(0x78 == *p){
		printf("Little-Endian\n");
	}
}
// 测试于 Ubuntu20.04 TLS
int main()
{
    test_Endian();
    unsigned int i = 0x00646c72; // 72 6c 64 00
	printf("H%x Wo%s\n", 57616, &i);
    //  Big-Endian
    i = 0x726c6400;
    i =  ntohl(i); // 转成网络字节序，大端
    printf("H%x Wo%s\n", 57616, &i);
    return 0;
}