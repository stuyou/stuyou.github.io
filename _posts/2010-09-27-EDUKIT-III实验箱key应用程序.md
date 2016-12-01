---
layout: post
title:  EDUKIT-III实验箱key应用程序
date: 2010-09-27 23:30:09 +0800
categories:
- 嵌入式Linux
tags:
- embedded
---

## 开发环境

开发板：EDUKIT-III实验箱，S3C2410+LINUX2.4，实验箱随箱光盘提供的Zimage，nor flash启动。  
主机：ubnutn10.4LTS，arm-linux-gcc 2.95.3

## 源代码

```c
/******************************************************************************
*Name: mykey1.c
*Desc: 按下一个按键，然后从串口终端显示该键值。运行该程序前，应先使用insmod i2c.o命令，加载i2c.o驱动程序
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
#include "test-key.h"


/* control code */
#define I2C_SET_DATA_ADDR 0x0601
#define I2C_SET_BUS_CLOCK 0x0602

char key_set(char ucChar) //直接从寄存器读出的key值并不显示相应按键的值，使用该函数

        //把key转换为用户能够看的懂的值

{
 switch(ucChar)
 {
  case 1:
  case 2:
  case 3:
  case 4:
  case 5:
    ucChar-=1; break;
  case 9:
  case 10:
  case 11:
  case 12:
  case 13:
    ucChar-=4; break;
  case 17:
  case 18:
  case 19:
  case 20:
  case 21:
    ucChar-=7; break;
  case 25: ucChar = 0xF; break;
  case 26: ucChar = '+'; break;
  case 27: ucChar = '-'; break;
  case 28: ucChar = '*'; break;
  case 29: ucChar = 0xFF; break;
  default: ucChar = 0xFE; // error

 }
 return ucChar;
}

int main()
{
 int fd,i;
 char key;
 
 if((fd=open("/dev/i2c/0",O_RDWR))==-1) //打开i2c设备

 {
  printf("open the key device FAILD!\n");
  exit(1);
 }
 else
 printf("open the key device SUCCESS!\n");
 
 ioctl(fd, I2C_SLAVE_FORCE, ZLG_SLAVE_ADDR); //设置zlg7290从设备地址

 ioctl(fd, I2C_SET_BUS_CLOCK, 250*1000); //设置i2c总线时钟为250khz

  
 while(1)
 {
  ioctl(fd,I2C_SET_DATA_ADDR,REG_Sys); //设置SystemReg寄存器的地址

  read(fd,&key,1); //读取SystemReg寄存器的内容，放入key中

  
  if(key&0x1) //如果SyetemReg寄存的内容为1,则表示有效的按键动作

  {
   ioctl(fd,I2C_SET_DATA_ADDR,REG_Key); //设置kye寄存器的地址

   read(fd,&key,1); //从key寄存器中读入内容到key

   printf("the press key is %d\n",key_set(key)); //把从key寄存器中读取的内容，经过key_set()函数转换后，在串口终端打印出来。

  }
 }
 return 0;
}
```

```c
/*****************************
*test-key.h
*****************************/

#ifndef __ZLG9290_H__
#define __ZLG9290_H__


#define ZLG_SLAVE_ADDR (0x70 >> 1)


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