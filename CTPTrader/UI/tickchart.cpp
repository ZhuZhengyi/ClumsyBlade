#include "tickchart.h"
#include "ui_tickchart.h"
#include <iostream>
using namespace std;

qreal big, small;
qreal maxRate;
qreal realMax;
qreal maxTemp;

int mLeft = 50;
int mRight = 80;
int mTop = 50;
int mBottom = 50;
int halfTextHeight = 5;
int sW = 1;
int mW = 2;
int bW = 3;
int percentWidth = 60;
int steps[] = {50, 100, 200, 500, 1000};
int currentTickIndex = 2;
int X = -1;
int Y = -1;
QString localInstrumentID = "unknown";

double PreClosePrice = -1;

/*
 *
 */
TickChart::TickChart(QWidget *parent) :
    QWidget(parent)
{
    this->setMouseTracking(true);
}

void TickChart::mouseMoveEvent(QMouseEvent * event)
{
    // cout << event->x() << "," << event->y() << endl;
    if(event->x() >= mLeft && event->x() <= this->width() - mRight && event->y() >= mTop && event->y() <= this->height() - mBottom)
    {
        X = event->x();
        Y = event->y();
    }
    else
    {
        X = -1;
        Y = -1;
    }

    update();
}

/*
 *
 */
void TickChart::clearAllData(QString InstrumentID)
{
    localInstrumentID = InstrumentID;
    QFile file("data/" + InstrumentID + ".dat");
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << __FUNCTION__ << "open failed!";
        return;
    }
    qint64 size = file.size();

    qint32 totalUnitSize = sizeof(double) * 2 + sizeof(int) + 9;
    totalUnitSize = 40;
    qint64 count = size / totalUnitSize;
    cout   << "file size=" << size << endl;
    cout   << "file totalUnitSize=" << totalUnitSize << endl;
    cout << "clearAllData sizeof(double)=" << sizeof(double) << endl;
    cout << "clearAllData sizeof(int)=" << sizeof(int) << endl;
    cout << "clearAllData size / totalUnitSize=" << count << endl;
    cout << "clearAllData size % totalUnitSize="  << size % totalUnitSize << endl;
    QDataStream ds(&file);
    ds.setVersion(QDataStream::Qt_4_8);

    double LastPrice = -1;
    double tempPreClosePrice = -1;
    QString UpdateTime;
    int UpdateMillisec;

    if(count == 0)
    {
        return;
    }

    int realCount = count;

    if(count > steps[currentTickIndex])
    {
        qint64 offset = totalUnitSize * (count - steps[currentTickIndex]);
        cout << "offset=" << offset << endl;
        file.seek(offset);
        realCount = steps[currentTickIndex];
    }

    PreClosePrice = tempPreClosePrice;
    QList<qreal> temp;
    for (int i = 0; i < realCount; i++)
    {
        ds >> LastPrice
           >> tempPreClosePrice
           >> UpdateTime
           >> UpdateMillisec;
        cout << "pDepthMarketData->LastPrice="  << LastPrice << endl;
        cout << "pDepthMarketData->PreClosePrice="  << tempPreClosePrice << endl;
        cout << "pDepthMarketData->UpdateTime="  << UpdateTime.toStdString() << endl;
        cout << "pDepthMarketData->UpdateMillisec="  << UpdateMillisec << endl << endl;
        if (PreClosePrice == -1)
        {
            PreClosePrice = tempPreClosePrice;
        }
        temp += LastPrice;
    }
    file.close();
    list = temp;

    getBigAndSmall();
    repaint();
}

void TickChart::updateTick(CThostFtdcDepthMarketDataField* data)
{
    // cout << "TickChart::updateTick->" << data->InstrumentID << endl;
    list += data->LastPrice;

    PreClosePrice = data->PreClosePrice;

    getBigAndSmall();
    repaint();
}

TickChart::~TickChart()
{
}

void TickChart::resizeEvent(QResizeEvent *event)
{
}

void TickChart::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setPen(QColor(0, 0, 0));
    painter.fillRect(QRectF(0, 0, this->width(), this->height()), Qt::SolidPattern);

    drawAxis();
    drawData();
    drawOthers();

}

/*
 *
 */
void TickChart::drawOthers()
{
    QPainter painter(this);
    pen.setWidth(sW);
    pen.setStyle(Qt::SolidLine);
    pen.setColor(QColor(255, 255, 255));
    painter.setPen(pen);

    painter.drawLine(X, mTop, X, this->height() - mBottom);
    painter.drawLine(mLeft, Y, this->width() - mRight, Y);
}

void TickChart::getBigAndSmall()
{
    big = small = list.at(0);
    int size = list.size();
    for(int i = 1; i < size; i++)
    {
        if(list.at(i) > big)
        {
            big = list.at(i);
        }
        else if (list.at(i) < small)
        {
            small = list.at(i);
        }
    }

    // cout << "big=" << big << " small=" << small << endl;
    maxRate = (big - PreClosePrice) > (PreClosePrice - small) ? (big - PreClosePrice) : (PreClosePrice - small);
    realMax = PreClosePrice + maxRate;
    maxRate /= PreClosePrice;

    // cout << "maxRate=" << maxRate << endl;

    maxTemp = maxRate / 9;
    // cout << "maxTemp=" << maxTemp << endl;

}

/*
 * 绘制数据线
 */
void TickChart::drawData()
{
    int size = list.size();
    if(size > 0)
    {
        int length = size < steps[currentTickIndex] ?  steps[currentTickIndex] : size;
        qreal stepWidth = (this->width() - mLeft - mRight) / (qreal)length;
        QPainterPath path;
        qreal newX = mLeft;
        const int realHeight = (this->height() - mTop - mBottom) / 2;
        qreal newY = mTop + realHeight * (realMax - list.at(0)) / (realMax - PreClosePrice);
        path.moveTo(newX, newY);
        if(size > 1)
        {
            for (int i = 1; i < size; i++)
            {
                newX = mLeft + i * stepWidth;
                newY = mTop + realHeight * (realMax - list.at(i)) / (realMax - PreClosePrice);
                // cout << "list.at(" << i << ")=" << list.at(i) << " (" << newX << "," << newY << ") ";
                path.lineTo(newX, newY);
            }
            // cout << endl;
        }

        QPainter painter(this);
        pen.setWidth(sW);
        pen.setStyle(Qt::SolidLine);
        pen.setColor(QColor(255, 255, 255));
        painter.setPen(pen);

        painter.drawPath(path);
    }
}

/*
 * 绘制坐标网格
 */
void TickChart::drawAxis()
{
    QPainter painter(this);

    pen.setWidth(sW);
    pen.setStyle(Qt::SolidLine);
    pen.setColor(QColor(255, 0, 0));
    painter.setPen(pen);
    QVector<QLineF> points;
    float lineHeight = (this->height() - mTop - mBottom) / 18.0;
    for(int k = 0; k < 19; k++)
    {
        if(k == 9)
        {
            k++;
        }
        points.append(QLineF(mLeft, mTop + k * lineHeight, this->width() - mRight, mTop + k * lineHeight));
    }
    painter.drawLines(points);

    //draw horizonal center line
    pen.setWidth(bW);
    pen.setStyle(Qt::SolidLine);
    pen.setColor(QColor(255, 0, 0));
    painter.setPen(pen);
    painter.drawLine(mLeft + 2, mTop + 9 * lineHeight, this->width() - mRight - 2, mTop + 9 * lineHeight);

    float lineWidth = (this->width() - mLeft -mRight) / 8.0;

    points.clear();
    for(int k = 0; k < 9; k += 2)
    {
        if(k == 4)
        {
            k += 2;
        }
        points.append(QLineF(mLeft + k * lineWidth, mTop, mLeft + k * lineWidth, this->height() - mBottom));
    }
    pen.setWidth(sW);
    pen.setStyle(Qt::SolidLine);
    pen.setColor(QColor(255, 0, 0));
    painter.setPen(pen);
    painter.drawLines(points);

    //draw vertical center line
    pen.setWidth(mW);
    pen.setStyle(Qt::SolidLine);
    pen.setColor(QColor(255, 0, 0));
    painter.setPen(pen);
    painter.drawLine(QLineF(mLeft + 4 * lineWidth, mTop + 1, mLeft + 4 * lineWidth, this->height() - mBottom - 1));

    points.clear();
    for(int k = 1; k <= 7; k += 2)
    {
        points.append(QLineF(mLeft + k * lineWidth, mTop, mLeft + k * lineWidth, this->height() - mBottom));
    }

    pen.setWidth(sW);
    pen.setStyle(Qt::DashLine);
    pen.setColor(QColor(255, 0, 0));
    painter.setPen(pen);

    painter.drawLines(points);

    // draw Texts
    pen.setColor(QColor(255, 255, 255));
    painter.setPen(pen);
    painter.drawText(QRectF(this->width() - mRight + 2, mTop + 9 * lineHeight - halfTextHeight, percentWidth, 2 * halfTextHeight), Qt::AlignCenter, "0.0%");

    pen.setColor(QColor(255, 0, 0));
    painter.setPen(pen);

    for(int k = 0; k < 9; k++)
    {
        QString str;
        if(k == 0)
        {
            str.sprintf("%0.6f", maxRate);
        }
        else
        {
            str.sprintf("%0.6f", maxTemp * (9 - k));
        }
        painter.drawText(QRectF(this->width() - mRight + 2, mTop + k * lineHeight - halfTextHeight, percentWidth, 2 * halfTextHeight), Qt::AlignCenter, str + "%");
    }

    pen.setColor(QColor(0, 255, 0));
    painter.setPen(pen);

    for(int k = 10; k < 19; k++)
    {
        QString str;
        if(k == 18)
        {
            str.sprintf("%0.6f", maxRate);
        }
        else
        {
            str.sprintf("%0.6f", maxTemp * (k - 9));
        }
        painter.drawText(QRectF(this->width() - mRight + 2, mTop + k * lineHeight - halfTextHeight, percentWidth, 2 * halfTextHeight), Qt::AlignCenter, str + "%");
    }
}
