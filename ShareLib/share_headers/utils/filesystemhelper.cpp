#include "filesystemhelper.h"

void createDirInNotExist(QString path)
{
    QDir checkDir(path);
    if (!checkDir.exists())
    {
        checkDir.mkpath(path);
    }
}

bool DeleteDirectory(QString dir)
{
    QDir qdir;
    QString DirName = dir + "_remove";
    if (!qdir.rename(dir, DirName))
    {
        return false;
    }
    if(NULL==DirName)    //the dirname is null
        return false;

    QDir filedir(DirName);
    if(!filedir.exists())
    {
        return false;
    }

    QString tempstr;
    QString tempDir = DirName;
    const QFileInfoList files = filedir.entryInfoList();
    if ( !files.isEmpty() )
    {

        foreach (QFileInfo fi, files)
        {
            tempstr = fi.fileName();
            if ( tempstr == "." || tempstr == ".." ) ; // nothing
            else if ( fi.isDir() )
            {
                tempDir = DirName + QDir::separator() + tempstr;
                DeleteDirectory( tempDir );
            }
            else if ( fi.isFile() )
            {
                QFile tempfile(tempstr);
                filedir.remove( tempfile.fileName() );
                // tempfile.remove();
            }
        }
    }    //end if

    if(!filedir.rmdir(DirName))
       return false;

    // Wait for the deleting is really completed.

    return true;
}

bool deleteDirectoryContent(QString &dirName)
{
    QDir fileDir(dirName);
    if (!fileDir.exists())
    {
        return false;
    }

    const QFileInfoList fileInfoList = fileDir.entryInfoList();

    if (!fileInfoList.isEmpty())
    {
        QString fileName;
        QString subDirName;
        foreach (QFileInfo info, fileInfoList)
        {
            fileName = info.fileName();

            if (fileName == "." || fileName == "..")
            {
                //nothing
            }
            else if (info.isDir()) //recursive calls
            {
                subDirName = dirName + QDir::separator() + fileName;
                deleteDirectoryContent(subDirName);
            }
            else if (info.isFile())
            {
                QFile tempfile(fileName);
                fileDir.remove(tempfile.fileName());
            }

            // delete sub dir
            QDir subDir(subDirName);
            if (subDir.exists(subDirName))
            {
                if (!subDir.rmdir(subDirName))
                {
                    return false;
                }
            }
        }//end foreach
    }//end if

    return true;
}

QString getSystemFolder(int folder_id)
{
    QString text = QDir::currentPath();

#if defined(__WIN32) || defined(__WINNT)
    TCHAR szPath[MAX_PATH];
    SHGetFolderPath(NULL, folder_id, NULL, 0, szPath);
    text = (QString::fromUtf16(reinterpret_cast<const unsigned short *>(szPath)));
#else

#endif

    return text;
}
