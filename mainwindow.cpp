#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "capturescreen.h"
#include "about.h"
#include "loading.h"

#include <QFileDialog>
#include <QClipboard>
#include <QAction>
#include <QSystemTrayIcon>
#include <QTime>
#include <QDebug>
#include <QMessageBox>
#include <QShortcut>

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

#include "qhotkey.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Version = "1.3.0";
    tv1 = 1, tv2 = 3, tv3 = 0;

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

    auto startcap_hotkey = new QHotkey(QKeySequence("ctrl+alt+p"), true, this);//The hotkey will be automatically registered

    QObject::connect(startcap_hotkey, &QHotkey::activated, this, [&](){
        this->startcap();
    });


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

    exitwindow = new QShortcut(QKeySequence(tr("ctrl+q")), this);
    connect(exitwindow, SIGNAL(activated()), this, SLOT(slotActionExitApp()));
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
            startcap();
            break;
        case QSystemTrayIcon::DoubleClick:
            this->show();
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
    this->ui->status->setText(statusHead + "Getting updates");
    Stopupd = false;
    QString requrl = "https://langong-dev.github.io/ScreenShot-L/versions.json";
    requrl = "http://victorwoo.synology.me:670/screenshot.json";
    Loading *videodlg = new Loading("Loading...","Fetching for updates...",this);
    connect(videodlg,SIGNAL(sendsignal()),this,SLOT(reshow()));
    connect(this, SIGNAL(doneupd()), videodlg, SLOT(clo()));
    videodlg->show();//子界面出现
    QNetworkRequest quest;
    quest.setUrl(QUrl(requrl));
    quest.setHeader(QNetworkRequest::UserAgentHeader,"RT-Thread ART");
    manager->get(quest);
}

void MainWindow::reshow(){
    Stopupd = true;
    this->ui->status->setText(statusHead+"Ready");
    return;
}

void MainWindow::replyFinished(QNetworkReply *reply)
{
    if (Stopupd){
        return;
    }
    QString str = reply->readAll();
    parse_UpdateJSON(str);
    reply->deleteLater();
}

int MainWindow::parse_UpdateJSON(QString str)
{
    QJsonParseError err_rpt;
    QJsonDocument  root_Doc = QJsonDocument::fromJson(str.toUtf8(),&err_rpt);
    if(err_rpt.error != QJsonParseError::NoError)
    {
        QMessageBox::critical(this, "Error", "Cannot get the infomations!");
        emit doneupd();
        return -1;
    }
    if(root_Doc.isObject())
    {
        QString url;
        QJsonObject  root_Obj = root_Doc.object();
        QString topver = root_Obj.value("version").toString().trimmed();
        QJsonObject PulseValue = root_Obj.value(topver).toObject();
        int v1 = PulseValue.value("v1").toInt();
        int v2 = PulseValue.value("v2").toInt();
        int v3 = PulseValue.value("v3").toInt();
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
        if (Stopupd) return 1;
        emit doneupd();
        bool upd = false;
        if (v1>tv1)
        {
            upd = true;
        }
        else if (v1 == tv1)
        {
            if (v2>tv2)
            {
                upd = true;
            }
            else if (v2 == tv2)
            {
                if (v3 > tv3)
                {
                    upd = true;
                }
            }
        }
        if(upd)
        {
            QString warningStr =  "We had got a new version!\nVersion: " + verison + "\n" + "Update time: " + UpdateTime + "\n" + "Infomation:\n" + ReleaseNote;
            int ret = QMessageBox::warning(this, "Get Update",  warningStr, "Update", "Miss");
            if(ret == 0)
            {
                QDesktopServices::openUrl(QUrl(url));
            }
        }
        else{
            QString str = "This is the latest version!\nYour version: ScreenShot-L@"+Version;
            QMessageBox::information(this, "Get Update", str);
        }
    }
    this->ui->status->setText(statusHead+"Ready");
    return 0;
}
