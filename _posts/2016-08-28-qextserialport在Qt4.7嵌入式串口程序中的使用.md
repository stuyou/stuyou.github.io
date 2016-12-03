---
layout: post
title: qextserialport在Qt4.7嵌入式串口程序中的使用
date: 2016-08-28 09:23:53 +0800
categories:
- Linux
tags:
- qextserialport
---

要编写一个工作于Tiny6410开发板上的嵌入式串口程序，决定使用qextserialport，qextserialport是一个第三方类，用于Qt下的串口编程。

## 【1.开发环境】

开发板环境：友善之臂Tiny6410+LINUX  
主机环境：操作系统Fedora9，交叉开发工具链采用友善之臂的arm-linux-gcc，QtCreator2.0.1友善之臂推荐的版本。

## 【2.关于qextserialport】

www.qter.org论坛上有关于qextserialport使用的详细教程。需要指出的是，早期的qextserialport版本在LINUX下只能工作于polling驱动模式。升级版的qextserialport在LINUX可工作于事件驱动模式。qcom-1.1.rar压缩包中使用的qextserialport就是升级版的，可支持事件驱动模式。qcom-1.1.rar压缩包也可以到www.qter.org的下载频道下载。

## 【3.建立工程】

在主机中，使用QtCreator新建一个工程，把qcom-1.1.rar压缩包中的qextserialport.h、qextserialport_global.h、qextserialport_p.h及qextserialport.cpp、qextserialport_unix.cpp复制到工程目录，然后再添加到工程即可。Qt Creator会自动修改工程文件(*.pro)，然后就可以使用qextserialport编写程序了, 不要忘记把头文件添加到工程中。

[qcom-1.rar](https://github.com/stuyou/stuyou.github.io/raw/master/_posts/data/qcom-1.rar)