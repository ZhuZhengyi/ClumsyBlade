#ifndef FILESYSTEMHELPER_H
#define FILESYSTEMHELPER_H

#include <QtCore>

#if defined(__WIN32) || defined(__WINNT)
#include "shlobj.h"
#endif

void createDirInNotExist(QString path);

bool DeleteDirectory(QString DirName);
bool deleteDirectoryContent(QString &dirName);
QString getSystemFolder(int folder_id);

#endif // FILESYSTEMHELPER_H
