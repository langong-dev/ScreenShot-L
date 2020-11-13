#ifndef DRAWWIDGET_H
#define DRAWWIDGET_H

#include <QWidget>

class DrawWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DrawWidget(QPixmap *set, QWidget *parent = nullptr);

    void setStyle (int s);
    void setColor(QColor c);

    int style, weight;
    QColor color;
    QPoint startPos;
    QPixmap *pix;
    double ox, oy, r, rw, rh;
    double px, py;
    int ifx, ify;

private:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent* event);
//    void mouseReleaseEvent(QMouseEvent *event);
//    void keyPressEvent(QKeyEvent *event);
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

public slots:
    void setWidth(int w);
    void cleari();

signals:

};

#endif // DRAWWIDGET_H
