---
layout: post
title:  arm-linux-gcc工具链下载 
date: 2010-09-27 23:30:09 +0800
categories:
- Linux
tags:
- embedded
---

常用的交叉编译起可以从下边的站点下载：  
http://frank.harvard.edu/~coldwell/toolchain/  
http://www.kegel.com/crosstool/   

以下内容摘自：http://hi.baidu.com/zhxubo/blog/item/ee0987b41c66a0748ad4b263.html  

http://www.handhelds.org/download/projects/toolchain/  
http://ftp.arm.linux.org.uk/pub/armlinux/toolchain/  
http://so.hustonline.net/list.aspx?word=arm-linux-gcc-4.1.2&schoolInput=%CB%F9%D3%D0%D1%A7%D0%A3&schoolId=0&level=0  
http://ftp.snapgear.org:9981/pub/snapgear/tools/arm-linux/    3.4.1  
最常用的编译版本是arm-linux-gcc-3.4.1 和 arm-linux-3.3.2 的，现在的嵌入式开发基本上用的是这些，3.4.1的用于编译2.6的内核，而3.3.2的常用于编译busybox,和bootloader(u- boot),编译的版本配合不好的话就会出错，所以要选择好编译版本，如果这个版本不行的话，可以试试其他的版本,在uclinux上用的多的就是 arm-elf-tools-20030314  
http://www.handhelds.org/download/projects/toolchain/arm-linux-gcc-3.4.1.tar.bz2
http://www.handhelds.org/download/projects/toolchain/arm-linux-gcc-3.3.2.tar.bz2
如 果系统中又装了3.4.1和3.3.2的版本的话，可以在 .bashrc 中通过设置PATH来指定默认的版本为GCC3.4.1，然后再打开一个新的终端就可以用了，如果需要使用3.3.2的话，可以用具体的路径指定 (/usr/local/arm/3.3.2/bin/arm-linux-)。 
在~/.bashrc最后加入:   export PATH=$PATH:/usr/local/arm/3.4.1/bin    
如果编译u-boot或者busybox的时候指定 3.3.2的版本：  
CROSS_COMPILE=/usr/local/arm/3.3.2/bin/arm-linux-  
3.4.1的就直接用arm-linux-就可以了。  
arm-linux-gcc- 4.2.1的版本在 http://ftp.snapgear.org:9981/pub/snapgear/tools/arm-linux/ 这里可以下载，arm-linux-tools-20070808.tar.gz 这个可能是4.2.1的版本，因为下面有编译4.2.1的方法还有相应的代码包，build-arm-linux-4.2.1，此版本由于过大，我没有下载。
下面的这个是ARM官方给的下载链接4.2.1的http://www.codesourcery.com/gnu_toolchains/arm/download.html，然后将HOST选择为IA32 GNU/Linux，点击下载就可以了。不过前缀为arm-none-eabi-而不是arm-linux-有点郁闷。  

自己编译一个交叉编译环境是个很艰难的过程，有些软件又依赖不同的版本，所以自己维护一个是相当费时费力伤脑筋的过程，关于arm-linux- toolchain,arm-elf-toochain的区别，主要是编译过程中所用的C库的不同，arm-linux用的是glibc，arm-elf 用的是newlibc,ulibc等,具体的可以去网上搜集，一般编译arm+linux的用arm-linux-,而编译uclinux则用arm- elf-  
这是我在网上找到的一些下载交叉编译环境的网站  

```
0.http://ftp.arm.linux.org.uk/pub/armlinux/toolchain
[   ] cross-2.95.3.tar.bz2            20-Jul-2001 21:12   35M
[   ] cross-3.0.tar.bz2               20-Jul-2001 22:27   39M
[   ] cross-3.2.tar.bz2               23-Aug-2002 11:04   81M
[   ] cross-3.2.tar.gz                23-Aug-2002 10:01   93M
1.http://opensrc.sec.samsung.com/download.html      
GCC 3.4.0 based :
arm-uclinux-tools-base-gcc3.4.0-20040713.sh (binutil-2.15 based)
arm-uclinux-tools-c++-gcc3.4.0-20040713.sh
arm-uclinux-tools-gdb-20040713.sh      
GCC 2.95.3 based :
arm-elf-tools-20040427.sh by Hyok, Apr 27, 2004. (binutil-2.14/linux-2.6.5 based)
arm-elf-tools-20040305.sh by Hyok, Mar 5, 2004. (binutil-2.14 based)
   
2.http://www.handhelds.org/download/projects/toolchain/
[ ]arm-linux-gcc-3.3.2.tar.bz2                   03-Nov-2003 10:23   71M  
[ ]arm-linux-gcc-3.4.1.tar.bz2                   29-Jul-2004 14:01   41M
3.http://linux.omap.com/pub/toolchain/
[   ]obsolete-gcc-3.3.2.tar.bz2        15-May-2004 12:18   76M
4.http://ftp.snapgear.org:9981/pub/snapgear/tools/arm-linux/
5.http://www.uclinux.org/pub/uClinux/arm-elf-tools/
6.http://www.w-ww.org/~rmoravcik/sbc2410/toolchains/arm-linux-gcc-4.1.2-moko. 
```