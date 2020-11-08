#ifndef DRAW_H
#define DRAW_H

#include "drawwidget.h"

#include <QWidget>
#include <QMainWindow>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QToolButton>
#include <QPushButton>

class Draw : public QMainWindow
{
    Q_OBJECT
public:
    explicit Draw(QPixmap setpix, QMainWindow *parent = nullptr);

public:
    void createToolBar();

    QLabel *styleLabel, *widthLabel, *titl, *colorlabel;
    QComboBox *styleComboBox;
    QSpinBox *widthSpinBox;
    QToolButton *colorBtn, *clearBtn;
    DrawWidget *drawWidget;
    QPushButton *okbtn;
    QToolBar *toolBar;

private slots:
    void ShowStyle();
    void ShowColor();
    void oky();

signals:
    void oksig(QPixmap px);

};

#endif // DRAW_H
