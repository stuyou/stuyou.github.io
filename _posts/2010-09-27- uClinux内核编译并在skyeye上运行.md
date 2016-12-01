---
layout: post
title:  uClinux内核编译并在skyeye上运行
date: 2010-09-27 23:30:09 +0800
categories:
- 嵌入式Linux
tags:
- embedded
- skyeye
---

在进行uClinux内核编译之前，要保证arm-elf-tools工具链安装正确，并在PATH中指定路径。  
首先下载uClinux源码，http://sourceforge.net/projects/uclinux/files/ ，我下载的是uClinux-dist-20100315.tar.bz2源码包。并把源码包放在/home目录中。uClinux源码安装在/usr/local/src目录中。  

```
cd /usr/local/src
tar -xjvf /home/uClinux-dist-20100315.tar.bz2
```

解压缩完成以后，uClinux源码成功安装，在/usr/local/src/uClinux-dist目录下执行编译命令  

```
sudo make xconfig
```  

这时会出现错误，是因为libbfd-2.20.1-system.20100303.so库文件没有找到，这是因为在安装skyeye时，把libbfd-2.20.1-system.20100303.so替换成了libbfd-2.19.90.20090909.so，所以在/usr/lib目录下，重新恢复libbfd-2.19.90.20090909.so包即可。  
重新执行，sudo make xconfig，出错，提示缺少libglade-2.0包，使用以下命令，安装libglade-2.0包  

```
sudo apt-get install libgtk2.0-dev libglade2-dev firefox-dev libchm-dev libssl-dev
```
  
重新执行sudo make xconfig   
看到了uClinux内核配置界面。在内核配置中，vendors选择Samsung，内核选择linux-2.4.x，函数库选择uClibc,保存并退出，   
sudo make dep，没有错误   
sudo make clean 没有错误   
sudo make lib_only，出错，  
 
```
find . -depth -type d | grep -v .svn | xargs rmdir > /dev/null 2>&1 || exit 0
Making symlinks in include/
/bin/sh: ucfront-gcc：找不到命令
Making include/c++ symlink to compiler c++ includes
/bin/sh: ucfront-gcc：找不到命令
ln: 正在创建指向 ‘c++’ 的符号链接 ‘./c++’: File exists
make[2]: *** [all] 错误 1
make[2]: Leaving directory `/curret1/uClinux-dist/include'
make[1]: *** [all] 错误 2
make[1]: Leaving directory `/curret1/uClinux-dist/lib'
```

提示ucfront-gcc找不到，解决方法：   

```
$ cd ../uClinux-dist/tools/ucfront
$ make   
```

将会生成ucfront可执行文件   

```
$ mv ucfront ucfront-gcc
$ cp ucfront-gcc /usr/local/bin
$ cd uClinux-dist
$ make lib_only
```

不再出现“ucfront-gcc找不到”提示。但会出现arm-linux-gcc找不到提示。   
这是因为在uClinux-dist-20100315.tar.bz2包中采用到交叉编译器是arm-linux-gcc，而不是arm-elf-gcc，开始安装到开发工具链是arm-elf-gcc类型到，所以会提示找不到arm-linux-gcc。可在/verdors/config/armnommu/cofig.arch文件中找到定义的交叉编译器到类型。因此在下载了uclinux源码包后，应该查看一下这个文件，根据这个文件安装相应到交叉编译器。   
解决方法，重新下载低版本的uClinux或者安装arm-linux-gcc工具链。   
到http://www.uclinux.org/pub/uClinux/dist/ 下载arm-linux-gcc工具链，安装   
到http://www.uclinux.org/pub/uClinux/dist/ 下载旧版到uclinux源码包，这里下载uClinux-dist-20051110.tar.gz   
删除uClinux-dist-20100315:   

```
cd /usr/local/src
sudo rm -rf ./uClinux-dist

```
重新安装uClinux-dist-20051110   

```
sudo tar -xzvf /home/uClinux-dist-20100315.tar.gz
```

sudo make xconfig，vendors选择Samsug，内核选择linux-2.4.x,内核选择uClibc，保存退出   
sudo make dep，正常   
sudo make clean,正常   
sudo make lib_only,正常   
sudo make user_only，正常    
sudo make romfs，正常   
sudo make image，出错，提示：arm-elf-objcopy: /home/pbman/uClinux-dist/ linux-2.4.x/linux: No such file or directory   
第一次make image出错是正常的，因为没有生成linux   
make出错是因为linux-2.4.x/drivers/block/blkmem.c中使用了romfs_data，而链接脚本中没有。可以这样解决：修改blkmem.c中   

```
#ifdef CONFIG_BOARD_SNDS100
{0, romfs_data, -1},
#endif
为
#ifdef CONFIG_BOARD_SNDS100
{0, 0x00300000, -1},
#endif
```

此处的 0x00300000为你的romfs加载的位置，可以自己决定。这样在加载内核时，先加载romfs到 0x00300000，在加载内核并执行。   
当然也可修改链接脚本加入romfs_data

也可以不管make image出现的错误，执行sudo make，如果成功得到image文件，则编译成功。   
不知道什么问题，按照这种方法，编译成功，但在使用skyeye运行时，skyeye配置文件搞不定，放弃！   

skyeyetestsuits中有at91的配置脚本，为了利用这个配置脚本，因此uclinux重新配置vendors，选择GDB/Armulator
```
sudo make dep
sudo make
```
编译成功，看一下images目录的内容  
```
stuyou@UBUNTU:/usr/local/src/uClinux-dist/images$ ls
image.bin linux.data linux.text romfs.img
```
linux内核文件在uClinux-dist/linux-2.4.x/目录下，  
拷贝linux到images文件夹下，
```
sudo cp ./linux-2.4.x/linux ./images
```
在iamges下建立skyeye配置文件   
```
sudo vi skyeye.conf
```
skyeye.conf配置脚本内容如下   
```
#skyeye config file sample
arch:arm
cpu: arm7tdmi

mach: at91

mem_bank: map=M, type=RW, addr=0x00000000, size=0x00004000
mem_bank: map=M, type=RW, addr=0x01000000, size=0x00400000
mem_bank: map=M, type=R, addr=0x01400000, size=0x00400000, file=./romfs.img
mem_bank: map=M, type=RW, addr=0x02000000, size=0x00400000
mem_bank: map=M, type=RW, addr=0x02400000, size=0x00008000
mem_bank: map=M, type=RW, addr=0x04000000, size=0x00400000
mem_bank: map=I, type=RW, addr=0xf0000000, size=0x10000000
#set nic info
#net: type=cs8900a, base=0xfffa0000, size=0x20,int=16, mac=0:4:3:2:1:f, ethmod=tuntap, hostip=10.0.0.1
net: type=cs8900a, ethmod=tuntap, hostip=10.0.0.1
uart: mod = term
#dbct: state=on
```
然后在iamges目录下执行   
```
skyeye -c skyeye.conf -e linux
```
uClinux成功启动。    
uClinux编译成功！！