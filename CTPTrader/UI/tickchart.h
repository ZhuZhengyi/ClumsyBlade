#ifndef TICKCHART_H
#define TICKCHART_H

#include <QtGui>
#include "../ShareLib/share_headers/ctp_headers/ThostFtdcUserApiStruct.h"

#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

class TickChart : public QWidget
{
    Q_OBJECT

protected:
    void mouseMoveEvent(QMouseEvent * event);
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);

public slots:
    void updateTick(CThostFtdcDepthMarketDataField* data);
    
public:
    explicit TickChart(QWidget *parent = 0);
    ~TickChart();
    void clearAllData(QString);
    
private:
    QPen pen;
    QBrush brush;

    void drawAxis();
    void drawData();
    void drawOthers();

    QList<qreal> list;
    void getBigAndSmall();
};

#endif // TICKCHART_H
