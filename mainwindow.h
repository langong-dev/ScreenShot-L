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

private:
    Ui::MainWindow *ui;

public:

    QSystemTrayIcon *trayIcon;

//Q_SIGNALS:

};
#endif // MAINWINDOW_H
