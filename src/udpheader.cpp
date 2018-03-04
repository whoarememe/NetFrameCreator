#include "udpheader.h"
#include "ui_udpheader.h"

UdpHeader::UdpHeader(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UdpHeader)
{
    ui->setupUi(this);
}

UdpHeader::UdpHeader(char *buf, NetCard *netCard):
    QWidget(0),
    ui(new Ui::UdpHeader)
{
    ui->setupUi(this);

    this->buf = buf;

    this->netCard = netCard;
}

int UdpHeader::fillData(int index, int mtu)
{
    if (this->isHidden())
    {
        return 0;
    }

    bool ok;
    udph = (struct udphdr *)(buf + index);

    udph->source = ui->lineEdit_16srcPort->text().toUShort(&ok, 16);
    udph->dest = ui->lineEdit_16dstPort->text().toUShort(&ok, 16);
    udph->len = ui->lineEdit_16UdpLen->text().toUShort(&ok, 16);
    udph->check = ui->lineEdit_16UdpCheckSum->text().toUShort(&ok, 16);

    return sizeof(struct udphdr);
}

UdpHeader::~UdpHeader()
{
    delete ui;
}

void UdpHeader::setDefaultInfo()
{
    QTime time;
    time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);

    /*进行随机设置*/
    ui->lineEdit_16srcPort->setText(QString::number((unsigned int)(qrand()), 16));
    ui->lineEdit_16dstPort->setText(QString::number((unsigned int)(qrand()), 16));
    ui->lineEdit_16UdpLen->setText(QString::number((unsigned int)(qrand()), 16));
    ui->lineEdit_16UdpCheckSum->setText(QString::number((unsigned int)(qrand()), 16));

}
