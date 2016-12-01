---
layout: post
title: EDUKIT-III实验箱LED应用程序
date: 2010-09-27 23:30:09 +0800
categories:
- 嵌入式Linux
tags:
- embedded
---

## 开发板环境

开发板：EDUKIT-III实验箱，S3C2410+LINUX2.4，实验箱随箱光盘提供的Zimage，nor flash启动。  
主机：ubnutn10.4LTS，arm-linux-gcc 2.95.3
 
## 源程序代码

```c
/******************************************************************************
*Name: myled1.c
*Desc: 依次点亮lec0-led7，然后再依次熄灭led7-led0，运行该程序前，应使用insmod i2c.o命令，加载i2c.o驱动模块
*Parameter: 
*Return:
*Author: yoyoba(stuyou@126.com)
*Date: 2010-9-17
*Modify: 2010-9-17
********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <linux/i2c.h>
#include <linux/fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "test-8led.h"

/* control code */
#define I2C_SET_DATA_ADDR 0x0601
#define I2C_SET_BUS_CLOCK 0x0602
unsigned char digit[] ={0xFC,0x60,0xDA,0xF2,0x66,0xB6,0xBE,0xE0,0xFE,0xF6,0xEE,0x3E,0x9C,0x7A,0x9E,0x8E}; //0-F


int main()
{
 int fd,i;
 char chon=0xff,choff=0x0;
 
 printf("this is a example for 8-led,writed by youhaidong!\n");
 
 if((fd=open("/dev/i2c/0",O_RDWR))==-1) /*打开i2c设备*/
  {
   printf("open led device FAILD!\n");
   exit(1);
  }
 else
 printf("open led device SUCCESS!\n");
 
 ioctl(fd, I2C_SLAVE_FORCE, ZLG_SLAVE_ADDR); /*设置zlg7290从设备地址*/
 ioctl(fd, I2C_SET_BUS_CLOCK, 250*1000); /*设置i2c总线时钟频率为250khz*/
 
 for(i=0;i<8;i++)
 {
  ioctl(fd, I2C_SET_DATA_ADDR, REG_Dis0+i);
  write(fd,&chon,1); /*依次把led0-led8全部点亮*/
  sleep(2);
 }
 
 for(i=0;i<8;i++)
 {
  ioctl(fd, I2C_SET_DATA_ADDR, REG_Dis7-i);
  write(fd,&choff,1); /*依次把led7-led0熄灭*/
  sleep(2);
 }
 
 printf("led example is over!\n");

 return 0;
}
```

```c
/******************************************************************************
*Name: myled2.c
*Desc: led0依次点亮0-F，运行该程序前，应使用insmod i2c.o命令，加载i2c.o驱动模块
*Parameter: 
*Return:
*Author: yoyoba(stuyou@126.com)
*Date: 2010-9-17
*Modify: 2010-9-17
********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <linux/i2c.h>
#include <linux/fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "test-8led.h"

/* control code */
#define I2C_SET_DATA_ADDR 0x0601
#define I2C_SET_BUS_CLOCK 0x0602
unsigned char digit[] ={0xFC,0x60,0xDA,0xF2,0x66,0xB6,0xBE,0xE0,0xFE,0xF6,0xEE,0x3E,0x9C,0x7A,0x9E,0x8E}; //0-F


int main()
{
 int fd,i;
 char chon=0xff,choff=0x0;
 
 printf("this is a example for 8-led,writed by youhaidong!\n");
 
 if((fd=open("/dev/i2c/0",O_RDWR))==-1) /*打开i2c设备*/
  {
   printf("open led device FAILD!\n");
   exit(1);
  }
 else
 printf("open led device SUCCESS!\n");
 
 ioctl(fd, I2C_SLAVE_FORCE, ZLG_SLAVE_ADDR); /*设置zlg7290从设备地址*/
 ioctl(fd, I2C_SET_BUS_CLOCK, 250*1000); /*设置i2c总线时钟频率为250khz*/
 
 
 ioctl(fd, I2C_SET_DATA_ADDR, REG_Dis0);

 for(i=0;i<16;i++)
 { 
  write(fd, &digit[i], 1); //Led0依次显示0-F

  sleep(2);
  write(fd,&choff,1); //led0熄灭，使得下次能够正常显示

 }

 printf("led example is over!\n");

 return 0;
}
```

```c
/***********************
*test-8led.h
***********************/
#ifndef __ZLG9290_H__
#define __ZLG9290_H__


#define ZLG_SLAVE_ADDR (0x70>>1)


#define REG_Sys 0x00
#define REG_Key 0x01
#define REG_Cnt 0x02
#define REG_Func 0x03
#define REG_Cmd0 0x07
#define REG_Cmd1 0x08
#define REG_Flas 0x0C
#define REG_Num 0x0D
#define REG_Dis0 0x10
#define REG_Dis1 0x11
#define REG_Dis2 0x12
#define REG_Dis3 0x13
#define REG_Dis4 0x14
#define REG_Dis5 0x15
#define REG_Dis6 0x16
#define REG_Dis7 0x17

#define SEG_A (1<<7)
#define SEG_B (1<<6)
#define SEG_C (1<<5)
#define SEG_D (1<<4)
#define SEG_E (1<<3)
#define SEG_F (1<<2)
#define SEG_G (1<<1)
#define SEG_DIP (1<<0)


#endif /*__ZLG9290_H__*/
```