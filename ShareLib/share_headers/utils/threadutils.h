#ifndef THREAD_UTIL_H
#define THREAD_UTIL_H

#include <QThread>

// This function split the "abc123def456" to a list of (abc, 123, def, 456).
QStringList splitStringByNumber(const QString & str);

// Smart string compare function. which will consider the number in the
// string.
class ThreadUtil: public QThread
{
public:
	static void msleep(unsigned long msec)
	{
		QThread::msleep(msec);
	}
};
#endif // THREAD_UTIL_H
