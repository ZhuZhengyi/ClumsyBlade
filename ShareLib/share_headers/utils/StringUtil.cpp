#include "StringUtil.h"
#include <QDebug>

QString checkValue(double val, const char * format, QString defaultValue)
{
    //qDebug() << Q_FUNC_INFO;
    if(val >= 1.79769e+308)
    {
        return defaultValue;
    }
    QString showRes;
    showRes.sprintf(format, val);
    return showRes;
}

QString checkValue(int val, const char * format, QString defaultValue)
{
    //qDebug() << Q_FUNC_INFO;
    if(val >= 4294967295)
    {
        return defaultValue;
    }
    QString showRes;
    showRes.sprintf(format, val);
    return showRes;
}
