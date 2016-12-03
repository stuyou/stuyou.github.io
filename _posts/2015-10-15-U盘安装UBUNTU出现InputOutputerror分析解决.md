---
layout: post
title: U盘安装UBUNTU出现Input/Output error分析解决 
date: 2015-10-15 22:41:47 +0800
categories:
- Linux
---

昨天用UltraISO将U盘做成启动盘后,安装Ubuntu,发生[error 5]Input/Output error错误。搜索资料,发现说的千篇一律,都是复制粘贴的,这无所谓,但关键是根本解决不了我们的问题。

废话不长,我们直接来说出错的原因:u盘里的文件错误导致。

[error 5]Input/Output error这个错误说的没有错,导致文件出错的原因是,下载后的Ubuntu.ISO在写入u盘时发生了改变。这个是由于下载后的文件存入的驱动器格式化格式与u盘不同造成。

我是将下载后的Ubuntu.ISO放入了驱动器D盘,是NTFS格式,我的u盘是fat32格式。因为目前UltraISO并不支持将u盘格式化NTFS,而且一开始我也没当回事,结果怎么安装也不成功。

后来我用了大白菜和老毛桃,还是同样错误。

解决方法:

于是,我将u盘手动格式化为NTFS,用老毛桃直接将ISO写入到u盘,安装过程没有遇到任何问题。

总结:

u盘安装ubuntu过程出错[error 5]Input/Output error。

出错原因：

u盘格式与存放ubuntu.iso驱动器格式不同，导致u盘启动器制作工具在写入u盘时，造成写入u盘里的文件发生改变所致。

解决方法：

u盘格式与存放ubuntu.iso的驱动器格式一致，用老毛桃、大白菜或UltraISO写入即可。（推荐老毛桃，亲身体验，不是广告）

注：

UltraISO写入时可能会出现“设备忙”的错误，这是UltraISO自身版本兼容问题。为了安装顺利避免不必要的麻烦。我最后用的老毛桃，虽然比UltraISO要大，但现在网络下载也不需要太多时间。

这是网上的一篇文章。

今天用U盘装WIN7的时候，复制文件的过程中也发生了类似错误。因为在把WIN7.iso写入U盘之前，U盘被格式化为FAT32，而WIN7.ISO文件存放在NTFS分区中，因此会造成这样的错误，后来把U盘格式化成NTFS分区，然后再把WIN7.ISO写入U盘，安装成功。