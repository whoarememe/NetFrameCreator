#include "macheader.h"
#include "ui_macheader.h"
#include <QDebug>

MacHeader::MacHeader(QWidget *parent):
    QWidget(parent),
    ui(new Ui::MacHeader)
{
    ui->setupUi(this);
}

MacHeader::MacHeader(char * buf, NetCard *netCard, sockaddr_ll *saddr_ll):
    QWidget(0),
    ui(new Ui::MacHeader)
{
    ui->setupUi(this);

    ui->comboBox->insertItem(0, tr("Users(16进制)"));
    ui->comboBox->insertItem(1, tr("Arp"));
    ui->comboBox->insertItem(2, tr("Rarp"));
    ui->comboBox->insertItem(3, tr("Ip"));
    ui->comboBox->setCurrentIndex(0);

    this->buf = buf;

    this->netCard = netCard;

    this->saddr_ll = saddr_ll;

    /*设置默认信息*/
//    memcpy(srcAddr, netCard->getMac()->sa_data, 6);

    setDefaultInfo();

    /*网卡改变的时候重新设置默认信息*/
    connect(netCard, SIGNAL(netChanged()), this, SLOT(setDefaultInfo()));
}

/*
 * 当creator点击发送的时候才会进行数据填充，而填充的数据全部来自ui
 * 这就需要ui进行判定数据的输入是否正确,采用高度信任
 */
int MacHeader::fillData(int index, int mtu)
{
    /*判断自己需不需要填充，不需要返回0*/
    if (this->isHidden())
    {
        return 0;
    }

    bool ok;

    /*填充以太网头部*/
    eth_h = (struct ether_header *)(buf+index);

    QStringList l = ui->lineEdit_dstaddr->text().split(":");

    /*设置目的地址*/
    eth_h->ether_dhost[0] = (unsigned char)l.at(0).toUShort(&ok, 16);
    eth_h->ether_dhost[1] = (unsigned char)l.at(1).toUShort(&ok, 16);
    eth_h->ether_dhost[2] = (unsigned char)l.at(2).toUShort(&ok, 16);
    eth_h->ether_dhost[3] = (unsigned char)l.at(3).toUShort(&ok, 16);
    eth_h->ether_dhost[4] = (unsigned char)l.at(4).toUShort(&ok, 16);
    eth_h->ether_dhost[5] = (unsigned char)l.at(5).toUShort(&ok, 16);

    l = ui->lineEdit_srcaddr->text().split(":");
    /*设置来源地址*/
    eth_h->ether_shost[0] = (unsigned char)l.at(0).toUShort(&ok, 16);
    eth_h->ether_shost[1] = (unsigned char)l.at(1).toUShort(&ok, 16);
    eth_h->ether_shost[2] = (unsigned char)l.at(2).toUShort(&ok, 16);
    eth_h->ether_shost[3] = (unsigned char)l.at(3).toUShort(&ok, 16);
    eth_h->ether_shost[4] = (unsigned char)l.at(4).toUShort(&ok, 16);
    eth_h->ether_shost[5] = (unsigned char)l.at(5).toUShort(&ok, 16);

    /*上层协议设定*/
    switch (ui->comboBox->currentIndex()) {
    /*0：用户自定义，为短整型*/
    case 0:
        eth_h->ether_type = htons(ui->comboBox->currentText().toUShort(&ok, 16));
        break;
        /*1: ARP, 0x0806*/
    case 1:
        eth_h->ether_type = htons(0x0806);
        break;
        /*2: RAPR, 0x8035*/
    case 2:
        eth_h->ether_type = htons(0x8035);
        break;
        /*3: Ip, 0x0800*/
    case 3:
        eth_h->ether_type = htons(0x0800);
        break;
    default:
        break;
    }

    return sizeof(struct ether_header);
}

MacHeader::~MacHeader()
{
    delete ui;
}

/*
 * 设置默认显示信息
 * 能默认的就默认显示
 * 这里默认显示的是目的和来源mac地址
 */
void MacHeader::setDefaultInfo()
{
    ui->lineEdit_dstaddr->setText(QString("%1:%2:%3:%4:%5:%6")
            .arg(QString::number((uchar)netCard->getMac()->sa_data[0], 16).toUpper())
            .arg(QString::number((uchar)netCard->getMac()->sa_data[1], 16).toUpper())
            .arg(QString::number((uchar)netCard->getMac()->sa_data[2], 16).toUpper())
            .arg(QString::number((uchar)netCard->getMac()->sa_data[3], 16).toUpper())
            .arg(QString::number((uchar)netCard->getMac()->sa_data[4], 16).toUpper())
            .arg(QString::number((uchar)netCard->getMac()->sa_data[5], 16).toUpper()));

    ui->lineEdit_srcaddr->setText(QString("%1:%2:%3:%4:%5:%6")
            .arg(QString::number((uchar)netCard->getMac()->sa_data[0], 16).toUpper())
            .arg(QString::number((uchar)netCard->getMac()->sa_data[1], 16).toUpper())
            .arg(QString::number((uchar)netCard->getMac()->sa_data[2], 16).toUpper())
            .arg(QString::number((uchar)netCard->getMac()->sa_data[3], 16).toUpper())
            .arg(QString::number((uchar)netCard->getMac()->sa_data[4], 16).toUpper())
            .arg(QString::number((uchar)netCard->getMac()->sa_data[5], 16).toUpper()));
}

/*
 * 当前的标签改变的时候，发送标签的标号
 */
void MacHeader::on_comboBox_currentIndexChanged(int index)
{
    emit macHeaderPro(index);
}
