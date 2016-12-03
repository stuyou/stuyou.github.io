---
layout: post
title: ThinkStation P310安装WIN7 64bit 
date: 2016-07-07 15:44:42 +0800
categories:
- Windows
---

ThinkStation P310是联想退出的塔式工作站。用U盘安装WIN7时，安装不成功。这是因为ThinkStation P310采用skybay主板，全部采用USB3.0接口，而WIN7 64bit安装盘中只有USB2.0驱动，因此当进入WIN7安装界面时，鼠标、键盘卡死（都是USB键鼠），且不能正常从U盘读入WIN7安装文件。解决方法是把USB3.0驱动写入U盘里的WIN7安装包。具体过程参考如下链接：http://iknow.lenovo.com/detail/dc_143417.html

注意：第一次用Windows7 USB3.0 Creator写入USB3.0程序，不成功。第二次改用Windows USB Installation Tool，成功把WIN7 64BIT安装到ThinkStation P310