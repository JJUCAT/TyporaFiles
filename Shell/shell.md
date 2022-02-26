##  变量

使用变量，变量前加 `$`

```sh
name="maga"
echo ${name}
```



只读变量，变量前加 `readonly`

```sh
name="maga"
readonly name
name="wula"
运行脚本报错 name 只读
```



删除变量，变量前加 `unset` ，不能删除只读变量

```sh
name="maga"
unset name
echo ${name}
运行脚本无输出
```



使用  \`\` 或者 $( ) 来获取指令的返回值



## 字符串

可用单引号，双引号，甚至不用引号

双引号内可以出现变量和转义字符，`""` 可以直接拼接字符串

```sh
name="maga"
str="Hello,\"${name}\""
echo -e ${str}
运行脚本输出：Hello,"maga"
```



获取字符串长度

```sh
name="maga"
echo ${#name}
运行脚本输出：4
```



提取字符串，第一个字符的索引为 0

`${string:pos}:` 提取 `pos` 位置开始的子串

```sh
name="maga"
echo ${name:1}
运行脚本输出：aga
```

`${string:pos:len}:` 提取 `pos` 位置开始，长度 `len` 的子串

```sh
name="maga"
echo ${name:1:2}
运行脚本输出：ag
```



查找字符串，返回第一次找到字符串的位置，这里索引位置又变成第一个字符为 1 了 ???

```sh
name="maga"
echo `expr index ${name} a`
运行脚本输出：2
```





```sh
${string#substring}                             从变量$string的开头, 删除最短匹配$substring的子串
${string##substring}                            从变量$string的开头, 删除最长匹配$substring的子串
${string%substring}                             从变量$string的结尾, 删除最短匹配$substring的子串
${string%%substring}                            从变量$string的结尾, 删除最长匹配$substring的子串   
${string/substring/replacement}                 使用$replacement, 来代替第一个匹配的$substring
${string//substring/replacement}                使用$replacement, 代替所有匹配的$substring
${string/#substring/replacement}                如果$string的前缀匹配$substring, 那么就用$replacement来代替匹配到的$substring
${string/%substring/replacement}                如果$string的后缀匹配$substring, 那么就用$replacement来代替匹配到的$substring
```



## 数组

shell 支持一维数组，用 `( )` 表示数组，元素用空格分隔开

```sh
maga=(m a g a)
```

```sh
maga[0]=m
maga[1]=a
maga[2]=g
maga[3]=a
```



读取数组单个元素

```sh
maga=(m a g a)
g=${maga[2]}
```



读取数组所有元素

```sh
maga=(m a g a)
all=${maga[@]}
```



获取数组长度

```sh
maga=(m a g a)
n=${#maga[@]}
```



获取数组元素长度

```sh
all=("wula" "maga" "hola")
sn=${#maga[1]} #获取元素"maga"的长度
```



## 注释

行注释 

```sh
# 这是行注释
```

段注释

```sh
<< EOF
这是段注释
EOF
```



## 参数

| 变量 | 含义                                                         |
| ---- | ------------------------------------------------------------ |
| $0   | 当前脚本的文件名，包含路径                                   |
| $n   | 传递给脚本或函数的参数。n 是一个数字，表示第几个参数。例如，第一个参数是$1，第二个参数是$2。 |
| $#   | 传递给脚本或函数的参数个数。                                 |
| $*   | 传递给脚本或函数的所有参数。视为 1 个变量                    |
| $@   | 传递给脚本或函数的所有参数。被双引号(" ")包含时，会分别列出各个参数 |
| $?   | 上个命令的退出状态，或函数的返回值。                         |
| $$   | 当前Shell进程ID。对于 Shell 脚本，就是这些脚本所在的进程ID。 |

`“${@:n}”` 可以筛选并分离出第 n 个开始的所有参数





## 基本运算符

原生 bash 没有运算符，可通过 `expr` 或者 `awk` 来运算，运算符和变量之间一定要有空格隔开



#### 算数运算符

| 运算符 | 说明                                          | 举例                          |
| :----- | :-------------------------------------------- | :---------------------------- |
| +      | 加法                                          | `expr $a + $b` 结果为 30。    |
| -      | 减法                                          | `expr $a - $b` 结果为 -10。   |
| *      | 乘法                                          | `expr $a \* $b` 结果为  200。 |
| /      | 除法                                          | `expr $b / $a` 结果为 2。     |
| %      | 取余                                          | `expr $b % $a` 结果为 0。     |
| =      | 赋值                                          | a=$b 把变量 b 的值赋给 a。    |
| ==     | 相等。用于比较两个数字，相同则返回 true。     | [ $a == $b ] 返回 false。     |
| !=     | 不相等。用于比较两个数字，不相同则返回 true。 | [ $a != $b ] 返回 true。      |

**注意：**条件表达式要放在方括号之间，并且要有空格，例如: **[$a==$b]** 是错误的，必须写成 **[ $a == $b ]**。

乘法运算符前面必须加 `\*`



#### 关系运算符

关系运算符只支持数字，不支持字符串，除非字符串是数字

假设 a=10，b=20

| 运算符 | 说明                                                  | 举例                       |
| :----- | :---------------------------------------------------- | :------------------------- |
| -eq    | 检测两个数是否相等，相等返回 true。                   | [ $a -eq $b ] 返回 false。 |
| -ne    | 检测两个数是否不相等，不相等返回 true。               | [ $a -ne $b ] 返回 true。  |
| -gt    | 检测左边的数是否大于右边的，如果是，则返回 true。     | [ $a -gt $b ] 返回 false。 |
| -lt    | 检测左边的数是否小于右边的，如果是，则返回 true。     | [ $a -lt $b ] 返回 true。  |
| -ge    | 检测左边的数是否大于等于右边的，如果是，则返回 true。 | [ $a -ge $b ] 返回 false。 |
| -le    | 检测左边的数是否小于等于右边的，如果是，则返回 true。 | [ $a -le $b ] 返回 true。  |



#### 布尔运算符

假设 a=10，b=20

| 运算符 | 说明                                                | 举例                                     |
| :----- | :-------------------------------------------------- | :--------------------------------------- |
| !      | 非运算，表达式为 true 则返回 false，否则返回 true。 | [ ! false ] 返回 true。                  |
| -o     | 或运算，有一个表达式为 true 则返回 true。           | [ $a -lt 20 -o $b -gt 100 ] 返回 true。  |
| -a     | 与运算，两个表达式都为 true 才返回 true。           | [ $a -lt 20 -a $b -gt 100 ] 返回 false。 |



#### 逻辑运算符

假设 a=10，b=20

| 运算符 | 说明       | 举例                                       |
| :----- | :--------- | :----------------------------------------- |
| &&     | 逻辑的 AND | [[ $a -lt 100 && $b -gt 100 ]] 返回 false  |
| \|\|   | 逻辑的 OR  | [[ $a -lt 100 \|\| $b -gt 100 ]] 返回 true |



#### 字符串运算符

假设 a="abc"，b="efg"

| 运算符 | 说明                                         | 举例                     |
| :----- | :------------------------------------------- | :----------------------- |
| =      | 检测两个字符串是否相等，相等返回 true。      | [ $a = $b ] 返回 false。 |
| !=     | 检测两个字符串是否不相等，不相等返回 true。  | [ $a != $b ] 返回 true。 |
| -z     | 检测字符串长度是否为0，为0返回 true。        | [ -z $a ] 返回 false。   |
| -n     | 检测字符串长度是否不为 0，不为 0 返回 true。 | [ -n "$a" ] 返回 true。  |
| $      | 检测字符串是否为空，不为空返回 true。        | [ $a ] 返回 true。       |



#### 文件测试运算符

检测 Unix 文件的各种属性

| 操作符  | 说明                                                         | 举例                      |
| :------ | :----------------------------------------------------------- | :------------------------ |
| -b file | 检测文件是否是块设备文件，如果是，则返回 true。              | [ -b $file ] 返回 false。 |
| -c file | 检测文件是否是字符设备文件，如果是，则返回 true。            | [ -c $file ] 返回 false。 |
| -d file | 检测文件是否是目录，如果是，则返回 true。                    | [ -d $file ] 返回 false。 |
| -f file | 检测文件是否是普通文件（既不是目录，也不是设备文件），如果是，则返回 true。 | [ -f $file ] 返回 true。  |
| -g file | 检测文件是否设置了 SGID 位，如果是，则返回 true。            | [ -g $file ] 返回 false。 |
| -k file | 检测文件是否设置了粘着位(Sticky Bit)，如果是，则返回 true。  | [ -k $file ] 返回 false。 |
| -p file | 检测文件是否是有名管道，如果是，则返回 true。                | [ -p $file ] 返回 false。 |
| -u file | 检测文件是否设置了 SUID 位，如果是，则返回 true。            | [ -u $file ] 返回 false。 |
| -r file | 检测文件是否可读，如果是，则返回 true。                      | [ -r $file ] 返回 true。  |
| -w file | 检测文件是否可写，如果是，则返回 true。                      | [ -w $file ] 返回 true。  |
| -x file | 检测文件是否可执行，如果是，则返回 true。                    | [ -x $file ] 返回 true。  |
| -s file | 检测文件是否为空（文件大小是否大于0），不为空返回 true。     | [ -s $file ] 返回 true。  |
| -e file | 检测文件（包括目录）是否存在，如果是，则返回 true。          | [ -e $file ] 返回 true。  |
| -S file | 检测文件是否 socket。                                        |                           |
| -L file | 检测文件是否存在并且是一个符号链接。                         |                           |



## 流程控制

#### if

```sh
if condition
then
    cmd1
    cmd2
    ...
fi
```

```sh
if condition
then
    cmd1
    cmd2
    ...
else
    cmd
fi
```

```sh
if condition1
then
    cmd1
elif condition2
then
    cmd2
else
    cmd3
fi
```



#### for

```sh
for var in item1 item2 ... itemn
do
    cmd1
    cmd2
    ...
done
```



#### while

```sh
while condition
do
    cmd
done
```



#### until

```sh
until condition
do
    cmd
done
```



#### case

```sh
case var in
value1)
    cmd1
    ;;
value2)
    cmd2
    ;;
value3)
    cmd3
    ;;
*) # others value
    cmd4
    ;;
esac
```



#### break & continue

`break` 跳出整个循环

`continue` 跳过当前循环到下一次的循环



## 函数

函数格式如下，`[ ]` d表示可选，如果没有 `return` 会把最后一条指令结果作为返回值

```sh
[function] funname [()]
{
    cmd
    ...
    [return int]
}
```

| 变量 | 含义                                                         |
| ---- | ------------------------------------------------------------ |
| $0   | 当前脚本的文件名，包含路径                                   |
| $n   | 传递给脚本或函数的参数。n 是一个数字，表示第几个参数。例如，第一个参数是$1，第二个参数是$2。 |
| $#   | 传递给脚本或函数的参数个数。                                 |
| $*   | 传递给脚本或函数的所有参数。视为 1 个变量                    |
| $@   | 传递给脚本或函数的所有参数。被双引号(" ")包含时，会分别列出各个参数 |
| $?   | 上个命令的退出状态，或函数的返回值。                         |
| $$   | 当前Shell进程ID。对于 Shell 脚本，就是这些脚本所在的进程ID。 |

```sh
#!/bin/bash
# author:菜鸟教程
# url:www.runoob.com

funWithReturn(){
  echo "这个函数会对输入的两个数字进行相加运算..."
  echo "输入第一个数字: "
  read aNum
  echo "输入第二个数字: "
  read anotherNum
  echo "两个数字分别为 $aNum 和 $anotherNum !"
  return $(($aNum+$anotherNum))
}
funWithReturn
echo "输入的两个数字之和为 $? !"
```



## 输入输出重定向

| 命令            | 说明                                               |
| :-------------- | :------------------------------------------------- |
| command > file  | 将输出重定向到 file。                              |
| command < file  | 将输入重定向到 file。                              |
| command >> file | 将输出以追加的方式重定向到 file。                  |
| n > file        | 将文件描述符为 n 的文件重定向到 file。             |
| n >> file       | 将文件描述符为 n 的文件以追加的方式重定向到 file。 |
| n >& m          | 将输出文件 m 和 n 合并。                           |
| n <& m          | 将输入文件 m 和 n 合并。                           |
| << tag          | 将开始标记 tag 和结束标记 tag 之间的内容作为输入。 |

需要注意的是文件描述符 0 通常是标准输入（STDIN），1 是标准输出（STDOUT），2 是标准错误输出（STDERR）



一般情况下，每个 Unix/Linux 命令运行时都会打开三个文件：

- 标准输入文件(stdin)：stdin的文件描述符为0，Unix程序默认从stdin读取数据。
- 标准输出文件(stdout)：stdout 的文件描述符为1，Unix程序默认向stdout输出数据。
- 标准错误文件(stderr)：stderr的文件描述符为2，Unix程序会向stderr流中写入错误信息。

默认情况下，`cmd > file` 将 stdout 重定向到 file，`cmd < file` 将stdin 重定向到 file。如果要 stderr 重定向，可以使用 `cmd 2>file`

如果要将 stderr 和 stdout 合并重定向到 file，可以 `cmd >> file 2>&1`



如果不希望指令不输出到屏幕，可以 `cmd > /dev/null`



## 文件包含

`. filename` 可以把其他 shell 脚本的内容包含进来

这个被包含的脚本不需要可执行权限



## 管道

`|` 管道符，可以连接多个命令

`cmd1 | cmd2` ：`cmd1` 的输出作为 `cmd2` 的输入

连接的命令只能处理前一个命令正确的输出，无法处理错误信息





## 可复用的 shell 脚本体系

1、减少脚本长度

2、减少脚本功能范围

3、减少脚本传入参数

4、注释

5、常用函数放到公共脚本，公共脚本（字符串，文件，目录，日期、时间、日志）禁用系统变量

6、函数应传入“输出参数变量名”来实现输出参数



脚本模块化和复用：

导入模块的两种方式：

sh test.sh #这种方法是新建子进程来执行脚本

source test.sh 或者 . test.sh #这种方法是拷贝脚本内容，可获取脚本的函数和变量



使用 source 或者 .  来引用模块时候，系统默认在环境变量 $PATH 中寻找脚本

可以用 export 来把引用的脚本添加到临时环境变量中（类似 include 头文件）

另外可以加入判断条件，发现某个变量就返回出去，不导入后续内容



log.sh

```sh
#!/bin/bash

#这里我注释掉的内容就是。而且这个变量名称，尽量按照某种规则确保是唯一的变量名称。

#if [${m_log} ];then

#return 0

#fi

#m_log="m_log"

#这一行输出是为了测试是否多次导入了。

echo"TEST IS"

MODULENAME=$(basename$0)

LOGFILE=/tmp/logfile-`date+%Y%m%d`

 

log_info()

{

  #[ 2017-03-31 12:00:00 ] - TextName - The logmessage

  datetime=`date "+%Y-%m-%d%H:%M:%S"`

  if [ "$1" ];then

     echo "[ ${datetime} ] - ${MODULENAME}- $1 " | tee -a ${LOGFILE}

  else

    return 1

  fi

}
```



main.sh

```sh
#!/bin/bash

 
MODUPATH=$(dirname $(readlink -f$0))/module

echo ${MODUPATH}

export PATH=$PATH:${MODUPATH}

 

. test1.sh

. Config.sh

main()

{

 log_info 'This is the main shell'

 readConf

 echo${conf}

}

main

root@docker-host-03:~/test_shell# moremodule/Config.sh

#!/bin/bash

 

. test1.sh

readConf()

{

 conf="Read the config file"

}

root@docker-host-03:~/test_shell# ./main.sh

/root/test_shell/module

TEST IS

TEST IS

[ 2017-03-31 16:51:09 ] - main.sh - This isthe main shell

Read the config file

#我们可以看到这里多次输出了TEST IS内容。
```















