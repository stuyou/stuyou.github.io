---
layout: post
title: Linus又骂人stupid（转）
date: 2011-06-16 17:02:13 +0800
categories:
- Linux
---

最近, 有位用户向 bugzilla.redhat.com 报告他用 Fedora Linux 上网听 MP3 音乐时, 会播放出奇怪的声音. Linux 之父 Linus Torvalds 参与了讨论, 并最终找出原因, 竟然是 glibc 升级了 memcpy() 函数, 导致浏览器的 Abobe Flash Player 插件出现问题.

这真是太强大了, 竟然能从上网听音乐追查到几乎是软件最底层基础的 memcpy() 函数! 如果你想知道他是如何一步一步找出 BUG 的原因的, 可以自己去看贴. (我个人不得不表示非常佩服他们敏锐的技术嗅觉和科学精神!)

这个 BUG 的原因是, 某位 glibc 贡献者(看邮件地址应该是 Intel 公司的某华裔工程师)提交了一个速度更快的 memcpy() 函数的实现并被采纳. 但是, 这个速度更快的 memcpy() 并没有像它的前一个版本一样对源内存和目的内存重叠的情况做兼容, 所以导致了 Flash 插件出问题.

Glibc 认为, memcpy() 函数的手册清楚的说明, memcpy() 所操作的两块内存不能重叠:

```
MEMCPY(3)                  Linux Programmer's Manual                 MEMCPY(3)

NAME
       memcpy - copy memory area

SYNOPSIS
       #include 

       void *memcpy(void *dest, const void *src, size_t n);

DESCRIPTION
       The  memcpy()  function  copies  n bytes from memory area src to memory
       area dest.  The memory areas should not overlap.  Use memmove(3) if the
       memory areas do overlap.
```

新版本的 memcpy() 完全遵守标准, 没有任何问题, 完全是 Adobe 的程序员没有编写正确的代码导致了 BUG, 应该算在 Adobe 的头上, 所以把这个报告标记为”NOTABUG(不是 BUG)”.

Linus 老大不屑地说, 在 Linux 内核里我们就用了我们自己的非常漂亮的 memcpy(), 而且经过简单测试, 还比所谓的 glibc 的新版本 memcpy() 还要快呢, glibc 的那个速度更快的新版本 memcpy() 根本就是愚蠢至极(“There’s no real reason to do the copy backwards that I can see, so doing it that way is just stupid.”). 毕竟是在 glibc 才导致了 BUG 的出现, 简单地推卸责任会让用户非常失望.

事情似乎告了一段落, 但是这些个国外的大牛人们的争论, 让我们看到了做技术的人所应该具有的科学态度, 他们据理力争, 反驳有理有据的争论(讨论)方式值得我们学习. 特别是他们敏锐的观察和思考”领域外”的技术细节的精神 – 谁能想到浏览器放音乐出现破音竟然是 glibc 的升级后导致的? – 往往是我们缺少的.