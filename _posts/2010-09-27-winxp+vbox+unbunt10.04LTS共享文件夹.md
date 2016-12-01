---
layout: post
title: winxp+vbox+unbunt10.04LTS共享文件夹
date: 2010-09-27 23:30:09 +0800
categories:
- Linux
tags:
- virtualbox
- ubuntu10.04
---

1. 在XP下新建一个文件夹，用来作为和虚拟UBUNTU共享的文件夹，比如在E盘下建立xlshare文件夹。
2. 打开VBOX，依次打开设置-数据空间，点击添加“数据空间”，数据空间位置设置为E:\xlshare
3. 启动虚拟UBUNTU，在/mnt下建立一个目录，比如名字为xlshare，作为虚拟UBUNTU和XP的共享目录

```
sudo mkdir /mnt/xlshare
```

4. 终端下执行如下命令

```
sudo mount -t vboxsf xlshare /mnt/xlshare
```

把XP下的xlshare目录挂载到/mnt/xlsahre下，这样就可以实现XP和虚拟UBUNTU的数据共享了。XP中E:\xlshare文件夹下的内容和虚拟UBUNTU中/mnt/xlshare文件夹下的内容是一致的。
5. 每次重启虚拟UBUNTU后，都要重新执行以下挂载命令

```
sudo mount -t vboxsf xlshare /mnt/xlshare
```

可以把该条命令添加到/etc/rc.local文件中，这样每次重新启动UBUNTU后，就会自动挂载。注意，在/etc/rc.local的最后有一条exti 0语句，挂载命令应该位于exit 0的前面。