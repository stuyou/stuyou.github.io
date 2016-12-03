---
layout: post
title: Optisystem与MATLAB快速协同仿真 
date: 2013-10-17 09:54:57 +0800
categories:
- Matlab
tags:
- OptiSystem
- 协同仿真
---

在Optisystem与MATLAB进行协同仿真时，（参考《 OptiSystem与MATLAB协同真》）由于MATLAB模块计算缓慢，可能导致整个仿真过程需要花大量的时间，尤其是仿真之后，参数需要调整，需要不停的仿真，这样所花费的时间是很大的。为了解决这个问题，可以在MATLAB里先把MATLAB的代码运行一遍，然后把运行空间保存一下，这样在Optisystem中所需要的数据已经都保存好了，然后在仿真的时候，只需要用Load命令把刚才保存的运行空间文件加载进来就可以了，这样节省了大量的时间。以《 OptiSystem与MATLAB协同真》为例，对其进行修改。比如我把衰减因子A单独写成一个MATLATB脚本文件，即A=0.1;保存命名为AA.m，在MATLAB中运行AA.m脚本，然后把workspace保存为AA.mat。在Optisystem与MATLAB联合仿真时，MATLAB的脚本文件编写如下：

```matlab
OutputPort1=InputPort1;
f=InputPort1.Sampled.Frequency;%输入光信号的频谱
OutputPort1.Sampled.Frequency=f+1e+12;%输出光谱频率右移1THz
[is,cs]=size(InputPort1.Sampled);
len=length(InputPort1.Sampled);
load AA.mat;（用Load命令把AA.mat中的衰减因子A载入进来）
for counter=1:cs
    OutputPort1.Sampled(1,counter).Signal=InputPort1.Sampled(1,counter).Signal*A;
end
```

运行之后，得到的结果和原来的结果是完全一致的
