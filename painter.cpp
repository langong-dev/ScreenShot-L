#include "painter.h"
#include "ui_painter.h"

#include <QPixmap>
#include <QPainter>
#include <QPen>
#include <QMouseEvent>

Painter::Painter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Painter)
{
    ui->setupUi(this);
    setAutoFillBackground(true);    //自动设定背景颜色
    setPalette(QPalette(Qt::white));      //设置调色板的颜色为白色
    pix = new QPixmap(size());    //这个pixmap对象用来接受准备绘制到空间的内容
    pix->fill(Qt::white);     //填充这个图片的背景是白色
    setMinimumSize(600,400);     //设置绘图区域窗体的最小大小
}

Painter::~Painter()
{
    delete ui;
}

void Painter::setStyle(int s)
{
    style=s;
}

void Painter::setWidth(int w)    //设置线段宽
{
    weight=w;
}

void Painter::setColor(QColor c)
{
    color=c;
}

void Painter::mousePressEvent(QMouseEvent *e)
{
  //记录第一次点击的鼠标的起始坐标
  startPos = e->pos();
}

//然后拖拉鼠标，寻找到一个新的结束点，然后绘画到图片
void Painter::mouseMoveEvent(QMouseEvent *e)
{
  //首先得到绘画的工具
  QPainter *painter = new QPainter;

  //设置好画笔的风格
  QPen pen;
  pen.setStyle((Qt::PenStyle)style);    //设置画笔的风格
  pen.setWidth(weight);   //设置画笔的宽度
  pen.setColor(color);    //设置画笔的颜色

  painter->begin(pix);      //首先在图片上画图
  painter->setPen(pen);   //把设置好的画笔给绘图工具
  painter->drawLine(startPos, e->pos());    //画一条直线，重起点到结束
  painter->end();   //结束绘画,和上面的begin对应，就是结束在图片上的绘画
  startPos = e->pos();      //把这个结束的坐标做一个一个新的开始，这样就可以画多边形了

  this->update();   //更新空间的画面，多次的update调用结果只调用一次paintEvent事件

}

//在调用paintEven之前，Qt会清除掉相应空间的内容,上面的update会调用paintEvent事件
void Painter::paintEvent(QPaintEvent *)
{
  QPainter painter(this);   //得到当前控件绘制工具
  painter.drawPixmap(QPoint(0, 0), *pix);     //重0,0开始绘制这个图片
}
