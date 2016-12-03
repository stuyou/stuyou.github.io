---
layout: post
title: OptiSystem与MATLAB协同仿真
date: 2013-01-17 21:15:42 +0800
categories:
- Matlab
tags:
- OptiSystem
- 协同仿真
---

OptiSystem里面提供了MATLAB库，可以使用MATLAB代码对OS的信号进行处理，在使用OS与MATLAB协同仿真的时候，首先要了解OS的信号输入到MATLAB工作空间的格式。

## OS信号载入到MATLAB工作空间的格式

OS的InputPort的信号在MATLAB工作空间的数据格式如图1所示。

<img src="https://github.com/stuyou/stuyou.github.io/raw/master/_posts/image/OS_MATLAB_CO_1.jpg" style="display:block;margin:auto"/>
<center>图1</center>

由图1可以看出，OS的信号格式包括“TypeSignal”，字符类型，表示该信号的类型为光信号、电信号或二进制信号；“Sampled”，结构体，OS的信号就包含在该字段当中。"Parameterized"，结构体，参数化字段，表示一些与时间平均有关的量，如平均功率、中心波长、偏振态等；"Noise"，结构体，表示噪声数据；"Channels"，表示该信号的波长，是指中心波长。

### Smapled字段的数据结构

Smapled字段的数据结构如图2或图3所示。

<img src="https://github.com/stuyou/stuyou.github.io/raw/master/_posts/image/OS_MATLAB_CO_2.jpg" style="display:block;margin:auto"/>
<center>图2</center>

<img src="https://github.com/stuyou/stuyou.github.io/raw/master/_posts/image/OS_MATLAB_CO_3.jpg" style="display:block;margin:auto"/>
<center>图3</center>

如果选择的是频率抽样信号，则Sampled的数据格式如图2所示。如果选择的是时间抽样信号，则Sampled的数据格式如图3所示。到底是时间信号还是频率信号，由具体问题决定。使用MATLAB在时域对信号处理时，就选择时域抽样，否则，选择频域抽样。时域抽样频域抽样的选择参考下面实例的第2步。由图2、图3看出，Smapled包含两个字段，一个是Signal字段，该字段信号在抽样点的值；另一个是Frequency或Time字段，该字段是抽样点的频点或时间点。

### Parameterized字段数据结构

Parameterized字段数据结构如图4所示

<img src="https://github.com/stuyou/stuyou.github.io/raw/master/_posts/image/OS_MATLAB_CO_4.jpg" style="display:block;margin:auto"/>
<center>图4</center>

包含了4个字段，分别为平均功率Power，四个信道的中心频率Frequency，分光比SplittingRatio，相位Phase。

### Noise字段数据结构

Noise字段数据结构如图5所示

<img src="https://github.com/stuyou/stuyou.github.io/raw/master/_posts/image/OS_MATLAB_CO_5.jpg" style="display:block;margin:auto"/>
<center>图5</center>

包含了噪声功率Power，这里是个2*7的数据结构，2表示是两个偏振态，一共7个信道。每个信道噪声的最低频率LowerFrequency和最高频率UpperFrequency，以及相位Phase。

## 实例

使用MATLAB代码，对CW输出光衰减20dB，光谱右移1THz

### 搭建OS工程，创建MATLAB库

如图6所示。


<img src="https://github.com/stuyou/stuyou.github.io/raw/master/_posts/image/OS_MATLAB_CO_6.jpg" style="display:block;margin:auto"/>
<center>图6</center>

### 设置MATLAB库

对MATLAB库的参数作以下设置：（假设OS所使用的MATLAB库的脚本文件为att.m，该文件存放在“E:\MatlabWorkspace”）设置Main选项卡里的“Run command”值为“att”（即脚本文件名）；“Sampled signal domain”可以为时域也可以为频域；Inputs选项卡的端口数为1，Outputs选项卡的端口数也为1，然后勾选Main选项卡里的“load matlab”，然后点“OK”。会弹出一个如图7所示的“matlab command windows”

<img src="https://github.com/stuyou/stuyou.github.io/raw/master/_posts/image/OS_MATLAB_CO_7.jpg" style="display:block;margin:auto"/>
<center>图7</center>

在该窗口中，首先使用cd命令，切换到att.m文件所在的目录“E:\MatlabWorkspace”。即输入: cd E:\MatlabWorkspace

在该窗口中输入命令：open att.m，打开att.m文件，编写相应的代码，然后保存att.m；或者打开预先编写好的att.m文件。在做OS与MATLAB协同仿真时，以上2、3、4步必须要做。

### 运行OS

在OS中点运行。图8是CW激光器输出光谱，功率为0dBm，中心频率为193.1THz，图9为经过MATLAB库处理后的输出光谱，可以看到，输出光谱的功率为-20dBm，中心频率为194.1THz。

<img src="https://github.com/stuyou/stuyou.github.io/raw/master/_posts/image/OS_MATLAB_CO_8.jpg" style="display:block;margin:auto"/>
<center>图8</center>

<img src="https://github.com/stuyou/stuyou.github.io/raw/master/_posts/image/OS_MATLAB_CO_9.jpg" style="display:block;margin:auto"/>
<center>图9</center>

### att.m文件

```matlab
OutputPort1=InputPort1;
A=0.1;%定义衰减因子
f=InputPort1.Sampled.Frequency;%输入光信号的频谱
OutputPort1.Sampled.Frequency=f+1e+12;%输出光谱频率右移1THz
[is,cs]=size(InputPort1.Sampled);
len=length(InputPort1.Sampled);
for counter=1:cs
    OutputPort1.Sampled(1,counter).Signal=InputPort1.Sampled(1,counter).Signal*A;
end
```

## OS与MATLAB协同仿真要点总结：

1. OS中MATLAB模块的“Run command”值应与脚本文件的值保持一致；
2. 打开MATLAB的command窗口后，应把目录切换到脚本文件所在的路径；
3. 打开脚本文件；
4. 脚本文件的编写要点有两条，第一，把输入端口的数据结构复制到输出端口，即OutputPort1=InputPort1;第二，对从输入端口获得的信号InputPort1.Sampled(1,counter).Signal进行处理，把处理结果赋值到输出端口OutputPort1.Sampled(1,counter).Signal。
