#include "loading.h"
#include "ui_loading.h"

#include <QPixmap>
#include <QMovie>

Loading::Loading(QString title, QString contect, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Loading)
{
    ui->setupUi(this);

    this->setWindowIcon(QIcon(":/SystemTray/icon.ico"));
    this->setWindowTitle(title);
    this->ui->label_2->setText(contect);
//    QPixmap pixmap;
//    int with = ui->label->width();
//    int height = ui->label->height();
//    pixmap = QPixmap::fromImage(QImage(":/SystemTray/loading.gif"));
//    pixmap.scaled(with, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
//    this->ui->label->setPixmap(pixmap);
    QMovie *movie = new QMovie(":/SystemTray/loading.gif");
    ui->label->setMovie(movie);
    ui->label->setAlignment(Qt::AlignCenter);
    movie->start();
}

Loading::~Loading()
{
    delete ui;
}


void Loading::on_exit_clicked()
{
    emit sendsignal();
    this->close();
}

void Loading::clo()
{
    this->close();
}
