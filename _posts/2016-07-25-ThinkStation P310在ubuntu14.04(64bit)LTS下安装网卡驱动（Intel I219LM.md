---
layout: post
title:  ThinkStation P310在ubuntu14.04(64bit)LTS下安装网卡驱动（Intel I219LM）
date: 2016-07-25 09:03:26 +0800
categories:
- Windows
tags:
- ubuntu1404
---

联想服务器ThinkStation P310，在安装完ubuntu14.04(64bit)LTS后，不能上网。这是由于网卡驱动没有安装成功，需要自己安装网卡驱动。

ThinkStation P310的主板类型是skylake，网卡型号是Intel Ethernet Connection I219LM。在Intel官网并没有找到I219LM for Linux的驱动。不过看到一个帖子说，I219LM网卡和I218的网卡没什么区别，因此决定使用I218的网卡驱动。

1. 从intel官网下载I218 for LINUX的驱动压缩包e1000e-3.3.4.tar.gz（https://downloadcenter.intel.com/download/15817）
2. ubuntu下切换到`root：sudo -i`
3. 解压网卡驱动`tar -xzvf e1000e-3.3.4.tar.gz`
4. 切换目录：`cd e1000e-3.3.4/src`
5. 编译安装：`make install`，将编译好的驱动（e1000e.ko）安装到/lib/modules/3.16.0-30-generic/updates/drivers/net/ethernet/intel/ethernet/intel/e1000e/e1000e.ko
6. 载入驱动模块到内核：`modprobe e1000e`，此时正常情况下，就可以检测到网卡，并能上网了。
7. 如果第6步没有检测到网卡，尝试使用如下命令插入驱动模块到内核：

```
insmod /lib/modules/3.16.0-30-generic/updates/drivers/net/ethernet/intel/ethernet/intel/e1000e/e1000e.ko
```
然后重启ubuntu，就可以上网了。