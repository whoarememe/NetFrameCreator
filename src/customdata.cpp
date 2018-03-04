#include "customdata.h"
#include "ui_customdata.h"
#include <QDebug>

CustomData::CustomData(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CustomData)
{
    ui->setupUi(this);
}

CustomData::CustomData(char *buf, char *tmpBuf) :
    QWidget(0),
    ui(new Ui::CustomData)
{
    ui->setupUi(this);

    this->buf = buf;

    this->tmpBuf = tmpBuf;
}

int CustomData::fillData(int index, int mtu)
{
    if (this->isHidden())
    {
        return 0;
    }

    if (ui->textEdit->toPlainText().length() <= 0) return 0;

    QByteArray bd = ui->textEdit->toPlainText().toLatin1();

    //判断是不是大于剩余空间，是的话暂时丢弃
    if (bd.length() > (mtu - index))
    {
        memcpy((buf+index), bd.data(), (mtu-index));

        return mtu-index;
    }
    else
    {
        memcpy((buf+index), bd.data(), bd.length());

        return bd.length();
    }
}

CustomData::~CustomData()
{
    delete ui;
}
