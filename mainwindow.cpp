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

//SystemTray
#include <QDesktopServices>
#include <QCloseEvent>
#include <QHideEvent>
#include <QKeyEvent>
#include <QShortcut>
#include <Qt>
#include <QScreen>

//NetWork
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

//JSON
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Version = "1.2.7";
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));

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
    QScreen *screen = QGuiApplication::primaryScreen();
    QPixmap map = screen->grabWindow(0);
    CaptureScreen* captureHelper = new CaptureScreen(map);
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

void MainWindow::on_getupdate_clicked()
{
//    QNetworkAccessManager *manager;
    QNetworkRequest quest;
    quest.setUrl(QUrl("https://langong-dev.github.com/ScreenShot-L/versions.json"));
    quest.setHeader(QNetworkRequest::UserAgentHeader,"RT-Thread ART");
    manager->get(quest);
}

void MainWindow::replyFinished(QNetworkReply *reply)
{
    QString str = reply->readAll();//读取接收到的数据
    //    qDebug() << str;
    parse_UpdateJSON(str);
    reply->deleteLater();               //销毁请求对象
}

int MainWindow::parse_UpdateJSON(QString str)
{
    //    QMessageBox msgBox;
    QJsonParseError err_rpt;
    QJsonDocument  root_Doc = QJsonDocument::fromJson(str.toUtf8(),&err_rpt);//字符串格式化为JSON
    if(err_rpt.error != QJsonParseError::NoError)
    {
//        qDebug() << "root格式错误";
        QMessageBox::critical(this, "检查失败", "服务器地址错误或JSON格式错误!");
        return -1;
    }
    if(root_Doc.isObject())
    {
        QString url;
        QJsonObject  root_Obj = root_Doc.object();
        QString topver = root_Obj.value("version").toString().trimmed();
        QJsonObject PulseValue = root_Obj.value(topver).toObject();
        QJsonObject urlObj = PulseValue.value("url").toObject();
        int software = 0;
#if defined (Q_OS_LINUX)
        software = 1;
        url = urlObj.value("linux").toString().trimmed();
#endif
#if defined (Q_OS_WIN)
        software = 2;
        url = urlObj.value("windows").toString().trimmed();
#endif
#if defined (Q_OS_OSX)
        software = 3;
        url = urlObj.value("mac").toString().trimmed();
#endif
        if (software == 0)
        {
            QMessageBox::information(this, "Error", "Cannot get your system id!");
        }
        QString verison = topver;
        QString UpdateTime = PulseValue.value("updtime").toString();
        QString ReleaseNote = PulseValue.value("new").toString();
        if(verison > this->Version)
        {
            QString warningStr =  "We had got a new version!\nVersion: " + verison + "\n" + "Update time: " + UpdateTime + "\n" + "Infomation" + ReleaseNote;
            int ret = QMessageBox::warning(this, "Get Update",  warningStr, "Update", "Miss");
            if(ret == 0)    //点击更新
            {
                QDesktopServices::openUrl(QUrl(url));
            }
        }
        else
            QMessageBox::information(this, "Get Update", "This is the latest version!");
    }
    return 0;
}
