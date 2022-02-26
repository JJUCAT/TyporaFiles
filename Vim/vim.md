## VIM 常用配置

#### vim 配置目录

修改 vim 全局配置：`/etc/vim/vimrc` 或者 `/etc/vimrc`

修改 vim 用户配置：`~/.vimrc`



#### vim 配置参数

`set incsearch` 搜索自动跳转到第一个匹配的关键词那

`set hlsearch` 高亮搜索

`set showmatch` 遇到括号，自动高亮对应的另一个括号

`set cursorline` 高亮当前行

`set showcmd` 底部显示输入的指令

`set showmode` 底部显示命令模式还是插入模式

`syntax on` 语法高亮

`set encoding=utf-8` 用 utf-8 编码

`set mouse=a` 支持鼠标

`set number` 显示行号

`set nonumber` 不显示行号

`set ts=4` 设置TAB为 4 个空格

`set softtabstop=4` 退格的缩进

`set shiftwidth=4` 每级的缩进

`set autoindent` 自动缩进

`set laststatus=2` 显示文件名

`set expandtab` 插入模式下 Tab 输入空格

`set tabstop=4` 输入的 Tab 为 4 个空格

`set smartautoindent` 比较好的自动缩进，如果打开了 `expandtab` ，用空格缩进







## VIM 键盘图

![](vi-vim-cheat-sheet-sch.gif)



## VIM常用操作

#### 光标移动

`h` 或者 `←` 光标左移一个字符

`j` 或者 `↓` 光标下移一个字符

`k` 或者 `↑` 光标上移一个字符

`l` 或者 `→` 光标右移一个字符


`0` 或者 `Home` 光标移动到行首
`$` 或者 `End` 光标移动到行尾



`H` 光标移动到屏幕最上方行的第一个字符

`M` 光标移动到屏幕中间行的第一个字符

`L` 光标移动到屏幕最下方行的第一个字符

`G` 光标移动到文档最后一行

`nG` 光标移动到文档第 `n` 行

`gg` 光标移动到文档第一行



#### 搜索替换

`/word` 向光标下搜索 `word` 的字符串

`?word` 向光标上搜索 `word` 的字符串

`n` 向下搜索上次的关键词

`N` 向上搜索上次的关键词



`:n1,n2s/word1/word2/g` 在第 n1 行和第 n2 行搜索 word1 关键词，替换为 word2

e.g. `:100,200s/key/KEY/g`

`:1,$s/word1/word2/g` 全文档把 `word1` 替换为 `word2`

`1,$s/word1/world2/gc` 全文档把 `word1` 替换为 `word2` ,在替换前会询问客户



#### 删除复制粘贴

`dd` 删除光标所在行数据

`ndd` 删除光标下（包括光标所在行） `n` 行数据

`d1G` 删除光标到第一行数据

`dG` 删除光标到最后一行数据

`yy` 复制光标所在行数据

`nyy` 复制光标下（包括光标所在行）`n` 行数据

`y1G` 复制光标到第一行数据

`yG` 复制光标到最后一行数据

`p` 把复制的数据粘贴到光标下行

`P` 把复制的数据粘贴到光标上行



#### 编辑

`u` 撤回

`ctrl+r` 重做

`.` 重复上个动作



#### 选择

`v+光标移动` 选择



#### 另存

`:w [newfile]` 另存为 `newfile`



#### 暂时返回终端执行命令

`:! cmd` 到终端执行 `cmd`



#### 多窗口操作

`:vsp newfile` 新建 `newfile` 文件并垂直分割显示

`:new newfile` 新建 `newfile` 文件并水平分割显示

`vim -o file1 file2` 以水平分割显示的方式打开两个文件

`vim -O file1 file2` 以垂直分割显示的方式打开两个文件

`ctrl+w h` 窗口左移

`ctrl+w j` 窗口下移

`ctrl+w k` 窗口上移

`ctrl+w l` 窗口右移

`ctrl+w =` 多窗口几乎相等

`ctrl+w >` 窗口宽度增加

`ctrl+w <` 窗口宽度减小

`ctrl+w +` 窗口高度增加

`ctrl+w -` 窗口高度减小



以 16 进制查看文件

`%!xxh`

