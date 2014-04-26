#ifndef KLINECHART_H
#define KLINECHART_H

#include <QtGui>

class KLineChart : public QWidget
{
    Q_OBJECT
public:
    explicit KLineChart(QWidget *parent = 0);

protected:
    void mouseMoveEvent(QMouseEvent * event);
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);
    
signals:
    
public slots:
    
};

#endif // KLINECHART_H
