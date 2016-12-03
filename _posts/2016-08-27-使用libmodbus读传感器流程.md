---
layout: post
title: 使用libmodbus读传感器流程
date: 2016-08-27 22:32:10 +0800
categories:
- Linux
tags:
- libmodbus
- embedded
---

## 【1.项目描述】

手上有一个温湿度传感器，基于modbus RTU协议，采用RS485串口和Tiny6410通信，把采集到的温湿度显示在Tiny6410的界面程序上。这里简要给出使用libmodbus第三方工具读取温湿度的程序流程。关于libmodbus在嵌入式LINUX上的使用方法，参考"基于QT4.7的嵌入式libmodbus开发环境"

## 【2.关于libmodbus】
libmodbus是一个免费开源的第三方modbus协议库，可工作在多种平台下。libmodbus支持RTU方式和TCP/IP方式。这里传感器使用的是modbus RTU方式。在modbus RTU方式中，对client/master和server/slave的是这样定义的：

```
The Modbus RTU framing calls a slave, a device/service which handle Modbus requests, and a master, a client which send requests. The communication is always initiated by the master.
```

从这个定义中可以看出，构造RTU数据帧，并且响应modbus请求的一方是server/slave，而发出请求的一方是client/master，并且双方通信总是由client/master一方发起。因此本项目中，温湿度传感器是server/slave一方，Tiny6410是client/master一方。

## 【3.温湿度传感器Modbus RTU协议】

查看温湿度传感器手册，采用RS485通信，采用串口参数为波特率9600，数据位8，停止位1，无校验位。若要读取温湿度数值，则Tiny6410要往温湿度传感器发送的请求帧为：01 03 00 00 00 02 c4 0b，这里都是十六进制表示。01表示温湿度传感器的从站地址；03是modbus功能码，表示读取保持寄存器的值；00 00是保持寄存器的地址；00 02是读取寄存器的长度；c4 0b是CRC校验位。正常情况下，温湿度传感器会返回给Tiny6410 TRU帧为01 03 04 01 0F 02 16 CRCHCRCL。01为从站地址；03为modbus功能码；04为读取的数据字节长度；010F为温度数据，换算成十进制后除以10，得到真实的温度值；0216为湿度数据，换算成十进制后除以10，得到真实的湿度值。

## 【4.libmods编写读传感器温湿度程序流程】

使用libmodbus读取传感器数值，一般要经过这样几个步骤（采用modbus RTU方式）：

1.创建一个modbus_t类型的context，用来打开串口

```c
modbus_t  *ctx;
ctx = modbus_new_rtu("/dev/ttySAC3",9600,'N',8,1);
```

这里打开的串口是Tiny6410的串口3，波特率9600，无校验，数据位是8，停止位是1。

2.建立连接

```c
modbus_connect(ctx)；//建立和传感器的连接
```

3.设置超时时间：

```c
struct timeval t;
t.tv_sec = 0;
t.tv_usec = 1000000;//1000ms
modbus_set_response_timeout(ctx,&t);
```

libmodbus中对 modbus_set_response_timeout的定义如下：

```
The modbus_set_response_timeout() function shall set the timeout interval used to wait for a response. If the waiting before receiving the response is longer than the given timeout, an error will be raised.
```
意思是如果在超时时间内还没有收到响应，那么就会发出一个错误码。

4.设置slave地址

```c
modbus_set_slave(ctx, 0x01);//温湿度传感器的modbus站地址为0x01；
```

5.读保持寄存器的值。

使用int modbus_read_registers(modbus_t *ctx, int addr, int nb, uint16_t *dest)读取保持寄存器的值。对该函数的描述是：

```
The modbus_read_registers() function shall read the content of the nb holding registers to the address addr of the remote device. The result of reading is stored in dest array as word values (16 bits).
You must take care to allocate enough memory to store the results in dest (at least nb * sizeof(uint16_t)).
The function uses the Modbus function code 0x03 (read holding registers).
```

该函数用来读保持寄存器的值，使用的功能码是0x03，其中寄存器的起始地址放入addr参数，读寄存器的个数放入nb参数，读出的值放入dest。若读取正确，返回值为读取的寄存器数，若读取错误，返回值为-1。针对本例，应使用的函数为：

```c
uint16_t tab_reg[2];
int regs=modbus_read_registers(ctx,0,2,tab_reg);
```

读取温湿度的请求帧为：01 03 00 00 00 02 c4 0b，即寄存器地址为0，要读的寄存器个数为2。若读取成功，温度数值置于tab_regp[0]，湿度数值置于tab_reg[1]，分别除以10，就得到正确的温湿度数值。

5.释放并关闭modbus

```c
modbus_close(ctx); 
modbus_free(ctx);
```

此外，当有多个从站时，需要仔细设置 response timeout。这是摘录libmodbus v3.1.2帮助文档里的一段话

```
The libmodbus implementation of RTU isn’t time based as stated in original Modbus specification, instead all bytes are sent as fast as possible and a response or an indication is considered complete when all expected characters have been received. This implementation offers very fast communication but you must take care to set a response timeout of slaves less than response timeout of master (ortherwise other slaves may ignore master requests when one of the slave is not responding).
```

这段话的意思是说，从站设置的response timeout应小于主站的response timeout，因此为了防止出现某些从站因为response timeout设置不恰当而不能响应的问题，建议把主站的response timeout设置的尽可能大一些。