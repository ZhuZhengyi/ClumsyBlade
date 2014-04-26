#ifndef FILENAME_LESSTHAN_H
#define FILENAME_LESSTHAN_H

#include <QString>
#include <QFileInfo>
#include <QStringList>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

// This function split the "abc123def456" to a list of (abc, 123, def, 456).
QStringList splitStringByNumber(const QString & str);

// Smart string compare function. which will consider the number in the
// string.
class StringLessThan {
public:
    bool operator() (const QString & str1, const QString & str2) {
        //Shortcut for equal case.
        if (str1 == str2)
            return true;

        QStringList myStrList = splitStringByNumber(str1);
        QStringList TheirStrList = splitStringByNumber(str2);
        int minListLength = MIN(myStrList.length(), TheirStrList.length());
        for (int i = 0; i < minListLength+1; i++) {
            if (i == minListLength)
                return myStrList.length() < TheirStrList.length();
            QString myStr = myStrList[i];
            QString TheirStr = TheirStrList[i];
            if (myStr.length() > TheirStr.length() && TheirStr[0].isDigit()) {
                int diff = myStr.length() - TheirStr.length();
                TheirStr.prepend(QString(diff, '0'));
            }
            else if (TheirStr.length() > myStr.length() && myStr[0].isDigit()) {
                int diff = TheirStr.length() - myStr.length();
                myStr.prepend(QString(diff, '0'));
            }
            if (myStr < TheirStr)
                return true;
            else if (myStr > TheirStr)
                return false;
        }
        return true;
    }
};

class FileNameLessThan {
public:
    bool operator() (const QFileInfo & f1, const QFileInfo & f2)
    {
        StringLessThan slt;
        return slt(f1.baseName(), f2.baseName());
    }
};


#endif // FILENAME_LESSTHAN_H
