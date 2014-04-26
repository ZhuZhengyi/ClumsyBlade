#include "lessthan.h"

// This function split the "abc123def456" to a list of (abc, 123, def, 456).
QStringList splitStringByNumber(const QString & str)
{
    int strLength = str.length();
    QStringList list;
    QString digital;
    QString other;
    for (int i = 0; i < strLength; i++) {
        if (str[i].isDigit()) {
            if (!other.isEmpty()) {
                list.append(other);
                other.clear();
            }
            digital.append(str[i]);
        }
        else {
            if (!digital.isEmpty()) {
                list.append(digital);
                digital.clear();
            }
            other.append(str[i]);
        }
    }
    if (!digital.isEmpty()) {
        list.append(digital);
    }
    else if (!other.isEmpty()) {
        list.append(other);
    }
    return list;
}
