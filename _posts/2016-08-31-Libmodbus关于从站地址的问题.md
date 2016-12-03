---
layout: post
title: Libmodbus关于从站地址的问题 
date: 2016-08-31 13:30:12 +0800
categories:
- Linux
tags:
- embedded
---

在嵌入式LINUX开发板上，基于libmodbus第三方库编程实现土壤水分及温度的读取，传感器采用大连祺峰科技有限公司的土壤水分温度传感器（型号：SMTS-II-485）。在程序运行过程中，遇到了一些问题，记录如下:

查看土壤水分温度传感器手册，若要读取水分温度，则需要发送如下RTU帧：FE 03 00 00 00 02 D0 04，这里FE是传感器modbus站地址，这是传感器出厂默认地址。在libmodbus程序中，程序运行到modbus_set_slave(ctx,0xFE)时，程序报错并终止运行，为了查清原因，查看libmodbus源码中modbus_set_slave()函数的实现。

在Modbus.c文件中，可以看到modbus_set_slave()函数的实现如下：

```x
/* Define the slave number */
int modbus_set_slave(modbus_t *ctx, int slave)
{
    return ctx->backend->set_slave(ctx, slave);
}
```

这个函数执行的是ctx->backend->set_slave(ctx, slave)，ctx是一个modbus_t类型的结构体，定义在Modbus.h中

```c
typedef struct _modbus modbus_t;
```

在Modbus-Private.h中有_modbus的具体定义

```c
struct _modbus {
    /* Slave address */
    int slave;
    /* Socket or file descriptor */
    int s;
    int debug;
    int error_recovery;
    struct timeval response_timeout;
    struct timeval byte_timeout;
    const modbus_backend_t *backend;
    void *backend_data;
};
```

可以看到成员backend是一个modbus_backend_t，继续追踪modbus_backend_t，定义在Modbus-Private.h中

```c
typedef struct _modbus_backend {
    unsigned int backend_type;
    unsigned int header_length;
    unsigned int checksum_length;
    unsigned int max_adu_length;
    int (*set_slave) (modbus_t *ctx, int slave);
    int (*build_request_basis) (modbus_t *ctx, int function, int addr,
                                int nb, uint8_t *req);
    int (*build_response_basis) (sft_t *sft, uint8_t *rsp);
    int (*prepare_response_tid) (const uint8_t *req, int *req_length);
    int (*send_msg_pre) (uint8_t *req, int req_length);
    ssize_t (*send) (modbus_t *ctx, const uint8_t *req, int req_length);
    ssize_t (*recv) (modbus_t *ctx, uint8_t *rsp, int rsp_length);
    int (*check_integrity) (modbus_t *ctx, uint8_t *msg,
                            const int msg_length);
    int (*pre_check_confirmation) (modbus_t *ctx, const uint8_t *req,
                                   const uint8_t *rsp, int rsp_length);
    int (*connect) (modbus_t *ctx);
    void (*close) (modbus_t *ctx);
    int (*flush) (modbus_t *ctx);
    int (*select) (modbus_t *ctx, fd_set *rfds, struct timeval *tv, int msg_length);
    int (*filter_request) (modbus_t *ctx, int slave);
} modbus_backend_t;
```

这个结构体中除了前面几个变量成员外，剩下的都是一些函数指针，这些函数指针用来对ctx进行操作。其中看到第一个函数指针int (*set_slave) (modbus_t *ctx, int slave);就是用来设置从站地址的。那么这个函数的具体实现在哪里？

在Modubs-rtu.c文件中，有函数modbus_new_rtu()的具体实现，这个函数用来创建一个modbus RTU通信的context（可以理解为一个标识符），这个函数的源码如下：

```c
modbus_t* modbus_new_rtu(const char *device,
                         int baud, char parity, int data_bit,
                         int stop_bit)
{
    modbus_t *ctx;
    modbus_rtu_t *ctx_rtu;
    size_t dest_size;
    size_t ret_size;

    ctx = (modbus_t *) malloc(sizeof(modbus_t));
    _modbus_init_common(ctx);

    ctx->backend = &_modbus_rtu_backend;
    ctx->backend_data = (modbus_rtu_t *) malloc(sizeof(modbus_rtu_t));
    ctx_rtu = (modbus_rtu_t *)ctx->backend_data;

    dest_size = sizeof(ctx_rtu->device);
    ret_size = strlcpy(ctx_rtu->device, device, dest_size);
    if (ret_size == 0) {
        fprintf(stderr, "The device string is empty\n");
        modbus_free(ctx);
        errno = EINVAL;
        return NULL;
    }

    if (ret_size >= dest_size) {
        fprintf(stderr, "The device string has been truncated\n");
        modbus_free(ctx);
        errno = EINVAL;
        return NULL;
    }

    ctx_rtu->baud = baud;
    if (parity == 'N' || parity == 'E' || parity == 'O') {
        ctx_rtu->parity = parity;
    } else {
        modbus_free(ctx);
        errno = EINVAL;
        return NULL;
    }
    ctx_rtu->data_bit = data_bit;
    ctx_rtu->stop_bit = stop_bit;

    return ctx;
}
```

这个函数中有很重要的一条语句：ctx->backend = &_modbus_rtu_backend;条语这条语句实际上就使用_modbus_rtu_backend对ctx中的backend成员进行初始化，因此当调用modbus_new_rtu()函数时，除了完成对串口的初始化之外，也完成了对ctx结构体中的成员进行初始化。那么_modbus_rtu_backend这个变量是在哪里定义的？追踪发下，在Modubs-rtu.c文件中有对_modbus_rtu_backend的定义：

```c
const modbus_backend_t _modbus_rtu_backend = {
    _MODBUS_BACKEND_TYPE_RTU,
    _MODBUS_RTU_HEADER_LENGTH,
    _MODBUS_RTU_CHECKSUM_LENGTH,
    MODBUS_RTU_MAX_ADU_LENGTH,
    _modbus_set_slave,
    _modbus_rtu_build_request_basis,
    _modbus_rtu_build_response_basis,
    _modbus_rtu_prepare_response_tid,
    _modbus_rtu_send_msg_pre,
    _modbus_rtu_send,
    _modbus_rtu_recv,
    _modbus_rtu_check_integrity,
    NULL,
    _modbus_rtu_connect,
    _modbus_rtu_close,
    _modbus_rtu_flush,
    _modbus_rtu_select,
    _modbus_rtu_filter_request
}
```

这段代码对 modbus_backend_t类型的变量“_modbus_rtu_backend”进行了初始化。其中可以看到，modbus_set_slave()函数真正调用的是_modbus_set_slave这个函数。在Modbus-rtu.c文件中可以找到_modbus_set_slave()的函数定义：

```c
/* Define the slave ID of the remote device to talk in master mode or set the
 * internal slave ID in slave mode */
static int _modbus_set_slave(modbus_t *ctx, int slave)
{
    /* Broadcast address is 0 (MODBUS_BROADCAST_ADDRESS) */
    if (slave >= 0 && slave <= 247) {
        ctx->slave = slave;
    } else {
        errno = EINVAL;
        return -1;
    }

    return 0;
}
```

阅读这个函数源码可以知道，modbus_set_slave(modbus_t *ctx, int slave)这个函数的作用是把ctx结构体中的slave成员设置为参数slave的值。该函数中规定，libmodbus中站地址的有效范围是0到247，其中0是广播地址。而所用传感器的默认出厂站地址是0xFE(254)，超出了libmodbus中站地址的有效范围，因此导致对ctx的slave成员设置不成功。

实际上当使用modbus_new_rtu()函数创建ctx时，对ctx的各成员已经初始化了默认值。在modbus_new_rtu()函数中调用了函数_modbus_init_common(ctx);该函数用来对ctx成员进行初始化默认值。这个函数的实现在Modbus.c文件中：

```c
void _modbus_init_common(modbus_t *ctx)
{
    /* Slave and socket are initialized to -1 */
    ctx->slave = -1;
    ctx->s = -1;

    ctx->debug = FALSE;
    ctx->error_recovery = MODBUS_ERROR_RECOVERY_NONE;

    ctx->response_timeout.tv_sec = 0;
    ctx->response_timeout.tv_usec = _RESPONSE_TIMEOUT;

    ctx->byte_timeout.tv_sec = 0;
    ctx->byte_timeout.tv_usec = _BYTE_TIMEOUT;
}
```

这里可以看到ctx->slave = -1;也就是说ctx中slave默认值是-1。

那么执行modbus_set_slave(ctx,0xFE)函数产生的错误信息是哪里来的呢？在Modbus-rtu.c文件中有一个函数：

```c
/* Builds a RTU request header */
static int _modbus_rtu_build_request_basis(modbus_t *ctx, int function,
                                           int addr, int nb,
                                           uint8_t *req)
{
    assert(ctx->slave != -1);
    req[0] = ctx->slave;
    req[1] = function;
    req[2] = addr >> 8;
    req[3] = addr & 0x00ff;
    req[4] = nb >> 8;
    req[5] = nb & 0x00ff;

    return _MODBUS_RTU_PRESET_REQ_LENGTH;
}
```

这个函数用来构造modbus-RTU帧头（RTU帧的前6个字节，包括地址、功能码、寄存器地址、个数等信息），这个函数中调用了assert(ctx->slave != -1);LINUX下assert函数的用法是assert( int expression );其作用是先计算表达式 expression ，如果其值为假（即为0），那么它先向stderr打印一条出错信息，然后通过调用 abort 来终止程序运行

当执行modbus_set_slave(ctx,0xFE)函数时，由于0xFE不在libmodbus有效的站地址范围内（0~247），因此ctx结构体中的slave成员还是保持原来用_modbus_init_common(ctx)初始化的值，即-1，在构造modbus-RTU帧头的时候，运行到assert(ctx->slave != -1)就会报错，并终止程序运行。

解决方法就是把传感器的站地址修改为02（在0~247之内），再对传感器进行读取，成功。

附记：对大连祺峰科技有限公司的土壤水分温度传感器（型号：SMTS-II-485）修改站地址流程如下：
该传感器有5根线，其中有一个为屏蔽线（或SET线），若要对传感器执行写操作，那么必须把SET线接高电平。当SET线置高时，传感器站地址自动变为0xFF，查看传感器手册，站地址寄存器地址为0x200，因此需往传感器发送如下RTU指令：FF 06 02 00 00 02 1C 6D，传感器返回FF 06 02 00 00 02 1C 6D，说明地址修改成功，这里是把传感器地址修改为02。把SET重新接回到低电平，用02地址访问传感器，成功。

另外，在SMTS-II-485手册中提到，当SET接高电平时，传感器内部通讯参数自动变为为9600,N,8,2（波特率9600，无校验，8个数据位，2个停止位），实验发现并非如此。SET接高电平时，通信参数还是和读取时一样，即9600,N,8,1（波特率9600，无校验，8个数据位，1个停止位）。
