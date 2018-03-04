#include "icmpheader.h"
#include "ui_icmpheader.h"

IcmpHeader::IcmpHeader(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IcmpHeader)
{
    ui->setupUi(this);
}

IcmpHeader::IcmpHeader(char *buf, NetCard *netCard):
    QWidget(0),
    ui(new Ui::IcmpHeader)
{
    ui->setupUi(this);

    this->buf = buf;

    this->netCard = netCard;
}

IcmpHeader::~IcmpHeader()
{
    delete ui;
}
