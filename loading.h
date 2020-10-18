#ifndef LOADING_H
#define LOADING_H

#include <QDialog>

namespace Ui {
class Loading;
}

class Loading : public QDialog
{
    Q_OBJECT

public:
    explicit Loading(QString title, QString contect, QWidget *parent = nullptr);
    ~Loading();

private:
signals:
    void sendsignal();

private slots:
    void on_exit_clicked();

    void clo();

private:
    Ui::Loading *ui;
};

#endif // LOADING_H
