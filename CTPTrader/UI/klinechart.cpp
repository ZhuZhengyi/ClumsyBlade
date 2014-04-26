#include "klinechart.h"

#include <iostream>
using namespace std;

namespace KLine{
int mLeft = 50;
int mRight = 80;
int mTop = 50;
int mBottom = 50;
int X = -1;
int Y = -1;
}

KLineChart::KLineChart(QWidget *parent) :
    QWidget(parent)
{
    this->setMouseTracking(true);
}

void KLineChart::mouseMoveEvent(QMouseEvent * event)
{
    cout << event->x() << "," << event->y() << endl;
    if(event->x() >= KLine::mLeft && event->x() <= this->width() - KLine::mRight && event->y() >= KLine::mTop && event->y() <= this->height() - KLine::mBottom)
    {
        KLine::X = event->x();
        KLine::Y = event->y();
    }
    else
    {
        KLine::X = -1;
        KLine::Y = -1;
    }

    update();
}

void KLineChart::resizeEvent(QResizeEvent *event)
{
}

void KLineChart::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setPen(QColor(0, 0, 0));
    painter.fillRect(QRectF(0, 0, this->width(), this->height()), Qt::SolidPattern);

}
