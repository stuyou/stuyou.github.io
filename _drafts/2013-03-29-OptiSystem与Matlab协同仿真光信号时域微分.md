---
layout: post
title: OptiSystem与Matlab协同仿真光信号时域微分 
date: 2013-03-29 15:07:14 +0800
categories:
- Matlab
tags:
- OptiSystem
- 协同仿真
- 微分
---

OptiSystem中没有提供光时域微分模块，若要对光信号进行时域微分，可以由OS中的MATLAB库实现光时域微分操作。这里，我们由双芯光纤作为光时域微分器，在OS的MATLAB库中实现双芯光纤传输函数，既可以对光信号提供时域微分。在OS里面搭建如图1所示的系统结构图。系统结构图搭建过程参考“ OptiSystem与MATLAB协同仿真”，MATLAB库所对应的MATLAB脚本文件为diff.m

## OS工程

常见一个OS工程，如图1所示

<img src="https://github.com/stuyou/stuyou.github.io/raw/master/_posts/image/OS_MATLAB_DIFF_1.jpg" style="display:block;margin:auto"/>
<center>图1.系统结构图</center>

待微分的信号设定为高斯信号，因此图1中使用高斯脉冲发生器来产生待微分的高斯光脉冲。用户自定义比特序列的码率为10Gb/s，“Bit sequence”设置为0001000000000000。高斯脉冲发生器中设定“Frequency”为190.6819THz，该频率为Matlab模块中的双芯光纤微分器的中心波长对应，“Width”为1bit。高斯光脉冲发生器产生的高斯光脉冲时域图及其单个脉冲的放大图如图2和图3所示。

<img src="https://github.com/stuyou/stuyou.github.io/raw/master/_posts/image/OS_MATLAB_DIFF_2.jpg" style="display:block;margin:auto"/>
<center>图2.高斯光脉冲序列时域图</center>

<img src="https://github.com/stuyou/stuyou.github.io/raw/master/_posts/image/OS_MATLAB_DIFF_3.jpg" style="display:block;margin:auto"/>
<center>图3.高斯光脉冲序列频谱图</center>

高斯光脉冲发生器产生的高斯光脉冲频谱图如图4所示。

<img src="https://github.com/stuyou/stuyou.github.io/raw/master/_posts/image/OS_MATLAB_DIFF_4.jpg" style="display:block;margin:auto"/>
<center>图4.高斯单个光脉冲放大图</center>

## MATLAB库脚本文件

MATLAB库脚本文件diff.m为：

```matlab
OutputPort1=InputPort1;
f=InputPort1.Sampled.Frequency;%输入光信号的频谱
f0=190.6819e+12;
[is,cs]=size(InputPort1.Sampled);
len=length(InputPort1.Sampled);
%定义双芯光纤参数
a=4.1e-6; %纤芯半径4.1微米
nco=1.44902; %纤芯折射率
dn=0.005; %纤芯和包层折射率差
ncl=nco-dn; %包层折射率
d=10.e-6; %双芯光纤两个纤芯之间的距离
jd=8.854e-12; %真空介电常数
dc=4*pi*1.e-7; %真空磁导率
c=3e+8; %光速
lamda=c./f;
z0=sqrt(dc/jd); %范林勇博士论文P40,公式（2-89）
N=length(d);
Nlamda=length(lamda);
Ktemp=zeros(size(lamda));
K12=zeros(size(lamda));
Mtemp=zeros(size(lamda));
M1=zeros(size(lamda));
for j=1:Nlamda
    omg=2*pi*c/lamda(j);    %光场角频率 
    [V,U,W]=f_LP01_V_U_W(nco,ncl,a,lamda(j));
    E0=sqrt((pi*a^2*nco/(2*z0))*(besselj(0,U)^2+besselj(1,U)^2+besselj(0,U)^2*...
        (besselk(1,W)^2-besselk(0,W)^2)/besselk(0,W)^2))^-1; %P40,公式（2-89）
    dtemp=d;
    alpha=asin(a/dtemp);
    tol=1.e-10;

    Ktemp(j)=dblquad(@(x,y)(conj((besselj(0,U)*besselk(0,W*y/a)/besselk(0,W))).*...
        besselj(0,U*(sqrt(dtemp^2+y.^2-2*dtemp.*y.*cos(x)))/a).*y).*(y.^2-2*dtemp*y.*...
        cos(x)+dtemp^2-a^2<=0),-alpha,alpha,dtemp-a,dtemp+a,tol); %计算K12的积分部分

    K12(j)=Ktemp(j)*omg*jd*(nco^2-ncl^2)*abs(E0)^2/4; %计算K12 

    Mtemp(j)=dblquad(@(x,y)abs((besselj(0,U)*besselk(0,W*y/a)/besselk(0,W))).^2.*y.*(y.^2-2*dtemp*y.*...
        cos(x)+dtemp^2-a^2<=0),-alpha,alpha,dtemp-a,dtemp+a,tol); %计算M1的积分部分

    M1(j)=Mtemp(j)*omg*jd*(nco^2-ncl^2)*abs(E0)^2/4; %计算M1 
end
z=70.e-3; %双芯光纤长度70mm
T1=cos(K12*z).*exp(-1i*M1*z); %第一个纤芯的透射谱

for counter=1:cs
    OutputPort1.Sampled(1,counter).Signal=InputPort1.Sampled(1,counter).Signal.*T1; %经过双芯光纤后的光谱
end
```

## 仿真结果

MATLAB库输出的高斯一阶微分脉冲时域图及单个脉冲放大图如图5、图6所示

<img src="https://github.com/stuyou/stuyou.github.io/raw/master/_posts/image/OS_MATLAB_DIFF_5.jpg" style="display:block;margin:auto"/>
<center>图5.高斯一阶微分光脉冲序列时域图</center>

<img src="https://github.com/stuyou/stuyou.github.io/raw/master/_posts/image/OS_MATLAB_DIFF_6.jpg" style="display:block;margin:auto"/>
<center>图6.高斯单个一阶微分光脉冲放大图</center>

高斯一阶微分光脉冲频谱图如图7所示

<img src="https://github.com/stuyou/stuyou.github.io/raw/master/_posts/image/OS_MATLAB_DIFF_7.jpg" style="display:block;margin:auto"/>
<center>图7.高斯一阶微分光脉冲频谱图</center>

通过时域和频域图可以看出，Matlab库输出的光信号确实为输入信号的一阶时域微分。

diff_tcf.rar为OptiSystem搭建的系统文件，diff.rar为MATLAB库的脚本文件。

[diff_tcf.rar](https://github.com/stuyou/stuyou.github.io/raw/master/_posts/data/diff_tcf.rar)

[diff.rar](https://github.com/stuyou/stuyou.github.io/raw/master/_posts/data/diff.rar)

