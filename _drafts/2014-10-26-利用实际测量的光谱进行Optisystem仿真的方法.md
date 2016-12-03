---
layout: post
title: 利用实际测量的光谱进行Optisystem仿真的方法
date: 2014-10-26 20:47:52 +0800
categories:
- Matlab
tags:
- OptiSystem
---

对于使用光谱仪测量得到的光谱，需要导入到OptiSystem中仿真使用。此时要使用“Measured Optical Filter”元件。举例如下：
1.假设所测量的长周期光纤光栅透射谱如图1所示，其数据文件为LPFG.TXT。


<img src="https://github.com/stuyou/stuyou.github.io/raw/master/_posts/image/MOF_1.jpg" style="display:block;margin:auto"/>
<center>图1 光谱仪所测LPFG透射谱</center>

2.为了把LPFG导入OS中仿真使用，在OS中打开“Filter Library”--->"Optical"，添加一个“Measured Opticl Filter”。

3.双击“Measured Opticl Filter”图标，打开其属性设置对话框，在“Main”选项卡中，“user define frequency”选中后，可以在“Frequency”设定中心频率。在“Measurements”选项卡中，设定所测光谱的相关参数。由于实测LPFG的光谱数据文件包含两列数据（如图2所示），第一列为波长，单位为nm，第二列为透射谱值，单位为dBm。

<img src="https://github.com/stuyou/stuyou.github.io/raw/master/_posts/image/MOF_2.jpg" style="display:block;margin:auto"/>
<center>图2 LPFG透射谱数据文件格式</center>

此时在“measurements”选项卡中，“file frequency unti”设置为nm，“file format”设置为power，“linear scale”不打勾，在“filter filename”中打开LPFG透射谱数据文件，这样就把LPFG透射谱导入到OS中了。

4.使用“Optical Filter Analyzer”对导入的LPFG透射谱进行频谱分析，结构如图3所示。LPFG透射谱如图4所示。


<img src="https://github.com/stuyou/stuyou.github.io/raw/master/_posts/image/MOF_3.jpg" style="display:block;margin:auto"/>
<center>图3 测试结构图</center>

<img src="https://github.com/stuyou/stuyou.github.io/raw/master/_posts/image/MOF_4.jpg" style="display:block;margin:auto"/>
<center>图4 optical filter analyzer测得的LPFG透射谱</center>