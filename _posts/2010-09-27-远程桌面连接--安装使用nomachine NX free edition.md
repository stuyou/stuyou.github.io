---
layout: post
title: 远程桌面连接--安装使用nomachine NX free edition
date: 2010-09-27 23:30:09 +0800
categories:
- Linux
tags:
- NX
- UBUNTU10.04
---

NX可以让用户远程登录到LINUX桌面，下面介绍在UBUNTU10.04LTS上安装NX的过程

## 准备工作 

1. 确保必要的依赖包存在。  
在新立得管理器(Synaptic)里搜索以下软件包，确保它们已经被安装  
libstdc++2.10-glibc2.2  
openssh  
2. 下载nomachine的NX free edition server client.  
Download "NX Desktop Server DEB for Linux" from:  
http://www.nomachine.com/select-package ... linux&id=1  
Download "NX Node DEB for Linux" from:  
http://www.nomachine.com/download-node.php?os=linux  
Download "NX Client DEB for Linux" from:  
http://www.nomachine.com/download-client-linux.php  
3. 如果你以前安装过FreeNX或者其它版本的NX，请先通过新立得卸载（用命令行也可以）  
并移除相关的文件夹和残留文件。 
 
## 安装  

按以下的顺序安装下载的DEB文件（顺序很重要，nxserver依赖于前两个包的安装）  
nxclient  
nxnode  
nxserver  
直接双击下载到的deb 文件安装即可。喜欢用命令行方式的可以cd到deb文件所在文件夹  
或用命令：  
```
sudo dpkg -i file/path/filename.deb   
```
## 配置  

1. 使用你喜欢的编辑器编辑/etc/ssh/sshd_config 文件


```
sudo vi /etc/ssh/sshd_config
```
添加一行：  
```
AuthorizedKeysFile /usr/NX/home/nx/.ssh/authorized_keys2
```

(注：如果原来有了AuthorizedKeysFile开头的这一行，很可能是因为之前安装过其他版本的NX，可以注释掉)  

2.  重启sshd:

```
sudo /etc/init.d/ssh restart
```
确定nxserver已经能正常运行：
```
sudo /usr/NX/bin/nxserver --status
```
如果得到信息如下，就是可以了
```
NX> 900 Connecting to server ..
NX> 110 NX Server is running.

NX> 999 Bye.
```
如果有错的话，应该是配置上的问题。到此nxserver安装完成。

## 更改nxserver配置文件

```
sudo vi /usr/NX/etc/server.cfg
```
去掉以下两行的注释，并改为
```
Enableautokillsessions = "1"
Enableunencryptedsession = "0"
```
在Windows或LINUX上安装nxclient，比如在windows上安装nxclient，按照设置进行设置，最后登录的用户名密码可选择安装nxserver的LINUX主机的用户名和密码进行登录。就可以看到远程LINXU的桌面了。 