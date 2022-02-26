## linux 系统启动过程

1. 内核引导

   `BIOS` 启动设备，系统接管硬件后首先读入 `/boot` 目录下内核文件

   `系统` --- `/boot`

2. 运行 init 进程

   init 进程是所有进程的起点，在 `/etc/init*` 目录下读取配置文件，根据 `运行级别` 运行不同的程序

   `系统` --- `/boot` --- `init 进程` --- `运行级别`

3. 系统初始化

   系统会根据 init 配置文件去运行 /etc/rc*.d/ 目录下的启动脚本

   `系统` --- `/boot` --- `init 进程` --- `运行级别` --- `启动脚本开启系统服务`

4. 建立终端

   `系统` --- `/boot` --- `init 进程` --- `运行级别` --- `启动脚本开启系统服务` --- `建立终端`

5. 用户登录

   `系统` --- `/boot` --- `init 进程` --- `运行级别` --- `启动脚本开启系统服务` --- `建立终端` --- `用户登录`



## linux 系统目录



![](d0c50-linux2bfile2bsystem2bhierarchy.jpg)

`/bin` 存放常用的命令

`/boot` 存放启动系统的核心文件

`/dev` 外部设备目录，访问设备方法同访问文件相同

`/etc` 存放系统管理所需的配置文件等

`home` 用户目录

`/lib` 存放最基本的动态连接共享库，几乎所有程序都需要用到该库

`/lost+found` 当系统非法关机了，就会生成该目录

`/media` 系统识别到的外部设备会挂载在这个目录下

`/mnt` 用户临时挂载其他文件系统

`/opt` 其他安装软件的存放目录

`/proc` 伪文件系统，访问该目录可以获取系统信息

`/root` 系统管理员目录

`/sbin` 系统管理员使用的系统管理程序

`/srv` 系统服务启动后需要的数据

`/sys` 映射内核设备树

`/tmp` 存放临时文件，重启清空

`/usr` (unix shared resources 共享资源)用户程序和文件都放在该目录下

`/usr/sbin` 系统管理员用的程序和系统守护程序(服务)

`/var` 该目录存放经常被修改，扩充的文件，例如 log

`/run` 临时文件系统，存放启动以来的信息



## linux 常用指令

#### awk



#### seq

输出序列化的整数

`seq [opt] tail`  或 `seq [opt] head tail` 或 `seq [opt] head step tail`

`-f` 浮点格式；`-s` 用指定字符串分割数字；`-w` 列前添加 0 使宽度相同

e.g.1

```sh
seq 5
输出
1
2
3
4
5
```

e.g.2

```sh
seq 1 2 5
输出
1
3
5
```

e.g.3

```sh
seq -w 9 11
输出
09
10
11
```



#### sed

文本编辑工具：



#### grep





查看文件类型：
file + 文件

文件查找：
find + 路径 + -name "关键词"：查找路径目录及其子目录下包含关键词的文件
e.g.查找当前目录及其子目录下"wula"开头的文件    
find . -name "wula*"

locate + 关键词：在保存目录和文件名的数据库中查找文件和文件夹
e.g.查找Wulala文件夹
sudo updatedb更新数据库
locate Wulala

which + 关键词： 在环境变量$PATH设置的目录中查找文件
e.g.which bash


whereis + 路径 + 关键词：在路径及其子目录下查找二进制文集，源文件和帮助文件

软连接：可以生存文件或者文件夹的链接
ln -s + 源文件/源文件夹 + 目标路径文件名/目标路径文件夹名
e.g.在当前目录生成“./Wulala/lala”的软连接，并改名为haha
ln -s ./Wulala/lala ./haha

硬连接：只能生存文件的链接
ln + 源文件 + 目标路径文件名

ext文件系统简介：
filename               文件
   |
 inode                 文件节点
   |
block:filecontext      文件实体内容



软硬链接区别

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

filename--|           file_hardlink     file_softlink                 |
   |                                          |                  |                          |
 inode - - - - - - - - - - - - - - - - -              inode                      |
   |                                                              |                          |
block:filecontext                       block:source file addr- - -  

硬链接和源文件共享inode节点和block实体内容；
误删除源文件的话，还可以通过硬文件找到文件内容；
只有删除源文件和硬链接，实体内容才会被删除；

软链接有独立inode和block，block内容指向源文件(文件名)；
源文件删除了，软链接将会失效，源文件的实体内容也会被删除；
源文件删除后，新建一个同源文件同名的新文件，软链接会指向新文件；



建立目录的链接：
lndir sourcedir targetdir



移动文件或者文件夹，还可以修改文件名
mv 源文件路径 新文件路径
e.g.把当前目录下tt1文件移动到./TT/tt1
mv ./tt1 ./TT/tt1
e.g.把tt1改名为tt
mv ./tt1 ./tt

mv -b 源文件 新文件：-b参数，如果新文件存在，会备份再覆盖
mv -i 原文件 新文件：-i参数，如果新文件存在，会询问是否覆盖
mv -f 源文件 新文件：-f参数，强制覆盖新文件
mv -n 源文件 新文件：-n参数，不覆盖已有文件和文件夹



删除：
rm [opt] name ...
-i 删除前逐一询问
-f 强制删除
-r 目录下文件逐一删除
e.g.强制删除TT文件夹
rm -rf TT



复制：
cp [opt] source dest
-a 复制目录时候把文件属性，链接，目录下所有内容都复制了。等同与dpr参数组合
-d 复制时候把链接也复制了
-f 覆盖已存在的文件
-p 把时间属性，访问权限一起复制
-r 目录下所有目录和文件一起复制
-l 不复制文件，只生成链接文件



更新数据库文件：
updatedb
主要用于更新locate命令所需要的数据库文件



查看系统文件系统磁盘使用情况：
df



新建目录：
mkdir [opt] dirname
-p 若目录不存在，就新建一个



显示当前工作目录的绝对路经：
pwd



挂载文件到目标路经上：
mount dev dir



卸载文件：
umount [-v] dev
e.g.卸载sda.1
umount -v /dev/sda1



以树状图列出目录内容：
tree



显示当前进程状态，类似windows任务管理器
ps [options] [--help]
-A  列出所有进程
-w  显示加宽显示更多信息
-au 显示详细信息

pstree 以树状图显示



实时显示process动态
top
c 切换显示模式：@1：只显示执行档名称；@2：显示完整路经名称
i 不显示任何空闲idle或者无用zombie进程



重启系统
reboot

关机指令
shutdown [-t seconds] [options] time [message]
-t seconds 设定几秒后关机
-k 不会真关机，只是传送警告信息
-r 关机重启
-h 关机停机
-n 强制关机
-c 取消进行中的关机动作



图形编辑模式打开文本
gedit + filename



ps 只是静态查看进程：`ps -ef` 和 `ps aux` 两者输出内容略有不同，查看进程开启后的消耗和状态；

top 可以动态（持续）监控进程：系统性能，负载，cpu 和内存消耗；

```c
top -H -p [pid]//可以看到[pid]下的子线程的系统消耗等
```

htop 交互式的 top 查看器，可鼠标操作；

ps 和 top 都是从 /proc 目录下读取进程状态信息，内核把系统进程各类信息放到该伪目录。



linux 上进程的 5 个状态：

  (1) 就绪/运行状态(正在运行或在运行队列中等待) 

  (2) 可中断等待状态(休眠中, 受阻, 在等待某个条件的形成或接受到信号) 

  (3) 不可中断(收到信号不唤醒和不可运行, 进程必须等待直到申请的资源有效才唤醒) 

  (4) 僵死/中止(进程已终止, 但进程描述符存在, 直到父进程调用wait4()系统调用后释放) 

  (5) 停止(进程收到SIGSTOP, SIGSTP, SIGTIN, SIGTOU信号后停止运行，收到 SIGCONT信号继续运行)

![](../Shell/image-20220112233512441.png)



linux/unix系统的文件类型大致可分为三类：普通文件、目录文件和伪文件。常见的伪文件分别是特殊文件、命名管道及proc文件。

伪文件不是用来存储数据的，因此这些文件不占用磁盘空间，尽管这些文件确实存在于目录树中，是目录树的一部分，并且也执照目录的方式来组织。伪文件的目的是提供一种服务，通过这种服务，系统可以像操作普通文件一样来访问这些文件对代表的对象。是重要的是伪文件是设备文件。如键盘、鼠标、显示器等。对于这些设备文件都有对应有伪文件来表示它们。除了设备之外，还有命名管道，通过命名管道，用户可以将一个命令的输出连接到另外一个命令的输入上面。最后一种伪文件是proc文件，它允许访问内核中的信息。



修改用户的 home 目录，也是登录后的目录：

在 /etc/passwd 中找到用户名，修改倒数第一个冒号前的目录

![](../Shell/企业微信截图_16421262684068.png)



终端下查看系统中断信息的记录：

```c
cat /proc/interrupt
```



more 和 less 可以翻页查看文本内容



关闭进程：

```c
kill + [PID]
```

```c
killall + 进程名
```



mount 挂载设备

umount 卸载设备



数据搜索：

grep [opt] targetdata [file]

e.g. 在 fileA 搜索 wula

```c
grep wula fileA
```



反向搜索：

e.g. 不显示 fileA 中的 maga

```c
grep -v maga fileA
```



tar 数据归档（压缩）

tar func [opt] obj1 obj2 ...

```c++
func:
-A	将一个已有tar归档文件追加到另一个已有tar归档文件
-c	创建一个新的tar归档文件
-r  追加文件到已归档的文件末尾    
-t  列出归档的内容    
-x  从已归档的文件从提取文件
```

```c
opt:
-C	切换到指定目录
-f	结果输出到文件或者设备
-p  保留文件所有权    
-v  处理文件时显示文件    
-z  将输出重定向给 gzip 压缩内容
-j  将输出重定向给 bzip 压缩内容
```

e.g. 把 obj1 obj2 打包到 /tmp/ 目录下

```c
tar -cvf /tmp/test.tar obj1 obj2
```

e.g. 解压 test.tar

```c
tar -xvf test.tar
```



命令行加 `;` 分号来执行多个指令

命令的后台执行：在命令后面加 `&` 



history 查看历史上使用的命令



## linux 系统概念

#### 环境变量

环境变量保存 shell 的会话信息和工作环境，全局变量对所有 shell 可见，局部变量只对创建它们的 shell 可见

查看环境变量：

```c
env 或者 printenv
```

printenv 可以查看具体某个环境变量

添加局部环境变量：

```c++
echo $my_env
my_env="Hello World"
echo $my_env
```

添加全局环境变量 export：

```c++
echo $my_env
my_env="Hello World"
export my_env
```

删除环境变量 unset

```c++
echo $my_env
my_env="Hello World"
unset my_env
```



登录  linux 就会启动 /etc/profile 文件中的指令

sh 脚本结果的获取：用 $( xxx.sh ) 或者 ·xxx.sh· 来获取 xxx.sh 的执行结果



## linux 属性



