---
layout: post
title: 不联网安装build-essential
date: 2011-06-01 17:45:22 +0800
categories:
- Linux
tags:
- build-essential
---

build-essential包含了LINUX下常用的一些编译工具，因此开发LINUX程序前，应先安装这个包，联网情况下只需要在终端输入sudo apt-get install build-essential就可以安装了。如果没有联网，可以用如下方法安装：

1. 下载LINUX安装光盘的ISO镜像文件。我这里下载的是xubuntu-8.04.1-desktop-i386.iso，放在了/home目录下。可到如下地址下载XUBUNTU镜像文件(http://cdimage.ubuntu.com/xubuntu/releases/)
2. 把该ISO镜像文件挂载到光驱。光驱文件一般是/cdrom，切换到ISO文件所在目录，通过如下命令挂载：

```
sudo mount -o loop -t iso9660 xubuntu-8.04.1-desktop-i386.iso  /cdrom
```

3. 依次打开System--->Administration--->Softwar Source，在Software Source对话框中选择“Other software”选项卡，然后点击"Add CD-ROM"按钮，这样就把软件源设置为了光驱
4. 在终端中输入以下命令：sudo apt-get update
sudo apt-get install build-essential，这样就可以把build-essential安装好了。