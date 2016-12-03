---
layout: post
title: virtualbox+WIN7+Fedora9文件夹共享的问题
date: 2016-07-01 18:41:55 +0800
categories:
- Linux
tags:
- virtualbox
- fedora9
---

主机安装WINDOWS7 64bit，采用virtualbox虚拟机安装Fedora9，设置文件夹共享的时候发生如下问题：

1.安装增强工具时，提示如下错误：

```
Building the main Guest 
Additions module                   [失败]
```

解决方法：执行指令`yum install kernel-devel`,然后再安装增强功能，重启即可。

2.设置共享文件夹时，需用到指令：`mount -t vboxsf share /mnt/share`。如果要想开机就挂载共享文件夹，那么需把这条指令写入文件：/etc/rc.local，在这个文件的最后添加`mount -t vboxsf share /mnt/share`。重启Fedora后发现共享文件夹还是不好使。只有在shell里指令`mount -t vboxsf share /mnt/share`，才能挂载文件夹。但是这种方式挂载的文件夹在Fedora9中只能读，不能写。为了解决这两个问题，修改/etc/selinux/config文件和/etc/sysconfig/selinux文件，将`SELINUX=enforcing`改为`SELINUX=disabled`或`SELINUX=permissive`，保存，重启Fedora9。此时发现共享文件夹启动系统后可以自动挂载了，而且也可以往共享文件夹写入文件了。

注意：由于fedora9的内核版本太低，即使安装增强功能，也不能使用自动调整窗口尺寸的功能。