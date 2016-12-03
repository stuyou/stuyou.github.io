---
layout: post
title: Optisystem与MATLAB协同仿真，Optisystem工作在参数扫描状态
date: 2015-04-20 20:39:05 +0800
categories:
- Matlab
tags:
- OptiSystem
---

## 问题的提出：

最近在做高能量效率UWB信号的构造，使用双芯光纤产生一阶高斯monocycle脉冲，CW激光器的频率与双芯光纤传输谱的相对位置，决定了产生高斯monocycle脉冲的质量，因此要对CW激光器的频率进行扫描，确定最佳CW频率。确定最佳monocycle脉冲采用如下方法：对于检测器输出的高斯monocycle脉冲，第一步，计算最低点的绝对值和最高点的绝对值的差值，第二部，计算该差值的绝对值。越接近理论monocycle的脉冲，这个差值越小。理论上，这个差值应该为0。采用Optisystem与MATLAB协同仿真，Optisyste设置CW频谱为sweep模式，MATLAB程序计算每次扫描得到的monocycle脉冲的质量。在MATLAB程序中，定义del()数组，把每次扫描得到的差值绝对值放入该数组中，最后分析del数据的值，寻找其最小值，即可得到最佳monocycle脉冲对应的CW频率值。

## Optisysystem搭建

搭建的Optisystem如图1所示

<img src="https://github.com/stuyou/stuyou.github.io/raw/master/_posts/image/OS_MATLAB_SWEEP_1.jpg" style="display:block;margin:auto"/>（居中）
<center>图1</center>

图1中，CW激光器的频率设置为sweep模式，扫描次数假设为11次。Measured Optical Filter装载的为双芯光纤的传输谱。电高斯脉冲串调相到光载波上，经过双芯光纤处理，基于PM-IM原理，经PIN后，得到高斯monocycle脉冲。DC block后接MATLAB Component，用来分析产生的高斯monocycle脉冲的质量。

## MATLAB Component设置

首先在MATLAB Component中增加一个用户自定义参数“matiteration”，用来记录CW激光器频率的扫描次数。双击MATLAB Component，在其属性窗口点击“Add Parameters”，在弹出的窗口中，添加用户自定义参数，自定义参数的设置如图2所示。

<img src="https://github.com/stuyou/stuyou.github.io/raw/master/_posts/image/OS_MATLAB_SWEEP_2.jpg" style="display:block;margin:auto"/>（居中）
<center>图2</center>

注意在Category中输入User，此时在MATLAB Component属性窗口中会增加一个User类别选项卡。如图3所示。

<img src="https://github.com/stuyou/stuyou.github.io/raw/master/_posts/image/OS_MATLAB_SWEEP_3.jpg" style="display:block;margin:auto"/>（居中）
<center>图3</center>

MATLAB Component其余属性主要设置Main选项卡相关输入，如图4所示。

<img src="https://github.com/stuyou/stuyou.github.io/raw/master/_posts/image/OS_MATLAB_SWEEP_4.jpg" style="display:block;margin:auto"/>（居中）
<center>图4</center>

图4中，Load Matlab需选中，点击确定后会启动Matlab程序。Run Command中为MATLAB脚本文件的文件名。本问题是要对高斯monocycle脉冲的时域图进行确认，因此Sampled Signal Domain应设置为Time。

然后再把Inputs和Outputs的Signal types改成Electrical

## MATLAB脚本文件

MATLAB脚本文件名为check_mono_OS_sweep.m，与Run Command中的设置一致。内容为：

```matlab
%==========================================================================
%Name: check_mono_OS_sweep 
%Desc: OS工作在扫描模式时，确定产生的高斯Monocycle脉冲的质量。
%Parameter: 
%Return: 
%Author: yoyoba(stuyou@126.com)
%Date: 2015-4-20
%Modify: 2015-4-20
%=========================================================================
if ~exist('myloop','var'); %测试myloop变量是否已经定义，如果没定义，
                            %就初始化为1，如果以定义，就不再初始化。
    myloop=1;
end
%以上三行代码非常重要，myloop用来指示OS中的当前sweep次数。OS每扫描一次，就重新
%调用该脚本文件，因此只能在第一次调用的时候定义初始化myloop，后续调用时，myloop
%不能再次初始化为1，因为myloop指示的是OS中当前sweep的次数。
if ~exist('del','var'); %测试del变量是否已经定义
    del=zeros(1,matiteration); %这里的matiteration是OS中MATLAB component
                                %中设定的值，表示OS中sweep总次数
end
OutputPort1=InputPort1;
t=InputPort1.Sampled.Time;%输入光信号时间值
[is,cs]=size(InputPort1.Sampled);
                         
for counter=1:cs
    OutputPort1.Sampled(1,counter).Signal=InputPort1.Sampled(1,counter).Signal;
end
signal=InputPort1.Sampled(1,counter).Signal;
uwb1=real(signal);
ma=max(uwb1)/max(abs(uwb1)); %归一化最大值点的绝对值
mi=min(uwb1)/max(abs(uwb1)); %归一化最小值点的绝对值
del(myloop)=abs(abs(ma)-abs(mi)); %每次扫描分析的结果，存放在del数组中。
myloop=myloop+1; %每扫描一次，myloop加1.
```
## 运行测试

（1）按照以上步骤，搭建好OS，假设CW激光器频率扫描次数为11次。编写好check_mono_OS_sweep.m脚本，假设该脚本存放位置为E:\MatlabWorkshop

（2）设置MATLAB Component的User的matiteration数值为11（与CW频率扫描次数一致），选中Load Matlab，点击OK，此时会启动MATLAB

（3）在启动的MATLAB命令窗口中，切换路径到check_mono_OS_sweep.m所在路径，cd E:\MatlabWorkshop

（4）清空所有变量，clear

（5）打开check_mono_OS_sweep.m，open check_mono_OS_sweep.m

（6）在OS中，开始扫描，直到全部扫描结束；

（7）扫描结束后，回到MATLAB命令窗口，查看分析del数值。

**注意：每次重新扫描时，均需要在MATLAB命令窗口清空所有变量，clear**