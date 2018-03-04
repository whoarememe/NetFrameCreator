#include "arpheader.h"
#include "ui_arpheader.h"

ArpHeader::ArpHeader(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ArpHeader)
{
    ui->setupUi(this);
}

ArpHeader::ArpHeader(char *buf, NetCard *netCard):
    QWidget(0),
    ui(new Ui::ArpHeader)
{
    ui->setupUi(this);

    this->buf = buf;

    this->netCard = netCard;

    setDefaultInfo();

    connect(netCard, SIGNAL(netChanged()), this, SLOT(setDefaultInfo()));
}

int ArpHeader::fillData(int index, int mtu)
{
    if (this->isHidden())
    {
        return 0;
    }

    bool ok;

    eth_arp = (struct ether_arp *)(buf + index);

    /*arp头部*/
    eth_arp->arp_hrd = htons(ui->lineEdit_hwType->text().toUShort(&ok, 16));
    eth_arp->arp_pro = htons(ui->lineEdit_proType->text().toUShort(&ok, 16));
    eth_arp->arp_hln = (unsigned char)ui->lineEdit_hwLen->text().toUShort(&ok, 16);
    eth_arp->arp_pln = (unsigned char)ui->lineEdit_proLen->text().toUShort(&ok, 16);
    eth_arp->arp_op = htons(ui->lineEdit_opNum->text().toUShort(&ok, 16));
    qDebug() << ui->lineEdit_opNum->text().toUShort(&ok, 16);

    QStringList l = ui->lineEdit_srcMac->text().split(":");

    /*arp数据部分*/
    //来源以太网
    eth_arp->arp_sha[0] = (uchar)l.at(0).toUShort(&ok, 16);
    eth_arp->arp_sha[1] = (uchar)l.at(1).toUShort(&ok, 16);
    eth_arp->arp_sha[2] = (uchar)l.at(2).toUShort(&ok, 16);
    eth_arp->arp_sha[3] = (uchar)l.at(3).toUShort(&ok, 16);
    eth_arp->arp_sha[4] = (uchar)l.at(4).toUShort(&ok, 16);
    eth_arp->arp_sha[5] = (uchar)l.at(5).toUShort(&ok, 16);

    l = ui->lineEdit_dstMac->text().split(":");
    //目的以太网
    eth_arp->arp_tha[0] = (uchar)l.at(0).toUShort(&ok, 16);
    eth_arp->arp_tha[1] = (uchar)l.at(1).toUShort(&ok, 16);
    eth_arp->arp_tha[2] = (uchar)l.at(2).toUShort(&ok, 16);
    eth_arp->arp_tha[3] = (uchar)l.at(3).toUShort(&ok, 16);
    eth_arp->arp_tha[4] = (uchar)l.at(4).toUShort(&ok, 16);
    eth_arp->arp_tha[5] = (uchar)l.at(5).toUShort(&ok, 16);

    //来源ip
    l = ui->lineEdit_srcIp->text().split(".");
    eth_arp->arp_spa[0] = (uchar)l.at(0).toUShort(&ok, 10);
    eth_arp->arp_spa[1] = (uchar)l.at(1).toUShort(&ok, 10);
    eth_arp->arp_spa[2] = (uchar)l.at(2).toUShort(&ok, 10);
    eth_arp->arp_spa[3] = (uchar)l.at(3).toUShort(&ok, 10);

    //目的ip
    l = ui->lineEdit_dstIp->text().split(".");
    eth_arp->arp_tpa[0] = (uchar)l.at(0).toUShort(&ok, 10);
    eth_arp->arp_tpa[1] = (uchar)l.at(1).toUShort(&ok, 10);
    eth_arp->arp_tpa[2] = (uchar)l.at(2).toUShort(&ok, 10);
    eth_arp->arp_tpa[3] = (uchar)l.at(3).toUShort(&ok, 10);

    return sizeof(struct ether_arp);
}

ArpHeader::~ArpHeader()
{
    delete ui;
}

/*
 * 根据网卡设置默认信息
 */
void ArpHeader::setDefaultInfo()
{
    switch (identity)
    {
    /*0 arp*/
    case 0:
    {
        ui->lineEdit_hwType->setText(QString("0001"));
        ui->lineEdit_proType->setText(QString("0800"));
        ui->lineEdit_hwLen->setText(QString("6"));
        ui->lineEdit_proLen->setText(QString("4"));
        ui->lineEdit_opNum->setText(QString("1"));  //默认为1 arp请求， 2arp应答， 3rarp请求， 4rarp应答
        ui->lineEdit_srcMac->setText(QString("%1:%2:%3:%4:%5:%6")
                                     .arg(QString::number((unsigned char)netCard->getMac()->sa_data[0], 16).toUpper())
                                     .arg(QString::number((unsigned char)netCard->getMac()->sa_data[1], 16).toUpper())
                                     .arg(QString::number((unsigned char)netCard->getMac()->sa_data[2], 16).toUpper())
                                     .arg(QString::number((unsigned char)netCard->getMac()->sa_data[3], 16).toUpper())
                                     .arg(QString::number((unsigned char)netCard->getMac()->sa_data[4], 16).toUpper())
                                     .arg(QString::number((unsigned char)netCard->getMac()->sa_data[5], 16).toUpper()));
        ui->lineEdit_srcIp->setText(QString(QLatin1String(inet_ntoa(((struct sockaddr_in *)netCard->getIp())->sin_addr))));
        ui->lineEdit_dstMac->setText(QString("ff:ff:ff:ff:ff:ff"));
        ui->lineEdit_dstIp->setText(QString("255.255.255.255"));

        break;
    }
    /*1 rarp*/
    case 1:
    {
        ui->lineEdit_hwType->setText(QString("0001"));
        ui->lineEdit_proType->setText(QString("0800"));
        ui->lineEdit_hwLen->setText(QString("6"));
        ui->lineEdit_proLen->setText(QString("4"));
        ui->lineEdit_opNum->setText(QString("3"));  //默认为1 arp请求， 2arp应答， 3rarp请求， 4rarp应答
        ui->lineEdit_srcMac->setText(QString("%1:%2:%3:%4:%5:%6")
                                     .arg(QString::number((unsigned char)netCard->getMac()->sa_data[0], 16).toUpper())
                                     .arg(QString::number((unsigned char)netCard->getMac()->sa_data[1], 16).toUpper())
                                     .arg(QString::number((unsigned char)netCard->getMac()->sa_data[2], 16).toUpper())
                                     .arg(QString::number((unsigned char)netCard->getMac()->sa_data[3], 16).toUpper())
                                     .arg(QString::number((unsigned char)netCard->getMac()->sa_data[4], 16).toUpper())
                                     .arg(QString::number((unsigned char)netCard->getMac()->sa_data[5], 16).toUpper()));
        ui->lineEdit_srcIp->setText(QString(QLatin1String(inet_ntoa(((struct sockaddr_in *)netCard->getIp())->sin_addr))));
        ui->lineEdit_dstMac->setText(QString("ff:ff:ff:ff:ff:ff"));
        ui->lineEdit_dstIp->setText(QString("255.255.255.255"));

        break;
    }
    default:
        break;
    }
}

void ArpHeader::setIdentity(int value)
{
    identity = value;
}
