---
layout: post
title: Edukit-III实验箱s3c2410子板led驱动程序编译运行
date: 2010-09-30 23:30:09 +0800
categories:
- 嵌入式Linux
tags:
- embedded
---

## 开发环境

开发板：EDUKIT-III实验箱，S3C2410+LINUX2.4.18，实验箱随箱光盘提供的Zimage，nor flash启动。  
主机：ubnutn10.4LTS，arm-linux-gcc 2.95.3，linux内核为随箱提供的内核源码和补丁文件，内核版本号为2.4.18  
 
## [1.系统环境配置]

参考http://blog.chinaunix.net/u3/119151/showart_2340356.html
 
## [2.编译过程]

在编译led驱动程序之前，必须要先编译内核成功，而且最好不要在虚拟机Linux下进行  
 
拷贝edukit-III实验箱配套光盘上的led驱动源程序（led-edukit-s3c2410.c）和Makefile文件，放入/root下，在root用户下对其进行编译  
 
首先修改Makefile，把Makefile的第4行改为“WKDIR  = /usr/src”，这个路径为linux内核安装目录，把第5行改为“CROSSDIR = /usr/local/arm/2.95.3”，这个路径为交叉工具链安装路径，修改后的Makefile文件内容如下

```
#
# Makefile for the kernel i2c driver (Module).
#
WKDIR        = /usr/src
CROSSDIR    = /usr/local/arm/2.95.3
INSTALLDIR    = /home/app
#$(WKDIR)/drivers

#                                    /* output module name */
MODDEV        = led.o
#                                    /* source file(s) */
MODFILE        = led-edukit-s3c2410.c
#                                    /* header file(s) */
MODFILE_H    = 

CROSS=arm-linux-
CC = $(CROSS)gcc
AS = $(CROSS)as
LD = $(CROSS)ld

MACRO = -DMODULE -D__KERNEL__ -DCONFIG_KERNELD

ifdef DEBUG
CFLAGS = -g
endif
CFLAGS = -O2 -fomit-frame-pointer

CFLAGS += $(MACRO) -mapcs-32 -march=armv4 -mtune=arm9tdmi -fno-builtin

INCLUDES = -I$(WKDIR)/kernel/include \
            -I$(CROSSDIR)/arm-linux/include \
            -I$(CROSSDIR)/lib/gcc-lib/arm-linux/2.95.3/include \

$(MODDEV): $(MODFILE) $(MODFILE_H) Makefile
    $(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

install: $(MODDEV)
    mkdir -p $(INSTALLDIR)
    cp --target-dir=$(INSTALLDIR) $(MODDEV)

clean:
    -rm -f $(MODDEV)
```

打开led-edukit-s3c2410.c，把第17行“#include ”小写的s3c2410.h改为大写的S3C2410.h
然后在/root下键入make，会在当前目录下生成led.o，这就是编译成功的led驱动程序

## [3.加载测试]

通过超级终端，把led.o下载到开发板linux操作系统的/var目录下，然后使用insmod led.o命令进行加载，加载成功后，会提示如下信息

```
etc/var # insmod led.o
Using led.o
Embest EdukitII-2410 led driver version 1.0 (2005-06-18) <www.embedinfo.com>
Led major number = 253
```

表示加载led驱动程序成功，在/dev目录下会新增/dev/led/0文件，这就是led的文件节点

```
/dev # cd led
/dev/led # ls
0
```

编写测试程序testled.c，编译成功，下载到开发板执行，得到如下成功执行信息

```
/etc/var # chmod +x testled
/etc/var # ./testled
led device open SUCCESS!
```

表示led驱动工作正常。。。

## [4.驱动源程序led-edukit-s3c2410.c与测试程序testled.c]

```c
/*
* linux/derivers/led/led-edukit-s3c2410.c
* led driver for Embest EduKit II
* Copyright (C) 2005 Embest 
*/
#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/errno.h>    // error codes

#include <linux/types.h>    // size_t

#include <linux/delay.h>    // mdelay

#include <linux/proc_fs.h>
#include <asm/uaccess.h>    // to copy to/from userspace

#include <asm/hardware.h>
#include <asm/arch-s3c2410/S3C2410.h>
#include <asm/leds.h>

#ifdef    LED_DEBUG    // if want to debug, define it while compile this function

#define DEBUG(str, args...)    printk("led: " str, ## args)
#else
#define DEBUG(str, args...)
#endif

#undef u32
#define u32    unsigned

static int nLedMajor = 0;                        /* allow to alloc the major number for led*/
#define LED_DEVNAME "led"

#define GPF_MASK    (0xF<<4)                    /* GPF4-7: LED1-LED4 */
#define GET_DATA(f) ((u8)(~f>>4))
#define SET_DATA(t, f) ( f = (~t&0x0F)<<4 )

#define LED_LOCK(u)        down(&u->lock);
#define LED_UNLOCK(u)    up(&u->lock);

#define GPF_CTL_BASE         io_p2v(0x56000050)/* get the virtual address map to GPF */
#define S3C2410_GPFCON (GPF_CTL_BASE + 0x0)
#define S3C2410_GPFDAT (GPF_CTL_BASE + 0x4)
#define S3C2410_GPFUP (GPF_CTL_BASE + 0x8)

unsigned long flags;
static char *version = "Embest EdukitII-2410 led driver version 1.0 (2005-06-18) \n";

struct unit {
    struct semaphore lock;
    u32 *GPF_CON;    /* GPFCON register */
    u32 *GPF_DAT;    /* GPFDAT register */
    u32 *GPF_UP;    /* GPFUP register */
    u32 f;            /* LEDs value (bit4-7:LED1-LED4)*/
};
static struct unit led_unit = {
    .GPF_CON        = (u32 *)S3C2410_GPFCON,
    .GPF_DAT        = (u32 *)S3C2410_GPFDAT,
    .GPF_UP            = (u32 *)S3C2410_GPFUP,
    .f                = 0x00 // turn on all LEDs

};


#ifdef CONFIG_PROC_FS
static int led_proc(char *page, char **start, off_t off,
                         int count, int *eof, void *data)
{
    char *p = page;
    int len;
    struct unit led_u;
    
    len = led_u.f;
    
    p += sprintf(p, "%s\n LED1: %s LED2: %s LED3: %s LED4: %s \n", version,
                 len&(1<<4)?"ON":"OFF",
                 len&(1<<5)?"ON":"OFF",
                 len&(1<<6)?"ON":"OFF",
                 len&(1<<7)?"ON":"OFF"
         );

    len = (p - page) - off;
    if (len < 0)
        len = 0;

    *eof = (len <= count) ? 1 : 0;
    *start = page + off;

    return len;
}
#endif


static void led_set_value(struct unit *unit, u8 val)
{
    u32 temp;

    SET_DATA(val, unit->f);
    
    temp = *unit->GPF_DAT;
    temp &= ~GPF_MASK;
    temp |= unit->f;
    *unit->GPF_DAT = temp;
    DEBUG("write to GPFDAT:0x%x.\n", temp);
}
static u8 led_get_value(struct unit *unit)
{
    u8 temp = GET_DATA(unit->f);

    return temp;
}


static int led_open(struct inode *inode, struct file *file)
{
    DEBUG("open\n");

    leds_event(led_stop);
    file->private_data = &led_unit;
    MOD_INC_USE_COUNT;

    return 0;
}

static int led_release_f(struct inode *inode, struct file *file)
{
    DEBUG("release\n");
    MOD_DEC_USE_COUNT;

    leds_event(led_start);
    return 0;
}

static ssize_t led_read(struct file *file, char *buf, size_t count, loff_t *offset)
{
    u8 temp;
    int ret;
    struct unit *unit = (struct unit *)file->private_data;

    DEBUG("read\n");
    if(count > 1)
        count = 1;
    LED_LOCK(unit);
    temp = led_get_value(unit);
    DEBUG("Read from unit value field:0x%x.\n", temp);
    ret = copy_to_user(buf, &temp, count) ? -EFAULT : count;
    LED_UNLOCK(unit);

    return ret;
}

static ssize_t led_write(struct file *file, const char *buf, size_t count, loff_t *offset)
{
    u8 temp;
    int ret;
    char *tmp;
    struct unit *unit = (struct unit *)file->private_data;

    DEBUG("write\n");
    
    if(count > 1)
        count = 1;
    LED_LOCK(unit);
    ret = copy_from_user(&temp, buf, count) ? -EFAULT : count;
    DEBUG("led0 writing %d bytes:0x%x.\n", ret,temp);
    if(ret)
        led_set_value(unit, temp);
    
    LED_UNLOCK(unit);

    return ret;
}


static struct file_operations led_ops = {
    owner:        THIS_MODULE,
    read:        led_read,
    write:        led_write,
    open:        led_open,
    release:    led_release_f,
};


/*
* led device init
*/
static void __init led_init(struct unit *unit)
{
    u32 temp;

    /* init device lock */
    init_MUTEX(&unit->lock);

    /* init io port */
    temp = *unit->GPF_CON;
    temp &= ~(0xF<<4);
    temp |= ((1<<14) | (1<<12) | (1<<10) | (1<<8));// GPF4,5,6,7 as Output

    *unit->GPF_CON = temp;

    temp = *unit->GPF_UP;
    temp |= (0xF<<8);// pull up all GPF4,5,6,7

    *unit->GPF_UP = temp;
    
    /* init data and turn on led, bit0-3:LED1-4*/
    led_set_value(unit, 0x0f);

    /* delay some time */
    mdelay(100);

    /* turn off led */
    led_set_value(unit, 0x00);
}


/*
* module init
*/
static devfs_handle_t devfs_handle,devfs_led_dir;
#ifdef MODULE
int init_module(void)
#else
int __init led_init_module(void)
#endif
{
    int res;

    DEBUG("init_module\n");
    /* print version information */
    printk(KERN_INFO "%s", version);

    /* register led device, character device: /proc/devices*/
#ifdef CONFIG_DEVFS_FS
    res = devfs_register_chrdev(0, LED_DEVNAME, &led_ops);
    if(res < 0) {
        printk("led-edukit-s3c2410.o: unable to get major for led device.\n");
        return res;
    }

    /* create the devfs: /driver/led/0 */
    devfs_led_dir = devfs_mk_dir(NULL, LED_DEVNAME, NULL);
    devfs_handle = devfs_register(devfs_led_dir, "0", DEVFS_FL_DEFAULT,
                 res, 0,S_IFCHR | S_IRUSR | S_IWUSR,&led_ops, NULL); 
#else
    res = register_chrdev(nLedMajor, LED_DEVNAME, &led_ops);
#endif

    /* add a profile to /proc/driver/led */
#ifdef CONFIG_PROC_FS
    create_proc_read_entry ("driver/led", 0, 0, led_proc, NULL);
#endif

    /* get the major number */
    if( nLedMajor == 0 )
    {
        nLedMajor = res;
        printk("Led major number = %d\n",nLedMajor);
    }

    /* then call led_init() */
    led_init(&led_unit);

    return 0;
}


/*
* module cleanup
*/
#ifdef MODULE
void cleanup_module(void)
#else
void __exit led_cleanup(void)
#endif
{
    int res;

    DEBUG("cleanup\n");
    /* unregister led device */
    res = unregister_chrdev(nLedMajor, LED_DEVNAME);
    if(res < 0)
        printk("led-edukit-s3c2410.o: unable to release major %d for led device.\n", nLedMajor);

#ifdef CONFIG_DEVFS_FS    
    devfs_unregister(devfs_handle);
    devfs_unregister(devfs_led_dir);
#endif    

    /* remove the profile /proc/driver/led */
#ifdef CONFIG_PROC_FS
    remove_proc_entry("driver/led", NULL);
#endif    
}

#ifndef MODULE
/* declare a module init entry */
module_init(led_init_module);
/* declare a module exit entry */
module_exit(led_cleanup);
#endif

MODULE_DESCRIPTION("EduKitII-2410 led driver");
MODULE_AUTHOR("Embest tech&info Co.,Ltd. ");
MODULE_LICENSE("GPL");
```

```c
/******************************************************************************
*Name:            testled.c
*Desc:            s3c2410子板led驱动程序测试程序
*Parameter: 
*Return:
*Author:        yoyoba(stuyou@126.com)
*Date:            2010-9-30
*Modify:        2010-9-30
********************************************************************************/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/fcntl.h>

int main()
{
    int fd;
    if(-1==(fd=open("/dev/led/0",O_RDWR)))
        printf("led device open FAILD!\n");
    else
        printf("led device open SUCCESS!\n");
        
    return 0;
}
```