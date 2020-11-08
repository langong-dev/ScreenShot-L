#include "draw.h"

#include "drawwidget.h"

#include <QToolBar>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QColorDialog>
#include <QMessageBox>
#include <QDebug>
#include <QFont>

Draw::Draw(QPixmap setpix, QMainWindow *parent) : QMainWindow(parent)
{
    QPixmap *x = new QPixmap(setpix);
    x->scaled(size());
    drawWidget =new DrawWidget(x,this);
    this->setCentralWidget(drawWidget);
    createToolBar();    //创建一个工具栏
    setMinimumSize(600, 400);
    ShowStyle();    //显示选择的风格,初始化风格，设置控件中的当前值作为选项
    widthSpinBox->setValue(6);
    drawWidget->setWidth(widthSpinBox->value());    //初始化画笔宽度
    drawWidget->setColor(Qt::black);    //初始画笔为黑色

//    drawWidget->pix = x;
}

void Draw::createToolBar()
{
  //首先得到一个工具栏
  toolBar = this->addToolBar("Tool");

  //首先是设置风格

  titl = new QLabel(tr("Edit  "));
  QFont font = titl->font();
  font.setWeight(QFont::Bold);
  titl->setFont(font);
  styleLabel = new QLabel(tr(" Line style  "));    //全都是线性的风格
  colorlabel = new QLabel(tr(" Color  "));
  styleComboBox = new QComboBox;
  //各种风格，这个Qt的说明文档里面有，粘过来就好，这里强制转换为int类型，后面可以转换回来
  styleComboBox->addItem(tr("SolidLine"),static_cast<int>(Qt::SolidLine));
  styleComboBox->addItem(tr("DashLine"),static_cast<int>(Qt::DashLine));
  styleComboBox->addItem(tr("DotLine"),static_cast<int>(Qt::DotLine));
  styleComboBox->addItem(tr("DashDotLine"),static_cast<int>(Qt::DashDotLine));
  styleComboBox->addItem(tr("DashDotDotLine"),static_cast<int>(Qt::DashDotDotLine));
  connect(styleComboBox, SIGNAL(activated(int)), this, SLOT(ShowStyle()));

  //设置画笔线宽
  widthLabel =new QLabel(tr(" Line width  "));    		//创建线宽选择控件
  widthSpinBox =new QSpinBox;
//  connect(widthSpinBox,SIGNAL(valueChanged(int)),drawWidget,SLOT(setWidth(int)));
  connect(widthSpinBox, SIGNAL(valueChanged(int)), drawWidget, SLOT(setWidth(int)));

  colorBtn =new QToolButton;                  //创建颜色选择控件
  QPixmap pixmap(20,20);
  pixmap.fill(Qt::black);
  colorBtn->setIcon(QIcon(pixmap));
  connect(colorBtn,SIGNAL(clicked()),this,SLOT(ShowColor()));

//  clearBtn =new QToolButton();               	//创建清除按钮
//  clearBtn->setText(tr("清除"));
//  connect(clearBtn,SIGNAL(clicked()),drawWidget,SLOT(cleari()));


  okbtn = new QPushButton();
  okbtn->setText(tr("OK"));
  connect (okbtn, SIGNAL(clicked()), this, SLOT(oky()));

  toolBar->addWidget(titl);
  toolBar->addWidget(styleLabel);
  toolBar->addWidget(styleComboBox);
  toolBar->addWidget(widthLabel);
  toolBar->addWidget(widthSpinBox);
  toolBar->addWidget(colorlabel);
  toolBar->addWidget(colorBtn);
  toolBar->addSeparator();
  toolBar->addWidget(okbtn);
//  toolBar->addWidget(clearBtn);
}

void Draw::oky()
{
    emit oksig(*drawWidget->pix);
    toolBar->close();
    this->hide();
}


void Draw::ShowStyle()
{
    drawWidget->setStyle(styleComboBox->itemData(styleComboBox->currentIndex(), Qt::UserRole).toInt());
}


void Draw::ShowColor()
{
  //重颜色对话框中取得颜色
  QColor color = QColorDialog::getColor(static_cast<int>(Qt::black), this);   //默认是黑色

  //取得的颜色值赋值到绘画区，和按钮图片
  if(color.isValid())
    {
      drawWidget->setColor(color);    //设置画笔颜色
      QPixmap p(20, 20);
      p.fill(color);
      colorBtn->setIcon(QIcon(p));
    }

}
