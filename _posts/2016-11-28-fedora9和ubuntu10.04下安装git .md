---
layout: post
title:  Fedora9和Ubuntu10.04下安装git 
date: 2016-11-28 10:10:20 +0800
categories:
- Linux
tags:
- fedora9
- ubuntu1004
- git
---

## UBUNTU10.04安装git

1. 下载git源码：地址：http://down1.chinaunix.net/distfiles/git-1.7.8.tar.gz
2. 切换到root：sudo -i
3. 安装zlib库：apt-get install zlib1g-dev
4. 解压git：tar xzvf git-1.7.8.tar.gz
5. 切换git目录：cd git-1.7.8
6. 配置：./configure
7. 编译：make
8. 安装：make prefix=usr/ install
9. 查看git版本：git --version，如果有输出，安装成功

## Fedora9安装git

1. 下载git源码：地址：http://down1.chinaunix.net/distfiles/git-1.7.8.tar.gz
2. 安装必要的库：yum install  curl-devel expat-devel gettext-devel openssl-devel zlib-devel
3. 解压git：tar xzvf git-1.7.8.tar.gz
4. 切换git目录：cd git-1.7.8
5. 配置：./configure
6. 编译：make
7. 安装：make prefix=usr/ install
8. 查看git版本：git --version，如果有输出，安装成功