
#include "tishi.h"

#include <QBitmap>
#include <QDesktopWidget>
#include <QPainter>

#include "ui_tishi.h"
Tishi::Tishi(QString str) : ui(new Ui::Tishi) {
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint |
                         Qt::WindowStaysOnTopHint);
    QBitmap bmp(this->size());
    bmp.fill();
    QPainter p(&bmp);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.drawRoundedRect(bmp.rect(), 10, 10);
    setMask(bmp);
    this->ID = str;
    QDesktopWidget* desktopWidget = QApplication::desktop();
    nWidth = desktopWidget->width();
    nHeight = desktopWidget->height();
    ui->label->setText(QString::fromLocal8Bit("�յ����ԣ�") + str + QString::fromLocal8Bit( "����Ϣ��"));
    timer = new QTimer();
    timer->setSingleShot(true);
    timer->setInterval(6000);
    connect(timer, &QTimer::timeout, this, &Tishi::Man_show);
    timer->start();
    this->move(nWidth - this->width() - 10, nHeight - this->height() - 10);
    this->show();
}

Tishi::~Tishi() { delete ui; }

void Tishi::SHOW() {
    this->show();
    timer->start();
}

void Tishi::on_noBtn_clicked() { this->close(); }

void Tishi::on_okBtn_clicked() {
    emit See(this->ID);
    this->close();
}

void Tishi::Man_show() { this->close(); }
