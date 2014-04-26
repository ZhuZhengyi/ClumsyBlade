#include "pvsplashscreen.h"

PVSplashScreen::PVSplashScreen(const QPixmap &pixmap, Qt::WindowFlags f) :
    QSplashScreen(pixmap, f)
{
    progressBar = new QProgressBar(this); //
    progressBar->setGeometry(0, height() - 32, width(), 32);
    progressBar->setValue(0);
    progressBar->setAlignment(Qt::AlignCenter);

    connect(progressBar, SIGNAL(valueChanged(int)), this, SLOT(progressChanged(int)));

    QFont font;
    font.setPixelSize(18);

    progressBar->setFont(font); // setFont
    progressBar->setTextVisible(true);

    // progressBar->setTextDirection(QProgressBar::TopToBottom);

}

void PVSplashScreen::setProgress(int value)
{
    progressBar->setValue(value);
}

void PVSplashScreen::setRange(int min, int max)
{
    progressBar->setRange(min, max);
}


void PVSplashScreen::progressChanged(int)
{
    repaint();
}
