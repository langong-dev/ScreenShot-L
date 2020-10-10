#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>

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

    //void on_delete_btn_clicked();

private:
    Ui::MainWindow *ui;

    void putimg(QPixmap img);

public:

    QSystemTrayIcon *trayIcon;
    QPixmap nowimage;
    QString path;
    QString statusHead;

//Q_SIGNALS:

};
#endif // MAINWINDOW_H
