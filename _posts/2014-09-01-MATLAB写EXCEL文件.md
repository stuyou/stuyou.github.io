---
layout: post
title: MATLAB写EXCEL文件
date: 2014-09-01 07:56:20 +0800
categories:
- Matlab
---

以下是一段简单的代码

```matlab
%==========================================================================
%Name: xls_w.m，
%Desc: 对EXCEL文件的写操作
%Parameter:
%Return:
%Author: yoyoba(stuyou@126.com)
%Date: 2014-9-1
%Modify: 2014-9-1
%==========================================================================
clear
getfilename=inputdlg('请输入文件名(包括路径)，例如：D:\mydata\data.xls');
                   %输入要写入的XLS文件名称和路径
a1={'第一列'}; %列标题
b1={'第二列'}; %列标题
c1={'第三列'}; %列标题
data1=1:10;
data2=11:20;
data3=21:30;
filename=getfilename{1,1}; %获取字符形式的文件名和路径
s1=xlswrite(filename,a1,'sheet1','A1'); %把第一列标题写入A1单元格
s2=xlswrite(filename,b1,'sheet1','B1');
s3=xlswrite(filename,c1,'sheet1','C1');
data1=data1'; %按列写入，需要对原始data转置
data2=data2';
data3=data3';
s4=xlswrite(filename,data1,'sheet1','A2');
s5=xlswrite(filename,data2,'sheet1','B2');
s6=xlswrite(filename,data3,'sheet1','C2');
if s1==1 && s2==1 && s3==1&&s4==1&& s5==1&&s6==1
    str1='数据已经成功保存到：';
    str=[str1,filename];
    msgbox(str);
else
    msgbox('保存数据错误，请检查！');
end
```

运行结果

![](https://github.com/stuyou/stuyou.github.io/raw/master/_posts/image/xls_w.jpg)


[xls_w.rar](https://github.com/stuyou/stuyou.github.io/raw/master/_posts/data/xls_w.rar)