#include "frame.h"
#include "ui_frame.h"

Frame::Frame(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Frame)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint);

    //setAttribute(Qt::WA_TranslucentBackground);
}

Frame::~Frame()
{
    delete ui;
}
