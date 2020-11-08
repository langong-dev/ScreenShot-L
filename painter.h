#ifndef PAINTER_H
#define PAINTER_H

#include <QDialog>

#include <QPixmap>
#include <QColor>
#include <QPoint>

namespace Ui {
class Painter;
}

class Painter : public QDialog
{
    Q_OBJECT

public:
    explicit Painter(QWidget *parent = nullptr);
    ~Painter();

    void setStyle(int s);
    void setWidth(int w);
    void setColor(QColor c);

    int style, weight;
    QColor color;
    QPixmap *pix;
    QPoint startPos;

private:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent* event);
//    void mouseReleaseEvent(QMouseEvent *event);
//    void keyPressEvent(QKeyEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    Ui::Painter *ui;
};

#endif // PAINTER_H
