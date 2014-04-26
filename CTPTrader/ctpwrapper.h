#ifndef CTPWRAPPER_H
#define CTPWRAPPER_H

#include <QObject>
#include "PVSConstant.h"

class CTPWrapper : public QObject
{
    Q_OBJECT
public:
    CTPWrapper(QObject *parent = 0);
    
signals:    
    void outputSignal(QString, bool append=true);
    void setNewStateSignal(PVSniper::PVS_STATE);
    
public slots:
    
};

#endif // CTPWRAPPER_H
