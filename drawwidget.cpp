#include "drawwidget.h"

#include <QPixmap>
#include <QMouseEvent>
#include <QPainter>
#include <QPen>
#include <Qt>
#include <QResizeEvent>
#include <QPalette>
#include <QMessageBox>
#include <QDebug>
#include <QSize>

DrawWidget::DrawWidget(QPixmap *set,QWidget *parent) : QWidget(parent)
{

    setAutoFillBackground(true);    //自动设定背景颜色
    setPalette(QPalette(Qt::white));      //设置调色板的颜色为白色
    pix=new QPixmap(size());    //这个pixmap对象用来接受准备绘制到空间的内容
//    pix->fill(Qt::white);     //填充这个图片的背景是白色
//    pix->fromImage(set->toImage());

//    QSize x = size();
//    int width=size().width();
//    int height=size().height();
//    QSize *x = new QSize(width, height);
//    pix = new QPixmap(set->scaled(*x, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    pix = new QPixmap(*set);
//    set->scaledToWidth(Qt::KeepAspectRatio, Qt::SmoothTransformation);
//    ui->show->setAlignment(Qt::AlignCenter);
//    ui->show->setPixmap(fitpixmap);
//    pix = set;
    setMinimumSize(600,400);     //设置绘图区域窗体的最小大小
}

void DrawWidget::setStyle(int s)
{
  style=s;
}

void DrawWidget::setWidth(int w)
{
  weight=w;
}

void DrawWidget::setColor(QColor c)
{
  color=c;
}

void DrawWidget::mousePressEvent(QMouseEvent *e)
{
    //记录第一次点击的鼠标的起始坐标
//    startPos = e->pos();
    qDebug() << e->pos().x()/rw << " " << e->pos().y()/rh << " " << px / rw;
    QPoint n((e->pos().x()/2/rw - px), e->pos().y()/rh/py);
    qDebug() << n.x() << " " << n.y();

    startPos = n;
}

//然后拖拉鼠标，寻找到一个新的结束点，然后绘画到图片
void DrawWidget::mouseMoveEvent(QMouseEvent *e)
{
    //首先得到绘画的工具
    QPainter *painter = new QPainter;
    QPoint n((e->pos().x()/2/rw - px), e->pos().y()/rh/py);

    //设置好画笔的风格
    QPen pen;
    pen.setStyle((Qt::PenStyle)style);    //设置画笔的风格
    pen.setWidth(weight);   //设置画笔的宽度
    pen.setColor(color);    //设置画笔的颜色

    painter->begin(pix);      //首先在图片上画图
    painter->setPen(pen);   //把设置好的画笔给绘图工具
    painter->drawLine(startPos, n);    //画一条直线，重起点到结束
    painter->end();   //结束绘画,和上面的begin对应，就是结束在图片上的绘画
    startPos = n;      //把这个结束的坐标做一个一个新的开始，这样就可以画多边形了

    this->update();   //更新空间的画面，多次的update调用结果只调用一次paintEvent事件

}

//在调用paintEven之前，Qt会清除掉相应空间的内容,上面的update会调用paintEvent事件
void DrawWidget::paintEvent(QPaintEvent *)
{
    double ww = width();
    double wh = height();
    double iw = pix->width();
    double ih = pix->height();
    rw = 1.0 * ww / iw;
    rh = 1.0 * wh / ih;
    r = qMin(rw, rh);
    ox = (ww - r * iw) / 2;
    oy = (wh - r * ih) / 2;
    if (ww - iw > 0)
    {
        px = 1.0 * ox;
        py = 1;
//        px = 0, py = ((wh - ih) / 2);
    }
    else
    {
        px = py = rh;
        qDebug() << ww << " " << iw;
    }
    QPainter painter(this);   //得到当前控件绘制工具
    painter.translate(ox, oy);
    painter.scale(r,r);
    painter.drawPixmap(QPoint(0, 0), *pix);     //重0,0开始绘制这个图片
//    setMinimumSize(pix->size());
}

void DrawWidget::cleari()
{    QPixmap *clearPix = new QPixmap(size());
     clearPix->fill(Qt::white);
     pix = clearPix;
     update();

}

void DrawWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);    //完成其余默认的工作
//    if(height() > pix->height() || width() > pix->width())    //只要控件的区域比画布还要大，那么我们就改到一样大
    {
//        int ww = width();
//        int wh = height();
//        int iw = pix->width();
//        int ih = pix->height();
//        rw = 1.0 * ww / iw;
//        rh = 1.0 * wh / ih;
//        r = qMin(rw, rh);
//        ox = (ww - r * iw) / 2;
//        oy = (wh - r * ih) / 2;
//        QPixmap *newPix = new QPixmap(size());    //设置一个新的画布，大小和控件一样大
        //同样先把背景设置为白色
//        newPix->fill(Qt::white);
//        QPixmap *newPix = new QPixmap();
        //设置完了，赋值到原来的地方之前，我们先前的画不能消失，所以早替换新画布之前要保留前面的画
        QPainter p(this);   //在新画布上进行操作
        p.translate(ox, oy);
        p.scale(r,r);
        p.drawPixmap(QPoint(0, 0), *pix);    //把原来的画画上去
        qDebug() << "Y";
//        pix = newPix;
//        pix = new QPixmap (newPix->scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

}
