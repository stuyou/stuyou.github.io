---
layout: post
title: 编译安装带有vimgdb调试器的VIM
date: 2010-09-27 23:30:09 +0800
categories:
- Linux
tags:
- vim
- ubuntu10.04
---

## 安装环境

UBUNTU10.04LTS，而且已经通过sudo apt-get install vim安装好了源里的VIM,通过UBUNTU源安装的VIM不带有GDB调试功能，下面自己编译安装一个带有GDB调试功能的VIM

## 安装相应库

1. 安装编译工具

```
sudo apt-get install build-essential
```

2. 安装编译vim－gtk的依赖包

```
sudo apt-get build-dep vim-gtk
```

3. 安装ncurses开发包

```
sudo apt -get install libncurses5-dev
```

如果不安装ncurses开发包，make时会产生如下错误：

```
no terminal library found
checking for tgetent()... configure: error: NOT FOUND!
      You need to install a terminal library; for example ncurses.
      Or specify the name of the library with --with-tlib. 
```
 
## 下载VIM源码及配置

1. http://www.vim.org/sources.php 下载当前最新的VIM 7.2的源代码，假设我们把代码放到~/install/目录，文件名为vim-7.2.tar.bz2
2. 下载vimgdb补丁。接下来，我们需要下载vimgdb补丁，下载页面在：
http://sourceforge.net/project/showfiles.php?group_id=111038&package_id=120238
 在这里，选择vim 7.2的补丁，把它保存到~/install/vimgdb72-1.14.tar.gz。
3. 打补丁。运行以下命令，解压VIM源码，并打上补丁

```
cd ~/install
tar xjvf vim-7.2.tar.bz2，会在~/install目录下生成一个vim72目录
tar xzvf vimgdb72-1.14.tar.gz，会在~/install目录下生成一个vimgdb目录
patch -d vim72 --backup -p0 < vimgdb/vim72.diff
```

4. 定制VIM功能.缺省的VIM配置已经适合大多数人，但有些时候你可能需要一些额外的功能，这时就需要自己定制一下VIM。定制VIM很简单，进入~/install/vim72/src文件，编辑Makefile文件。这是一个注释很好的文档，根据注释来选择：

```
cd ~/install/vim72/src
sudo vim Makefile
```

然后修改配置   
-gtk2支持,也能使用gnome，打开--enable-gui=gkt2   
-最大特性支持,打开--with-features=huge(必须打开，否则编译成功vim，运行后设置语法高亮时，产生如下错误   

```
Vim: Caught deadly signal ABRT
 Vim: Finished.
 Aborted)
```
 - 如果你想把perl, python, tcl, ruby等接口编译进来的话，打开相应的选项，例如，我打开了--enable-tclinterp选项；
- 如果你想在VIM中使用cscope的话，打开--enable-cscope选项；
- 我们刚才打的vimgdb补丁，自动在Makefile中加入了--enable-gdb选项；
- 如果你希望在vim使用中文，使能--enable-multibyte和--enable-xim选项；
- 可以通过--with-features=XXX选项来选择所编译的VIM特性集，缺省是--with-features=normal；
- 如果你没有root权限，可以把VIM装在自己的home目录，这时需要打开prefix = $(HOME)选项；这里我打开了这一项，把VIM安装在home目录下。   
 编辑好此文件后，就执行可以编辑安装vim了。如果你需要更细致的定制VIM，可以修改config.h文件，打开/关闭你想要的特性。

## 编译安装

在~/install/vim72/src目录下执行

```
make CFLAGS="-O2 -D_FORTIFY_SOURCE=1"
```

执行make时，加上CFLAGS="-O2 -D_FORTIFY_SOURCE=1"选项，如果直接执行make，VIM也能编译成功，但运行时会出现“Vim: Caught deadly signal ABRT”错误，具体请参考“http://forum.ubuntu.org.cn/viewtopic.php?f=122&t=240806”   
make执行完成之后，安装编译好的VIM，在~/install/vim72/src目录下执行如下命令

```
sudo make install
```

这样，VIM就被安装到了~/bin目录下，其它相关文件在~/share/vim目录下   
~/bin目录的内容
 
```
~$ ls -al bin
total 1780
drwxr-xr-x  2 root   root      4096 2010-05-23 23:43 .
drwxr-xr-x 42 stuyou stuyou    4096 2010-05-24 00:03 ..
lrwxrwxrwx  1 root   root         3 2010-05-23 23:43 eview -> vim
lrwxrwxrwx  1 root   root         3 2010-05-23 23:43 evim -> vim
lrwxrwxrwx  1 root   root         3 2010-05-23 23:43 ex -> vim
lrwxrwxrwx  1 root   root         3 2010-05-23 23:43 gview -> vim
lrwxrwxrwx  1 root   root         3 2010-05-23 23:43 gvim -> vim
lrwxrwxrwx  1 root   root         3 2010-05-23 23:43 gvimdiff -> vim
-rwxr-xr-x  1 root   root       143 2010-05-23 23:43 gvimtutor
lrwxrwxrwx  1 root   root         3 2010-05-23 23:43 rgview -> vim
lrwxrwxrwx  1 root   root         3 2010-05-23 23:43 rgvim -> vim
lrwxrwxrwx  1 root   root         3 2010-05-23 23:43 rview -> vim
lrwxrwxrwx  1 root   root         3 2010-05-23 23:43 rvim -> vim
lrwxrwxrwx  1 root   root         3 2010-05-23 23:43 view -> vim
-rwxr-xr-x  1 root   root   1789364 2010-05-23 23:43 vim
lrwxrwxrwx  1 root   root         3 2010-05-23 23:43 vimdiff -> vim
-rwxr-xr-x  1 root   root      2084 2010-05-23 23:43 vimtutor
-rwxr-xr-x  1 root   root     13900 2010-05-23 23:43 xxd
```

VIM编译安装成功

## 安装vimgdb的runtime文件

运行下面的命令，解压vimgdb的runtime文件到你的~/.vim/目录，如果没有~/.vim目录，则执行命令mkdir ~/.vim创建

```
cd ~/install/vimgdb/
tar zxf vimgdb_runtime.tgz –C~/.vim/
```

切换到~/bin目录下，启动我们自己编译的VIM，注意：这是不能直接通过输入VIM命令来启动刚才编译的VIM，因为在编译安装VIM之前，已经通过UBUNTU的源安转过了VIM，所以要想启动刚才编译安装的VIM，应该进入到~/bin目录下，通过如下方式启动

```
:~$ cd ~/bin
~/bin$ ./vim
```

在VIM中运行下面的命令以生成帮助文件索引：

```
 :helptags ~/.vim/doc
```

现在，你可以使用“:help vimgdb”命令查看vimgdb的帮助了。   
至此，我们重新编译了VIM，并为之打上了vimgdb补丁

## 简单配置VIM

编译安装的VIM，不能高亮显示C语言语法，不能显示行号，对gdb的按键没有映射。因此为了实现上述配置，在~目录下建立.vimrc文件

```
vim ~/.vimrc
```

然后输入以下内容：

```
  nmap :run macros/gdb_mappings.vim
syntax on
set nu
```

保存退出

## 把编译的VIM代替先前的VIM

执行如下命令，就可以把自己编译的VIM，代替以前安装的VIM，以后启动VIM时，就不需要在切换到~/bin目录下，直接输入VIM，就可以启动自己编译的VIM

```
sudo rm /etc/alternatives/vim  
sudo rm /etc/alternatives/vimdiff 
sudo ln -s ~/bin/vim /etc/alternatives/vim  
sudo ln -s ~/bin/vimdiff /etc/alternatives/vimdiff  
```

## 程序调试
 假设在主目录下建立test.c测试文件，内容如下：

```c
#include

int main()
{
printf("Hello World!!!\n");
return 0;
}
```
在主目录下创建Makefile文件，内容如下：

```
test:test.c
       gcc -g -o test test.c
```

然后用VIM打开test.c，假设VIM的当前工作目录为~（如果不是，使用:cd ~命令切换）  
在VIM中，输入命令:make，使用Makefile对test.c进行编译，如果编译出错，VIM会跳到第一个出错的位置，改完后，用“:cnext”命令跳到下一个错误，以此类推。  
现在，假设已经完成链接，生成了最终的test文件，  
好了，我们现在按空格键，在当前窗口下方会打开一个小窗口(command-line窗口)，这就是vimgdb的命令窗口，可以在这个窗口中输入任何合法的gdb命令，输入的命令将被送到gdb执行。现在，我们在这个窗口中输入“gdb”，按回车后，command-line窗口自动关闭，而在当前窗口上方又打开一个窗口，这个窗口是gdb输出窗口。  
具体调试细节请参考：http://easwy.com/blog/archives/advanced-vim-skills-catalog/