---
layout: post
title: XXXXXXX
date: 2013-12-24 23:30:09 +0800
categories:
- 个人经历
- 科研成果
- 教学工作
- 毕业指导
- 中国象棋
- Matlab
- Windows
- ARM
- 嵌入式Linux
- Linux
tags:
- matlab
- Linux
- embedded
- windows
- blog
- 主题使用
---

## 正文开始

发表模板

插入图片：（所有的图片都置于_post/image/下;）
<img src="https://github.com/stuyou/stuyou.github.io/raw/master/_posts/image/myself.jpg" style="display:block;margin:auto"/>（居中）
![](https://github.com/stuyou/stuyou.github.io/raw/master/_posts/image/myself.jpg)
<center>如图X所示</center>


插入链接：[查看详情](http://www.sciencedirect.com/science/article/pii/S0030402616307823)


表格：
| Tables        | Are           | Cool  |
| ------------- |:-------------:| -----:|
| col 3 is      | right-aligned | $1600 |
| col 2 is      | centered      |   $12 |
| zebra stripes | are neat      |    $1 |

WORD/EXCEL转HTML表格网址:http://pressbin.com/tools/excel_to_html_table/index.html
转成的HTML表格可直接插入MARKDOWN
插入代码：
```c
unsigned long ntp_get_time()
{
  pCon = (struct espconn *)os_zalloc(sizeof(struct espconn));
  pCon->type = ESPCONN_UDP;
  pCon->state = ESPCONN_NONE;
  pCon->proto.udp = (esp_udp *)os_zalloc(sizeof(esp_udp));
  pCon->proto.udp->local_port = espconn_port();
  pCon->proto.udp->remote_port = 123;
  ntp_send_request();
}
```