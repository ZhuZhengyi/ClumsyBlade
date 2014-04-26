#ifndef PVSPLASHSCREEN_H
#define PVSPLASHSCREEN_H

#include <QtGui>

class PVSplashScreen : public QSplashScreen
{
    Q_OBJECT

private:
    QProgressBar * progressBar;

public:

    explicit PVSplashScreen(const QPixmap &pixmap = QPixmap(), Qt::WindowFlags f = 0);

    void setProgress(int);
    void setRange(int min, int max);

signals:
    
public slots:

    void progressChanged(int);
    
};

#endif // PVSPLASHSCREEN_H
