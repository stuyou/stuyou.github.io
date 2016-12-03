---
layout: post
title: QCustomPlot实现实时动态曲线 
date: 2016-08-23 11:35:28 +0800
categories:
- Linux
tags:
- qcustomplot
---

Qt4中，可以使用QCustompPlot来绘制曲线，QCustompPlot是一个第三方工具，可以到官网下载：http://www.qcustomplot.com/index.php/download
这里实现一个实时动态曲线图，用随机数作为实时数据，程序运行结果如下：

<img src="https://github.com/stuyou/stuyou.github.io/raw/master/_posts/image/qcustomplot.png" style="display:block;margin:auto"/>

主机环境：fedora9，Qt4.7，Qtcreator 2.0.1

使用Qtcreator 2.0.1新建一个工程，基类模板选择QMainWindow。将解压得到的QCustompPlot文件夹里面的头文件qcustomplot.h和源文件qcustomplot.cpp复制粘贴到工程文件夹下。在Qtcreator中，对着工程名右键，添加已有文件，将头文件qcustomplot.h和源文件qcustomplot.cpp都添加到工程中来。

在界面上拖拽一个widget部件，然后升级成Qcustomplot，(参考：http://www.bubuko.com/infodetail-744744.html)部件名称改为customPlot

mainwindow.h代码如下：

```cpp
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "qcustomplot.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    //设置qcustomplot画图属性，实时
    void setupRealtimeDataDemo(QCustomPlot *customPlot);
private slots:
    //添加实时数据槽
    void realtimeDataSlot();

private:
    Ui::MainWindow *ui;
    //定时器，周期调用realtimeDataSlot()槽，实现动态数据添加到曲线
    QTimer dataTimer;


};

#endif // MAINWINDOW_H
```

mainwindow.cpp代码如下：

```cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVector>
#include <QTimer>
#include <QTime>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setupRealtimeDataDemo(ui->customPlot);
    ui->customPlot->replot();

    ui->checkBox_temp->setChecked(true);
    ui->checkBox_hui->setChecked(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//画图初始化
void MainWindow::setupRealtimeDataDemo(QCustomPlot *customPlot)
{
//#if QT_VERSION < QT_VERSION_CHECK(4, 7, 0)
  //QMessageBox::critical(this, "", "You're using Qt < 4.7, the realtime data demo needs functions that are available with Qt 4.7 to work properly");
//#endif
  //demoName = "Real Time Data Demo";

  // include this section to fully disable antialiasing for higher performance:
  /*
  customPlot->setNotAntialiasedElements(QCP::aeAll);
  QFont font;
  font.setStyleStrategy(QFont::NoAntialias);
  customPlot->xAxis->setTickLabelFont(font);
  customPlot->yAxis->setTickLabelFont(font);
  customPlot->legend->setFont(font);
  */
  customPlot->addGraph(); // blue line
  customPlot->graph(0)->setPen(QPen(Qt::blue));
  customPlot->graph(0)->setName("temp");
  //customPlot->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
  //customPlot->graph(0)->setAntialiasedFill(false);
  customPlot->addGraph(); // red line
  customPlot->graph(1)->setPen(QPen(Qt::red));
  customPlot->graph(1)->setName("hui");
  //customPlot->graph(0)->setChannelFillGraph(customPlot->graph(1));


  customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
  customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
  customPlot->xAxis->setAutoTickStep(false);
  customPlot->xAxis->setTickStep(2);
  customPlot->axisRect()->setupFullAxesBox();

  // make left and bottom axes transfer their ranges to right and top axes:
  //connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
  //connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));

  // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
  connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
  dataTimer.start(0); // Interval 0 means to refresh as fast as possible
  customPlot->legend->setVisible(true);



}

void MainWindow::realtimeDataSlot()
{
    //key的单位是秒
    double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
    qsrand(QTime::currentTime().msec() + QTime::currentTime().second() * 10000);
    //使用随机数产生两条曲线
    double value0 = qrand() % 100;
    double value1 = qrand() % 80;
    if (ui->checkBox_temp->isChecked())
        ui->customPlot->graph(0)->addData(key, value0);//添加数据1到曲线1
    if (ui->checkBox_hui->isChecked())
        ui->customPlot->graph(1)->addData(key, value1);//添加数据2到曲线2
    //删除8秒之前的数据。这里的8要和下面设置横坐标宽度的8配合起来
    //才能起到想要的效果，可以调整这两个值，观察显示的效果。
    ui->customPlot->graph(0)->removeDataBefore(key-8);
    ui->customPlot->graph(1)->removeDataBefore(key-8);

      //自动设定graph(1)曲线y轴的范围，如果不设定，有可能看不到图像
//也可以用ui->customPlot->yAxis->setRange(up,low)手动设定y轴范围
    ui->customPlot->graph(0)->rescaleValueAxis();
    ui->customPlot->graph(1)->rescaleValueAxis(true);   

    //这里的8，是指横坐标时间宽度为8秒，如果想要横坐标显示更多的时间
    //就把8调整为比较大到值，比如要显示60秒，那就改成60。
    //这时removeDataBefore(key-8)中的8也要改成60，否则曲线显示不完整。
    ui->customPlot->xAxis->setRange(key+0.25, 8, Qt::AlignRight);//设定x轴的范围
    ui->customPlot->replot();
}
```