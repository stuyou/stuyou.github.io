---
layout: post
title: UBUNTU+VirtualBox+WinXP集成摄像头和USB的使用
date: 2011-06-19 15:12:46 +0800
categories:
- Linux
tags:
- ubuntu10.04
- virtualbox
---

Thinkpad E40 0578m68笔记本电脑硬盘安装UBUNTU10.04，虚拟机VirtualBox下安装XP，在虚拟XP上使用集成摄像头和USB的方法如下：

1.http://www.virtualbox.org/wiki/Downloads 下载VirtualBox 4.0.8 Oracle VM VirtualBox Extension Pack，安装

2.virtualbox--->setting--->usb--->Enable USB 2.0(EHCI) Controller，选中

3.ubuntu下修改/etc/fstab文件，在该文件的最后一行添加

none /proc/bus/usb usbfs devmode=666 0 0

修改该文件前最好先备份

4.重启UBUNTU，启动虚拟XP，就可以在最下面的状态栏中右键单击USB图标，选择要使用的USB设备即可，包括集成摄像头也可以选中使用。