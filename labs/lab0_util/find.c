#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/stat.h"
char* fmtname(char *path);
// 查找 path 路径下， 名为 targetName 的文件
void find(char* path, const char *targetName);
// 处理文件的情况，目录的情况 直接写在 find 中了
void fileHand(char *path, const char *targetName) ;

int main(int argc, char *argv[])
{
    if(argc < 3) {
        // find(".", "*"); // 可以尝试加一个输出当前目录所有文件的功能
        exit(0);
    }
    if(chdir(argv[1]) < 0) {
        fprintf(2, "find: cannot chdir %s\n", argv[1]);
        exit(0);
    }
    for(int i = 2; i < argc; ++i) {
        find(".", argv[i]);
    }
    exit(0);
}

void  find(char* path, const char *targetName)
{
    // 打开这个路径
    int fd = open(path, 0);
    if(fd < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }
    struct stat st;
    if(fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        return;
    }
    char buf[512], *p;
    struct dirent de;
    switch(st.type) {
        // 如果是文件的话 则 直接检查匹配
        case T_FILE:
            fileHand(path, targetName);
            break;
        case T_DIR:
            if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
                printf("find: path too long\n");
                break;
            }
            // buf 是新的路径 或者是文件
            strcpy(buf, path);
            p = buf + strlen(buf);
            *p++ = '/';
            while(read(fd, &de, sizeof de) == sizeof de) {
                if(strcmp(de.name, ".") == 0) continue;
                if(strcmp(de.name, "..") == 0) continue;
                if(de.inum == 0) 
                    continue;
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                // 递归遍历新的路径 或者文件
                find(buf, targetName);
            }
    }
    close(fd);
}

void
fileHand(char *path, const char *targetName) 
{
    char *name = fmtname(path);
    // printf("name: %s path: %s  targetName : %s  %d \n", name, path, targetName, strcmp(name, targetName));
    if(strcmp(name, targetName) == 0) 
        fprintf(1, "%s\n", path);
}

char* fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;
  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), 0, DIRSIZ-strlen(p));
  return buf;
}