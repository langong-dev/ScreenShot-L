#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_start_clicked();
    void onCompleteCature(QPixmap captureImage);


    void on_save_clicked();

    void on_copy_clicked();

    void on_about_clicked();

    void on_refresh_clicked();

    void on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason);

    void slotActionExitApp();

    void slotShowwindow();

    void replyFinished(QNetworkReply *reply);

    //void on_delete_btn_clicked();

    void on_getupdate_clicked();

private:
    Ui::MainWindow *ui;

    void putimg(QPixmap img);

    void closeEvent(QCloseEvent *event);

    QNetworkAccessManager *manager;

    int parse_UpdateJSON(QString str);

public:

    void startcap();

    QString Version;

public:

    QSystemTrayIcon systemTrayIcon;
    QPixmap nowimage;
    QString path;
    QString statusHead;
    QMenu menu;

//Q_SIGNALS:

};
#endif // MAINWINDOW_H
