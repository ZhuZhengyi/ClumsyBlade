#ifndef STRINGUTIL_H
#define STRINGUTIL_H

#include <QString>

QString checkValue(double val, const char * format = "%0.2f", QString defaultValue = "---");
QString checkValue(int val, const char * format = "%d", QString defaultValue = "---");

#endif // STRINGUTIL_H
