---
layout: post
title: LINUX字符设备驱动程序实例(scull)
date: 2010-09-27 23:30:09 +0800
categories:
- 嵌入式Linux
tags:
- embedded
---

## 【1.系统环境】

该驱动程序在UBUNTU10.04LTS编译通过，系统内核为linux-2.6.32-24(可使用uname -r 命令来查看当前内核的版本号)  
由于安装UBUNTU10.04LTS时，没有安装LINUX内核源码，因此需要在www.kernel.org下载LINUX源码，下载linux-2.6.32.22.tar.bz2(与系统运行的LINUX内核版本尽量保持一致)，使用如下命令安装内核：  
1. 解压内核

```
cd /us/src
tar jxvf linux-2.6.32.22.tar.bz2
```

2. 为系统的include创建链接文件

```
cd /usr/include
rm -rf asm linux scsi
ln -s /usr/src/linux-2.6.32.22/include/asm-generic asm
ln -s /usr/src/linux-2.6.32.22/include/linux linux
ln -s /usr/src/linux-2.6.32.22/include/scsi scsi
```

LINUX内核源码安装完毕

## 【2.驱动程序代码】

```c
/******************************************************************************
*Name: memdev.c
*Desc: 字符设备驱动程序的框架结构，该字符设备并不是一个真实的物理设备，
* 而是使用内存来模拟一个字符设备
*Parameter: 
*Return:
*Author: yoyoba(stuyou@126.com)
*Date: 2010-9-26
*Modify: 2010-9-26
********************************************************************************/
#include <linux/module.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <asm/io.h>
#include <asm/system.h>
#include <asm/uaccess.h>

#include "memdev.h"

static mem_major = MEMDEV_MAJOR;

module_param(mem_major, int, S_IRUGO);

struct mem_dev *mem_devp; /*设备结构体指针*/

struct cdev cdev; 

/*文件打开函数*/
int mem_open(struct inode *inode, struct file *filp)
{
    struct mem_dev *dev;
    
    /*获取次设备号*/
    int num = MINOR(inode->i_rdev);

    if (num >= MEMDEV_NR_DEVS) 
            return -ENODEV;
    dev = &mem_devp[num];
    
    /*将设备描述结构指针赋值给文件私有数据指针*/
    filp->private_data = dev;
    
    return 0; 
}

/*文件释放函数*/
int mem_release(struct inode *inode, struct file *filp)
{
  return 0;
}

/*读函数*/
static ssize_t mem_read(struct file *filp, char __user *buf, size_t size, loff_t *ppos)
{
  unsigned long p = *ppos;
  unsigned int count = size;
  int ret = 0;
  struct mem_dev *dev = filp->private_data; /*获得设备结构体指针*/

  /*判断读位置是否有效*/
  if (p >= MEMDEV_SIZE)
    return 0;
  if (count > MEMDEV_SIZE - p)
    count = MEMDEV_SIZE - p;

  /*读数据到用户空间*/
  if (copy_to_user(buf, (void*)(dev->data + p), count))
  {
    ret = - EFAULT;
  }
  else
  {
    *ppos += count;
    ret = count;
    
    printk(KERN_INFO "read %d bytes(s) from %d\n", count, p);
  }

  return ret;
}

/*写函数*/
static ssize_t mem_write(struct file *filp, const char __user *buf, size_t size, loff_t *ppos)
{
  unsigned long p = *ppos;
  unsigned int count = size;
  int ret = 0;
  struct mem_dev *dev = filp->private_data; /*获得设备结构体指针*/
  
  /*分析和获取有效的写长度*/
  if (p >= MEMDEV_SIZE)
    return 0;
  if (count > MEMDEV_SIZE - p)
    count = MEMDEV_SIZE - p;
    
  /*从用户空间写入数据*/
  if (copy_from_user(dev->data + p, buf, count))
    ret = - EFAULT;
  else
  {
    *ppos += count;
    ret = count;
    
    printk(KERN_INFO "written %d bytes(s) from %d\n", count, p);
  }

  return ret;
}

/* seek文件定位函数 */
static loff_t mem_llseek(struct file *filp, loff_t offset, int whence)
{ 
    loff_t newpos;

    switch(whence) {
      case 0: /* SEEK_SET */
        newpos = offset;
        break;

      case 1: /* SEEK_CUR */
        newpos = filp->f_pos + offset;
        break;

      case 2: /* SEEK_END */
        newpos = MEMDEV_SIZE -1 + offset;
        break;

      default: /* can't happen */
        return -EINVAL;
    }
    if ((newpos<0) || (newpos>MEMDEV_SIZE))
     return -EINVAL;
     
    filp->f_pos = newpos;
    return newpos;

}

/*文件操作结构体*/
static const struct file_operations mem_fops =
{
  .owner = THIS_MODULE,
  .llseek = mem_llseek,
  .read = mem_read,
  .write = mem_write,
  .open = mem_open,
  .release = mem_release,
};

/*设备驱动模块加载函数*/
static int memdev_init(void)
{
  int result;
  int i;

  dev_t devno = MKDEV(mem_major, 0);

  /* 静态申请设备号*/
  if (mem_major)
    result = register_chrdev_region(devno, 2, "memdev");
  else /* 动态分配设备号 */
  {
    result = alloc_chrdev_region(&devno, 0, 2, "memdev");
    mem_major = MAJOR(devno);
  } 
  
  if (result < 0)
    return result;

  /*初始化cdev结构*/
  cdev_init(&cdev, &mem_fops);
  cdev.owner = THIS_MODULE;
  cdev.ops = &mem_fops;
  
  /* 注册字符设备 */
  cdev_add(&cdev, MKDEV(mem_major, 0), MEMDEV_NR_DEVS);
   
  /* 为设备描述结构分配内存*/
  mem_devp = kmalloc(MEMDEV_NR_DEVS * sizeof(struct mem_dev), GFP_KERNEL);
  if (!mem_devp) /*申请失败*/
  {
    result = - ENOMEM;
    goto fail_malloc;
  }
  memset(mem_devp, 0, sizeof(struct mem_dev));
  
  /*为设备分配内存*/
  for (i=0; i < MEMDEV_NR_DEVS; i++) 
  {
        mem_devp[i].size = MEMDEV_SIZE;
        mem_devp[i].data = kmalloc(MEMDEV_SIZE, GFP_KERNEL);
        memset(mem_devp[i].data, 0, MEMDEV_SIZE);
  }
    
  return 0;

  fail_malloc: 
  unregister_chrdev_region(devno, 1);
  
  return result;
}

/*模块卸载函数*/
static void memdev_exit(void)
{
  cdev_del(&cdev); /*注销设备*/
  kfree(mem_devp); /*释放设备结构体内存*/
  unregister_chrdev_region(MKDEV(mem_major, 0), 2); /*释放设备号*/
}

MODULE_AUTHOR("David Xie");
MODULE_LICENSE("GPL");

module_init(memdev_init);
module_exit(memdev_exit);

```

```c
/************************
*memdev.h
************************/
#ifndef _MEMDEV_H_
#define _MEMDEV_H_

#ifndef MEMDEV_MAJOR
#define MEMDEV_MAJOR 260 /*预设的mem的主设备号*/
#endif

#ifndef MEMDEV_NR_DEVS
#define MEMDEV_NR_DEVS 2 /*设备数*/
#endif

#ifndef MEMDEV_SIZE
#define MEMDEV_SIZE 4096
#endif

/*mem设备描述结构体*/
struct mem_dev 
{ 
  char *data; 
  unsigned long size; 
};

#endif /* _MEMDEV_H_ */
```

## 【3.编译驱动程序模块】

Makefile文件的内容如下：

```
ifneq ($(KERNELRELEASE),)

obj-m:=memdev.o

else

KERNELDIR:=/lib/modules/$(shell uname -r)/build

PWD:=$(shell pwd)

default:

 $(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:

 rm -rf *.o *.mod.c *.mod.o *.ko

endif
```

切换到root下，执行make时，如果UBUNTU是使用虚拟机安装的，那么执行make时，不要在ubuntu和windows的共享目录下，否则会出错。

```
root@VMUBUNTU:~# make
make -C /lib/modules/2.6.32-24-generic/build M=/root modules
make[1]: Entering directory `/usr/src/linux-headers-2.6.32-24-generic'
  CC [M] /root/memdev.o
/root/memdev.c:15: warning: type defaults to ‘int’ in declaration of ‘mem_major’
/root/memdev.c: In function ‘mem_read’:
/root/memdev.c:71: warning: format ‘%d’ expects type ‘int’, but argument 3 has type ‘long unsigned int’
/root/memdev.c: In function ‘mem_write’:
/root/memdev.c:99: warning: format ‘%d’ expects type ‘int’, but argument 3 has type ‘long unsigned int’
  Building modules, stage 2.
  MODPOST 1 modules
  CC /root/memdev.mod.o
  LD [M] /root/memdev.ko
make[1]: Leaving directory `/usr/src/linux-headers-2.6.32-24-generic'
```

ls查看当前目录的内容

```
root@VMUBUNTU:~# ls
Makefile memdev.h memdev.mod.c memdev.o Module.symvers
memdev.c memdev.ko memdev.mod.o modules.order
```

这里的memdev.ko就是生成的驱动程序模块。  
通过insmod命令把该模块插入到内核  

```
root@VMUBUNTU:~# insmod memdev.ko
```

查看插入的memdev.ko驱动

```
root@VMUBUNTU:~# cat /proc/devices
Character devices:
  1 mem
  4 /dev/vc/0
  4 tty
  4 ttyS
  5 /dev/tty
  5 /dev/console
  5 /dev/ptmx
260 memdev
  6 lp
  7 vcs
 10 misc
 13 input
 14 sound
 21 sg
 29 fb
 99 ppdev
108 ppp
116 alsa
128 ptm
136 pts
180 usb
189 usb_device
226 drm
251 hidraw
252 usbmon
253 bsg
254 rtc

Block devices:
  1 ramdisk
259 blkext
  7 loop
  8 sd
  9 md
 11 sr
 65 sd
 66 sd
 67 sd
 68 sd
 69 sd
 70 sd
 71 sd
128 sd
129 sd
130 sd
131 sd
132 sd
133 sd
134 sd
135 sd
252 device-mapper
253 pktcdvd
254 mdp
```

可以看到memdev驱动程序被正确的插入到内核当中，主设备号为260，该设备号为memdev.h中定义的

```
#define MEMDEV_MAJOR 260。
```

如果这里定义的主设备号与系统正在使用的主设备号冲突，比如主设备号定义如下：#define MEMDEV_MAJOR 254，那么在执行insmod命令时，就会出现如下的错误：

```
root@VMUBUNTU:~# insmod memdev.ko
insmod: error inserting 'memdev.ko': -1 Device or resource busy
```

查看当前设备使用的主设备号

```
root@VMUBUNTU:~# cat /proc/devices
Character devices:
  1 mem
  4 /dev/vc/0
  4 tty
  4 ttyS
  5 /dev/tty
  5 /dev/console
  5 /dev/ptmx
  6 lp
  7 vcs
 10 misc
 13 input
 14 sound
 21 sg
 29 fb
 99 ppdev
108 ppp
116 alsa
128 ptm
136 pts
180 usb
189 usb_device
226 drm
251 hidraw
252 usbmon
253 bsg
254 rtc

Block devices:
  1 ramdisk
259 blkext
  7 loop
  8 sd
  9 md
 11 sr
 65 sd
 66 sd
 67 sd
 68 sd
 69 sd
 70 sd
 71 sd
128 sd
129 sd
130 sd
131 sd
132 sd
133 sd
134 sd
135 sd
252 device-mapper
253 pktcdvd
254 mdp
```

发现字符设备的254主设备号为rtc所使用，因此会出现上述错误，解决方法只需要在memdev.h中修改主设备号的定义即可。

## 【4.编写应用程序，测试该驱动程序】

首先应该在/dev/目录下创建与该驱动程序相对应的文件节点，使用如下命令创建：

```
root@VMUBUNTU:/dev# mknod memdev c 260 0
```

使用ls查看创建好的驱动程序节点文件

```
root@VMUBUNTU:/dev# ls -al memdev
crw-r--r-- 1 root root 260, 0 2010-09-26 17:28 memdev
```

编写如下应用程序，来对驱动程序进行测试。

```c
/******************************************************************************
*Name: memdevapp.c
*Desc: memdev字符设备驱动程序的测试程序。先往memedev设备写入内容，然后再
* 从该设备中把内容读出来。
*Parameter: 
*Return:
*Author: yoyoba(stuyou@126.com)
*Date: 2010-9-26
*Modify: 2010-9-26
********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <linux/i2c.h>
#include <linux/fcntl.h>

int main()
{
 int fd;
 char buf[]="this is a example for character devices driver by yoyoba!";//写入memdev设备的内容

 char buf_read[4096]; //memdev设备的内容读入到该buf中

 
 if((fd=open("/dev/memdev",O_RDWR))==-1) //打开memdev设备

  printf("open memdev WRONG！\n");
 else
  printf("open memdev SUCCESS!\n");
  
 printf("buf is %s\n",buf); 

 write(fd,buf,sizeof(buf)); //把buf中的内容写入memdev设备

 
 lseek(fd,0,SEEK_SET); //把文件指针重新定位到文件开始的位置

 
 read(fd,buf_read,sizeof(buf)); //把memdev设备中的内容读入到buf_read中

 
 printf("buf_read is %s\n",buf_read);
 
 return 0;
}
```

编译并执行该程序

```
root@VMUBUNTU:/mnt/xlshare# gcc -o mem memdevapp.c

root@VMUBUNTU:/mnt/xlshare# ./mem
open memdev SUCCESS!
buf is this is a example for character devices driver by yoyoba!
buf_read is this is a example for character devices driver by yoyoba!
```

表明驱动程序工作正常。。。

## 【5.LINUX是如何make驱动程序模块的】

Linux内核是一种单体内核，但是通过动态加载模块的方式，使它的开发非常灵活 方便。那么，它是如何编译内核的呢？我们可以通过分析它的Makefile入手。以下是 一个简单的hello内核模块Makefile.

``` 
ifneq ($(KERNELRELEASE),)
obj-m:=hello.o
else
KERNELDIR:=/lib/modules/$(shell uname -r)/build
PWD:=$(shell pwd)
default:
        $(MAKE) -C $(KERNELDIR)  M=$(PWD) modules
clean:
        rm -rf *.o *.mod.c *.mod.o *.ko
endif
```

当我们写完一个hello模块，只要使用以上的makefile。然后make一下就行。 假设我们把hello模块的源代码放在/home/study/prog/mod/hello/下。 当我们在这个目录运行make时，make是怎么执行的呢？ LDD3第二章第四节“编译和装载”中只是简略地说到该Makefile被执行了两次， 但是具体过程是如何的呢？   
首先，由于make 后面没有目标，所以make会在Makefile中的第一个不是以.开头 的目标作为默认的目标执行。于是default成为make的目标。make会执行 $(MAKE) -C $(KERNELDIR) M=$(PWD) modules shell是make内部的函数,假设当前内核版本是2.6.13-study,所以$(shell uname -r)的结果是 2.6.13-study 这里，实际运行的是 
make -C /lib/modules/2.6.13-study/build M=/home/study/prog/mod/hello/ modules
/lib/modules/2.6.13-study/build是一个指向内核源代码/usr/src/linux的符号链接。 可见，make执行了两次。第一次执行时是读hello模块的源代码所在目录/home/s tudy/prog/mod/hello/下的Makefile。第二次执行时是执行/usr/src/linux/下的Makefile时.   
但是还是有不少令人困惑的问题：   
1. 这个KERNELRELEASE也很令人困惑，它是什么呢？在/home/study/prog/mod/he llo/Makefile中是没有定义这个变量的，所以起作用的是else…endif这一段。不 过，如果把hello模块移动到内核源代码中。例如放到/usr/src/linux/driver/中， KERNELRELEASE就有定义了。 在/usr/src/linux/Makefile中有 162 KERNELRELEASE=$(VERSION).$(PATCHLEVEL).$(SUBLEVEL)$(EXTRAVERSION)$(LOCALVERSION) 这时候，hello模块也不再是单独用make编译，而是在内核中用make modules进行 编译。 用这种方式，该Makefile在单独编译和作为内核一部分编译时都能正常工作。 
2. 这个obj-m := hello.o什么时候会执行到呢？ 在执行： 
make -C /lib/modules/2.6.13-study/build M=/home/study/prog/mod/hello/ modules
时，make 去/usr/src/linux/Makefile中寻找目标modules: 862 .PHONY: modules 863 modules: $(vmlinux-dirs) $(if $(KBUILD_BUILTIN),vmlinux) 864 @echo ' Building modules, stage 2.'; 865 $(Q)$(MAKE) -rR -f $(srctree)/scripts/Makefile.modpost 
可以看出，分两个stage:    
(1)编译出hello.o文件。   
(2)生成hello.mod.o hello.ko 在这过程中，会调用 make -f scripts/Makefile.build obj=/home/study/prog/mod/hello 而在 scripts/Makefile.build会包含很多文件： 011 -include .config 012 013 include $(if $(wildcard $(obj)/Kbuild), $(obj)/Kbuild, $(obj)/Makefile) 其中就有/home/study/prog/mod/hello/Makefile 这时 KERNELRELEASE已经存在。 所以执行的是： obj-m:=hello.o    
关于make modules的更详细的过程可以在scripts/Makefile.modpost文件的注释 中找到。如果想查看make的整个执行过程，可以运行make -n。