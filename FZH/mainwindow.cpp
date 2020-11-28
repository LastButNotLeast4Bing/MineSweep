#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("MineSweeper");
    mineMap.Create();  //非传入参数的布雷
    offsetx=5; //测好的
    offsety=30;  //测好的
    setFixedSize(mineMap.mx*20+offsetx*2,mineMap.my*20+offsety+46);
    rumtime=new QTimer(this);
    connect( rumtime,SIGNAL(timeout()), this, SLOT(on_sectime()) );
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionRestart_triggered()
{
    mineMap.Restart();
    update();
}

void MainWindow::on_actionExit_triggered()
{
    exit(0);
}

void MainWindow::paintEvent(QPaintEvent *)  //绘图函数
{ //painter.drawPixmap这个函数用于从位图中截取一部分按原大小贴到指定位置

    //实例 QPixmap
    QPixmap bmpmap(":new/prefix1/resource/item2.bmp");
    QPixmap bmpnub(":new/prefix1/resource/item1.bmp");
    QPixmap bmpfame(":new/prefix1/resource/item3.bmp");
    QPixmap bmpface(":new/prefix1/resource/item4.bmp");

    //实例 Qainter 对象
    QPainter painter(this);

    //绘制类区
    //void drawPixmap(int x, int y, const QPixmap & pixmap, int sx, int sy, int sw, int sh)
    for(int i=0;i<mineMap.mx;i++)
    {
        for(int j=0;j<mineMap.my;j++)
        {
            if (mineMap.Map[i][j] >= 0 && mineMap.Map[i][j] < 9)  //逐步显示它周围数字的图(0-8)[而且0为特殊图]
            {
                painter.drawPixmap( i * 20+offsetx, j * 20 + 40+offsety,  bmpmap, mineMap.Map[i][j] * 20, 0,20,20);
            }
            if (mineMap.Map[i][j]>=99)  //初始显示图
                painter.drawPixmap(i * 20+offsetx, j * 20 + 40+offsety,bmpmap, 10 * 20, 0,20,20);
            if (mineMap.Map[i][j] == -1)  //炸弹分布图
                painter.drawPixmap(i * 20+offsetx, j * 20 + 40+offsety,bmpmap, 9 * 20, 0,20,20);
            if (mineMap.Map[i][j] == -2)  //插错旗子图
                painter.drawPixmap(i * 20+offsetx, j * 20 + 40+offsety,  bmpmap, 12 * 20, 0,20,20);
            if (mineMap.Map[i][j] > 40 && mineMap.Map[i][j] < 60)  //显示旗子图
                painter.drawPixmap(i * 20+offsetx, j * 20 + 40+offsety,  bmpmap, 11 * 20, 0,20,20);
        }
    }
    int rm=mineMap.rMine;
    int rt=mineMap.timer;
    //绘制框架
    //放在雷区上面，根据雷区大小伸长缩短，左边显示剩余雷数，右边显示扫雷时间，中间放个表情

    painter.drawPixmap( 0+offsetx, 0+offsety, bmpfame, 0, 0, 70, 40); //中间方图
    painter.drawPixmap(mineMap.mx * 10 - 20+offsetx, 0+offsety, bmpfame, 80, 0, 40, 40);  //左边雷区贴图
    painter.drawPixmap(mineMap.mx * 20 - 70+offsetx, 0+offsety, bmpfame, 130, 0, 70, 40);  //右边计时器贴图

    painter.drawPixmap(70+offsetx, 0+offsety, mineMap.mx * 10 - 90, 40, bmpfame, 70, 0, 10, 40); //左边雷区与中心区空间距
    painter.drawPixmap(mineMap.mx * 10 + 20+offsetx, 0+offsety, mineMap.mx * 10 - 90, 40, bmpfame, 70, 0, 10, 40); //右边雷区与中心区空间距

    //根据mineMap.winf * 24确定所选表情(双击选中没松开还没弄)
    painter.drawPixmap(mineMap.mx * 10 - 12+offsetx, 7+offsety, bmpface, mineMap.winf * 24+24, 0, 24, 24);

    //绘制剩余类数，靠左边贴图的
    if (rm < 0) rm = 0;
    painter.drawPixmap(6+offsetx, 5+offsety, bmpnub, rm / 100 * 20, 0, 20, 28); //百位
    if (rm >= 100) rm %= 100;
    painter.drawPixmap(26+offsetx, 5+offsety, bmpnub, rm / 10 * 20, 0, 20, 28); //十位
    painter.drawPixmap(46+offsetx, 5+offsety, bmpnub, rm % 10 * 20, 0, 20, 28); //个位

    //绘制扫雷时间，靠右边贴图的
    if (rt >= 1000) rt %= 1000;
    painter.drawPixmap(mineMap.mx*20 - 66+offsetx, 5+offsety, bmpnub, rt / 100 * 20, 0, 20, 28); //百位
    if (rt >= 100) rt %= 100;
    painter.drawPixmap(mineMap.mx*20 - 46+offsetx, 5+offsety, bmpnub, rt / 10 * 20, 0, 20, 28);//十位
    painter.drawPixmap(mineMap.mx*20 - 26+offsetx, 5+offsety, bmpnub, rt % 10 * 20, 0, 20, 28);//个位
}

//鼠标按下事件
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    int px=event->x() -offsetx;  //通过event->x()和event->y()获取鼠标按下的坐标．
    int py=event->y() -offsety;  //根据这个坐标计算出点到哪一格，然后调用扫雷类的相关操作
    int m = (px) / 20;
    int n = (py) / 20 - 2;
    if(event->buttons()==(Qt::LeftButton|Qt::RightButton))
    {
        if(mineMap.Onlrbtdown(m, n))
            update();
    }
    else if(event->button()==Qt::LeftButton)  //左击拆雷或者中雷
    {
        if(mineMap.Onlbtup(m,n))
        {
            if (!(rumtime->isActive() ))
                rumtime->start(1000);//开始计时
            update();
        }
        if (mineMap.winf==-1 || mineMap.winf == 1)
        {
            if (rumtime->isActive() )
                rumtime->stop();//停止计时
            if (px >  mineMap.mx * 10 - 15 && px< mineMap.mx * 10 + 15 && py>4 && py < 34)
            {
                mineMap.Restart();//点击表情图标重新开始
                update();
            }
        }
    }
    else if(event->button()==Qt::RightButton)  //右键小红旗
    {
        if(mineMap.Onrbtdown(m, n))
            update();
    }
    //未设计
   /* else if(event->button()==Qt::MidButton)
    {

    }*/
}
//初级
void MainWindow::on_actionPrimary_triggered()
{
    mineMap.Create(10,8,15);
    setFixedSize(mineMap.mx*20+offsetx*2,mineMap.my*20+offsety+66);
}
//中级
void MainWindow::on_actionMedium_triggered()
{
    mineMap.Create(20,15,50);
    setFixedSize(mineMap.mx*20+offsetx*2,mineMap.my*20+offsety+66);
}
//高级
void MainWindow::on_actionSenior_triggered()
{
    mineMap.Create(30,20,100);
    setFixedSize(mineMap.mx*20+offsetx*2,mineMap.my*20+offsety+66);
}
//计时器每秒加一
void MainWindow::on_sectime()
{
    mineMap.timer++;
    update();
}

