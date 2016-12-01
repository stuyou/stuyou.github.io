---
layout: post
title:  在VIM中实现对嵌入式软件的调试以及VIM IDE的配置方法
date: 2010-09-27 23:30:09 +0800
categories:
- 嵌入式Linux
tags:
- vim
- embedded
---

请参考以下链接
http://blog.chinaunix.net/u1/55904/showart_435472.html
http://hi.chinaunix.net/batch.viewlink.php?itemid=18983
http://blog.chinaunix.net/u2/78601/article_94584.html
http://www.dzsc.com/data/html/2009-6-23/77129.html
no terminal library found
checking for tgetent()... configure: error: NOT FOUND!
You need to install a terminal library; for example ncurses.
Or specify the name of the library with --with-tlib.
其实是没有安装ncurses开发包，安装就是了    
在新立得软件管理器中找到libncurses5-dev，安装   
用的朋友喜欢用make xconfig，直接运行会出现错误