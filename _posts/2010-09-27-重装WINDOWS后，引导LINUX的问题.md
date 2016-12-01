---
layout: post
title: 重装WINDOWS后，引导LINUX的问题
date: 2010-09-27 23:30:09 +0800
categories:
- Linux
tags:
- grub
- ubuntu10.04
---

原来机器是双系统WINXP+UBUNTU10.04LTS，删掉XP，安装WIN7后，按照以下步骤重新引导UBUNTU10.04LTS
1. 用UBUNTU10.04LTS的安装盘引导启动，进入LIVE CD模式，即试用UBUNTU模式。
2. 打开终端，键入如下命令

```
sudo -i
```
切换到root用户

```
fdisk -l
```

查看LINUX安装在哪个分区，比如说安装在/dev/sda7分区，则执行如下命令

```
mount /dev/sda7 /mnt
grub-install --root-directory=/mnt /dev/sda
```

如果没有问题，会提示安装成功  
重新启动电脑，会看到GRUB引导菜单，这是引导菜单上没有新装的WIN7，先进入LINUX，然后执行如下命令

```
sudo update-grub2
```

更新grub2后，会看到WIN7的引导已经在引导菜单中了。  
重启电脑，引导菜单中又看到WIN7+UBUNTU10.04LTS双系统了。