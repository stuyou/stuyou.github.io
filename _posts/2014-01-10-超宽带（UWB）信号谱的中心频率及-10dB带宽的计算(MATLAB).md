---
layout: post
title: 超宽带（UWB）信号谱的中心频率及-10dB带宽的计算(MATLAB)
date: 2014-01-10 00:40:37 +0800
categories:
- Matlab
tags:
- UWB
---

对于超宽带（UWB）信号，相对带宽或带宽及中心频率是其中两个重要的参数。如果已经得到UWB信号谱，那么通过信号谱如何来计算带宽及中心频率呢？

一般来说，UWB信号谱可以通过直接测量或者由OptiSystem得到，是离散谱。通过拟合的方法，可以求得其中心频率及带宽。以下是自定义的求中心频率和带宽的函数find_UWB:

```matlab
%==========================================================================
%Name: function [cw,bw_low,bw_up,val_low,val_up,val_max]=find_UWB(filename)
%Desc: 已知由OptiSystem产生的UWB脉冲的频谱文件(.txt)，
% 求其中心频率和-10dB带宽
%Parameter: [in]finename,UWB频谱文件，该频谱文件已经去除了直流
%Return: [out]cw,返回中心频率的值
% [out]bw_low,返回-10dB带宽所对应的低端频率
% [out]bw_up,返回-10dB带宽所对应的高端频率
% [out]val_low,返回bw_low所对应的UWB频谱值(dBm)
% [out]val_up,返回bw_up所对应的UWB频谱值(dBm)
% [out]val_max,返回UWB频谱最大值(dBm)
%Author: yoyoba(stuyou@126.com)
%Date: 2014-1-7
%Modify: 2014-1-10
%=========================================================================
function [cw,bw_low,bw_up,val_low,val_up,val_max]=find_UWB(filename) 
uwb=load(filename); %已除去直流分量的UWB频谱
uwb_f=uwb(:,1); %读入频率值
uwb_val=uwb(:,2); %读入频谱值
imax=find(diff(sign(diff(uwb_val)))==-2)+1; %由OptiSystem产生的UWB频谱是离散谱
%有些频点的值为0，为了后续的数据拟合，需要把不为0的点单独提取出来，即寻找极大值
tuwb_val_max=uwb_val(imax); %频谱中的一系列极大值
tuwb_ff_max=uwb_f(imax); %极大值所对应的频率点
uwb_val_max=zeros(size(tuwb_val_max));
uwb_ff_max=zeros(size(tuwb_ff_max));
uwb_val_max(1)=tuwb_val_max(1);
uwb_ff_max(1)=tuwb_ff_max(1);
for i=1:length(tuwb_val_max)-1 %由于UWB频谱存在一些伪最大值点，该循环用来
    %去除这些伪最大值点，原理是两个相邻点差值小于10dB的认为是伪最大值点。
    if tuwb_val_max(i)-tuwb_val_max(i+1)>10
        uwb_val_max(i)=tuwb_val_max(i);
        uwb_ff_max(i)=tuwb_ff_max(i);
    end
    if tuwb_val_max(i)-tuwb_val_max(i+1)<-10
        uwb_val_max(i+1)=tuwb_val_max(i+1);
        uwb_ff_max(i+1)=tuwb_ff_max(i+1);
    end
end
uwb_val_max=uwb_val_max(uwb_val_max<0);
uwb_ff_max=uwb_ff_max(uwb_ff_max>0);
p5_uwb_val_max=polyfit(uwb_ff_max,uwb_val_max,5); %5阶多项式拟合
deltaf=uwb_ff_max(1):1e+6:uwb_ff_max(end); %频率间隔1MHz
p5val=polyval(p5_uwb_val_max,deltaf); %计算频率间隔为1MHz对应的频谱值
[p5max,ip5max]=max(p5val); %频谱最大值及所在位置
p5max_10dB=p5max-10; %最大值-10dB
p5val_10dB=p5val-p5max_10dB; %频谱值-10dB
ibw=find(diff(sign(diff(abs(p5val_10dB))))==2)+1; %寻找频谱值-10dB之后绝对值的最小值的位置
bw_low=deltaf(ibw(1)); %-10dB带宽低端频率
bw_up=deltaf(ibw(2)); %-10dB带宽高端频率
val_low=p5
val(ibw(1)); %bw_low所对应的UWB频谱值(dBm)
val_up=p5val(ibw(2)); %bw_up所对应的UWB频谱值(dBm)
val_max=p5max; %UWB频谱最大值(dBm)
cw=bw_low+(bw_up-bw_low)/2; %中心频率
```

对于使用OS得到的UWB信号谱：posi_1_1_f.txt，调用该函数，计算其中心频率及带宽，如下：

```matlab
filename='posi_1_1_f.txt';
[cw,bw_low,bw_up,val_low,val_up,val_max]=find_UWB(filename);
uwb=load(filename);
plot(uwb(:,1)*1e-9,uwb(:,2),'k','linewidth',2);
hold on
plot([bwlow,bwup]*1e-9,[val_low,val_up],'r','linewidth',2);
axis([0,15,-100,-75]);
xlabel('Frequency(GHz)');
ylabel('Power (dBm)');
set(get(gca,'XLabel'),'FontSize',14,'FontName','Times New Roman');
set(get(gca,'YLabel'),'FontSize',14,'FontName','Times New Roman');
set(gca,'FontName','Times New Roman','FontSize',14)
```

结果为：

<img src="https://github.com/stuyou/stuyou.github.io/raw/master/_posts/image/UWB.jpg" style="display:block;margin:auto"/>

[posi_1_1_f.txt](https://github.com/stuyou/stuyou.github.io/raw/master/_posts/data/posi_1_1_f.txt)