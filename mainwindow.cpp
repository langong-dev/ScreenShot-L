#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "capturescreen.h"
#include "about.h"

#include <QFileDialog>
#include <QClipboard>
#include <QAction>
#include <QSystemTrayIcon>
#include <QTime>
#include <QDebug>
#include <QMessageBox>

#include <QDesktopServices>
#include <QCloseEvent>
#include <QHideEvent>
#include <QKeyEvent>
#include <QShortcut>
#include <Qt>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /*Init Status-bar*/
    statusHead = " Status: ";
    this->ui->status->setText(statusHead+"Ready");

    /*Init System tray*/
    systemTrayIcon.setIcon(QIcon::fromTheme("folder",QIcon(":/SystemTray/LanGongIconMode.PNG")));
    systemTrayIcon.setToolTip("LanGong ScreenShot-L");

    /*Init System tray actions*/
    QAction *showwindow = new QAction("Show Window");
    QAction *menucapture = new QAction("Capture", &menu);
    QAction *gethelp = new QAction("Help / About");
    QAction *exitapp = new QAction("Quit");

    showwindow->setIcon(QIcon(":/SystemTray/LanGongIconMode.PNG"));

    /*Init System tray menu*/
    menu.addAction(showwindow);
    menu.addAction(menucapture);
    menu.addAction(gethelp);
    menu.addSeparator();
    menu.addAction(exitapp);

    /*Connect System tray*/
    connect(&systemTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason)));
    connect(menucapture, SIGNAL(triggered()), this, SLOT(on_start_clicked()));
    connect(exitapp, SIGNAL(triggered()), this, SLOT(slotActionExitApp()));
    connect(gethelp, SIGNAL(triggered()), this, SLOT(on_about_clicked()));
    connect(showwindow, SIGNAL(triggered()), this, SLOT(slotShowwindow()));

    /*Init System tray icon and title*/
    systemTrayIcon.setContextMenu(&menu);
    systemTrayIcon.showMessage("title", "message", QSystemTrayIcon::MessageIcon::Information, 3000);
    systemTrayIcon.show();


    if(!systemTrayIcon.isVisible())
    {
        QMessageBox::information(NULL, "ScreenShot-L", "ScreenShot-L's system tray started error!");
    }
}

void MainWindow::slotActionExitApp()
{
    systemTrayIcon.hide();
    exit(0);
}

void MainWindow::on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
        case QSystemTrayIcon::Trigger:
            this->show();
            break;
        case QSystemTrayIcon::DoubleClick:
            //双击托盘图标
            //双击后显示主程序窗口
            this->show();
//            systemTrayIcon->hide();
            break;
        default:
            break;
    }

}

void MainWindow::slotShowwindow(){
    this->show();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(systemTrayIcon.isVisible())
    {
        hide(); //隐藏窗口
        event->ignore(); //忽略事件
    }
}
//void MainWindow::hideEvent(QHideEvent *event)
//{
//    if(systemTrayIcon.isVisible())
//    {
//        hide(); //隐藏窗口
//        event->ignore(); //忽略事件
//    }
//}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startcap(){
    this->ui->status->setText(statusHead+"Starting Capturer");
    this->hide();
    QTime _Timer = QTime::currentTime().addMSecs(750);
    while (QTime::currentTime() < _Timer)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    CaptureScreen* captureHelper = new CaptureScreen();
        connect(captureHelper, SIGNAL(signalCompleteCature(QPixmap)), this, SLOT(onCompleteCature(QPixmap)));
    captureHelper->show();
    this->show();
}

void MainWindow::on_start_clicked()
{
    startcap();
}

void MainWindow::onCompleteCature(QPixmap captureImage)
{
//    this->ui>show->setPixmap(captureImage);
//    this->setFixedSize(100,100);
    this->show();
//    int nowwidth=this->ui->show->width()/2;
//    QSize picsize(nowwidth, this->ui->show->heightForWidth(nowwidth));
//    captureImage.scaled(this->size(), Qt::KeepAspectRatio);
//    captureImage.scaled(picsize, Qt::KeepAspectRatio);
    //ui->show->setScaledContents(true);

    this->ui->status->setText(statusHead+"Ready");
    nowimage=captureImage;
    path="Unsaved";
    putimg(nowimage);
//    this->ui->path->setText(path);

}

void MainWindow::putimg(QPixmap img)
{
    int with = ui->show->width();
    int height = ui->show->height();
    QPixmap fitpixmap = img.scaled(with, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->show->setAlignment(Qt::AlignCenter);

    ui->show->setPixmap(fitpixmap);
}

void MainWindow::on_save_clicked()
{
    this->ui->status->setText(statusHead+"Waiting QFileDialog");
    QString filename1 = QFileDialog::getSaveFileName(this,tr("Save Image"),"",tr("Images (*.png *.bmp *.jpg)"));
    if(!filename1.isEmpty()&& !filename1.isNull()){
    // functionA
//        ui->show->pixmap()->save(filename1);
        this->ui->status->setText(statusHead+"Writing to file");
        nowimage.save(filename1);
        path=filename1;
//        this->ui->path->setText(path);
    }
    this->ui->status->setText(statusHead+"Ready");
}

void MainWindow::on_copy_clicked()
{
    QClipboard *clipb=QApplication::clipboard();
    //clipb->setImage(ui->show->pixmap()->toImage());
    clipb->setImage(nowimage.toImage());
}

void MainWindow::on_about_clicked()
{
    About* about = new About();
    about->show();
}

void MainWindow::on_refresh_clicked()
{
    putimg(nowimage);
//    this->ui->path->setText(path);
}

//void MainWindow::on_delete_btn_clicked()
//{
//    this->ui->show->clear();
//    nowimage=QPixmap::fromImage(this->ui->show->pixmap()->toImage());
//}
