#! https://zhuanlan.zhihu.com/p/504164986
# MIT6.S081 脚本、docker一键环境配置 （支持任意 OS）



这是MIT6.S081 2020年 labs 的环境配置方法，主要提供了 **脚本 和 docker ** 两种方法。



## ubuntu 脚本安装

Ubuntu 用户可直接 执行下面的命令即可。

```bash
sudo apt-get install git build-essential gdb-multiarch qemu-system-misc gcc-riscv64-linux-gnu binutils-riscv64-linux-gnu  -y && 
sudo apt install gcc-riscv64-unknown-elf -y  && 
sudo apt-get install qemu-system-misc=1:4.2-3ubuntu6

# 这一步可能需要外网
git clone git://g.csail.mit.edu/xv6-labs-2020
cd xv6-labs-2020
git checkout util

# 这一步是为了 make grade 的时候，要用到 python2，但是我们默认安装的 python3，所以做一个 ln
sudo ln /usr/bin/python3 /usr/bin/python

# 目前已知，在 Ubuntu18.04， Ubuntu22.04 需要继续执行，注释 exit 即可
exit
# 如果上面的 sudo apt-get install qemu-system-misc=1:4.2-3ubuntu6 失败了
# 则在 Ubuntu 上可以 通过这个方式 获得 qemu-system-misc=1:4.2-3ubuntu6
# 安装 两个依赖，
wget http://kr.archive.ubuntu.com/ubuntu/pool/main/b/brltty/libbrlapi0.7_6.0+dfsg-4ubuntu6_amd64.deb
sudo dpkg -i libbrlapi0.7_6.0+dfsg-4ubuntu6_amd64.deb

wget http://security.ubuntu.com/ubuntu/pool/main/n/nettle/libnettle7_3.5.1+really3.5.1-2ubuntu0.2_amd64.deb
sudo dpkg -i libnettle7_3.5.1+really3.5.1-2ubuntu0.2_amd64.deb

# 安装
wget http://archive.ubuntu.com/ubuntu/pool/main/q/qemu/qemu-system-misc_4.2-3ubuntu6_amd64.deb
sudo dpkg -i qemu-system-misc_4.2-3ubuntu6_amd64.deb

```



接下来输入       , 看到打开一个新的 shell 就算成功了，可以尝试执行一个 ls

```bash
cd xv6-labs-2020/
make qemu
```






## docker 

我已经配置好了一个基于 Ubuntu20.04 的 MIT6.S081 docker 环境，可以直接通过 docker 在**任意操作系统上配置环境。**



### 安装 docker 

下面是我在 Ubuntu 上 使用的 **命令**，来自 docker 官网，其他发行版可以参考官网：[安装 Docker 引擎|Docker 文档](https://docs.docker.com/engine/install/) 

```bash
# 官网提供的 Ubuntu 下的 sh 脚本安装方法（不建议将脚本用于生产环境 ) https://docs.docker.com/engine/install/ubuntu/#installation-methods
curl -fsSL https://get.docker.com -o get-docker.sh
sudo sh get-docker.sh
```

### pull MIT6.S081 images

这个 images （400多MB）开箱即用，我已经配置好了所需要的环境。,下面是获得方法：

```bash
sudo docker push linxi177229/mit6.s081:latest
```

等待下载完成之后，可以用  ,简单查看一些，是否存在： linxi177229/mit6.s081:latest

![image-20220424151029214](https://picture-1304504595.cos.ap-beijing.myqcloud.com/image-20220424151029214.png)

```bash
sudo docker images 
```

然后 接着 由 镜像  run 一个 mit6.s081 容器，attach 之后 环境就配置好了。

```bash
sudo docker run --name mit6.s081 -itd linxi177229/mit6.s081
docker attach mit6.s081 
# 开启了一个 新的 terminal
cd xv6-labs-2020/
make qemu
```

![image-20220424152251427](https://picture-1304504595.cos.ap-beijing.myqcloud.com/image-20220424152251427.png)



### 一些 docker 使用建议

一些 docker 使用建议 如果使用过程中遇到什么 命令不太熟悉，去[docker | Docker Documentation](https://docs.docker.com/engine/reference/commandline/docker/)直接搜是很方便的。

另外 VS Code 中的 [Docker - Visual Studio Marketplace](https://marketplace.visualstudio.com/items?itemName=ms-azuretools.vscode-docker) 这个脚本会让使用 docker 变得很自然。



## 测试方法

```bash
cd xv6-labs-2020

make qemu # 开启一个 shell 手动运行那些 usr/ 下的程序来测试

# 自动化测试：make grade 可以获得得分情况
make grade

# 或者可以使用 进行单个测试
./grade-具体lab名字 part名字

```

## 退出方式
```bash
# 在另一个新开的终端执行 
pkill -f qemu  
```


## 调试方法

```bash
# 第一个 terminal
cd xv6-labs-2020
# 第一次执行 gdb 需要 执行 下面条语句 
echo "add-auto-load-safe-path $(pwd)/.gdbinit " >> ~/.gdbinit # 第一次执行

make CPUS=1 qemu-gdb

# 第二个 terminal
cd xv6-labs-2020
gdb-multiarch

# 接下来就和使用 平常的 gdb 一样了， layout split 是一个很棒的用法
# 在 gdb 中输入 kill 即可 退出
# 或者 在第三个 teminal 中输入 pkill -f qemu 也可以退出
```

![image-20220424124231555](https://picture-1304504595.cos.ap-beijing.myqcloud.com/image-20220424124231555.png)



一项调试提示：[实验室指导 (mit.edu)](https://pdos.csail.mit.edu/6.828/2020/labs/guidance.html)











