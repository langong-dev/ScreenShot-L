#include "about.h"
#include "ui_about.h"

#include <QApplication>

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);
}

About::~About()
{
    delete ui;
}

void About::closeEvent(QCloseEvent *event)
{
    this->hide();
    event->ignore();
}


void About::on_aboutqt_clicked()
{
    qApp->aboutQt();
}
