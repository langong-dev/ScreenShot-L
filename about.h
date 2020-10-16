#ifndef ABOUT_H
#define ABOUT_H

#include <QDialog>
#include <QCloseEvent>

namespace Ui {
class About;
}

class About : public QDialog
{
    Q_OBJECT

public:
    explicit About(QWidget *parent = nullptr);
    ~About();

    void closeEvent(QCloseEvent *event);

private:
    Ui::About *ui;
};

#endif // ABOUT_H
