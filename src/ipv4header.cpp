#include "ipv4header.h"
#include "ui_ipv4header.h"
#include <QDebug>

IPv4Header::IPv4Header(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IPv4Header)
{
    ui->setupUi(this);

}

IPv4Header::IPv4Header(char *buf, NetCard *netCard):
    QWidget(0),
    ui(new Ui::IPv4Header)
{
    ui->setupUi(this);

    this->buf = buf;

    this->netCard = netCard;

    ui->comboBox_pro->insertItem(0, tr("Users"));
    ui->comboBox_pro->insertItem(1, tr("Tcp"));
    ui->comboBox_pro->insertItem(2, tr("Udp"));
    ui->comboBox_pro->insertItem(3, tr("Icmp"));
    ui->comboBox_pro->insertItem(4, tr("Igmp"));
    ui->comboBox_pro->setCurrentIndex(0);

    setDefaultInfo();

    connect(netCard, SIGNAL(netChanged()), this, SLOT(setDefaultInfo()));
}

int IPv4Header::fillData(int index, int mtu)
{
    if (this->isHidden())
    {
        return 0;
    }

    bool ok;

    ipV4 = (struct ip *)(buf + index);

    //数据总长度和校验和先直接读取，之后进行计算
    ipV4->ip_v = ui->lineEdit_version->text().toUShort(&ok, 16);
    ipV4->ip_hl = ui->lineEdit_len->text().toUShort(&ok, 16);
    ipV4->ip_tos = ui->lineEdit_tos->text().toUShort(&ok, 16);
    ipV4->ip_len = htons(ui->lineEdit_len->text().toUShort(&ok, 16));
    ipV4->ip_id = htons(ui->lineEdit_flag->text().toUShort(&ok, 16));
    ipV4->ip_off = htons(ui->lineEdit_pianyi->text().toUShort(&ok, 16));
    ipV4->ip_ttl = (uchar)(ui->lineEdit_ttl->text().toUShort(&ok, 16));

    /*8位上层协议*/
    switch (ui->comboBox_pro->currentIndex())
    {
    //用户自定义
    case 0:
    {
        ipV4->ip_p = (uchar) (ui->comboBox_pro->currentText().toUShort(&ok, 16));
        break;
    }
        //tcp
    case 1:
    {
        ipV4->ip_p = IPPROTO_TCP;
        break;
    }
        //udp
    case 2:
    {
        ipV4->ip_p = IPPROTO_UDP;
        break;
    }
        //icmp
    case 3:
    {
        ipV4->ip_p = IPPROTO_ICMP;
        break;
    }
        //igmp
    case 4:
    {
        ipV4->ip_p = IPPROTO_IGMP;
        break;
    }
    default:
        break;

    }

    ipV4->ip_sum = htons(ui->lineEdit_Crc->text().toUShort(&ok, 16));

    QByteArray b = ui->lineEdit_srcIp->text().toLatin1();
    inet_aton(b.data(), &ipV4->ip_src);
    b = ui->lineEdit_dstIp->text().toLatin1();
    inet_aton(b.data(), &ipV4->ip_dst);

    //暂时不写选项字段，貌似没有定义

    return sizeof(sizeof(struct ip));
}

IPv4Header::~IPv4Header()
{
    delete ui;
}

void IPv4Header::setDefaultInfo()
{
    ui->lineEdit_version->setText(QString::number(IPVERSION, 16));
    ui->lineEdit_len->setText(QString::number(sizeof(struct ip) >> 2, 16));
    ui->lineEdit_tos->setText(QString("0"));
    //暂时这么设定
    ui->lineEdit_lenAll->setText(QString("ffff"));
    ui->lineEdit_flag->setText(QString("0"));
    ui->lineEdit_pianyi->setText(QString("0"));
    ui->lineEdit_ttl->setText(QString::number(MAXTTL, 16));
    //临时，应该添加一个函数进行调用
    ui->lineEdit_Crc->setText(QString("0"));
    ui->lineEdit_srcIp->setText(QString(QLatin1String(inet_ntoa(((struct sockaddr_in *)netCard->getIp())->sin_addr))));
    ui->lineEdit_dstIp->setText(QString("255.255.255.255"));
}

void IPv4Header::on_comboBox_pro_currentIndexChanged(int index)
{
    qDebug() << "yes set sucess";
    emit ipv4Pro(index + 3);
}
