#include "creator.h"
#include "ui_creator.h"

#include <QList>
#include <QDebug>

Creator::Creator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Creator)
{
    ui->setupUi(this);

    ipv4Header = new IPv4Header();
    arpHeader = new ArpHeader();
}

Creator::Creator(NetCard *netCard):
    QWidget(0),
    ui(new Ui::Creator)
{
    ui->setupUi(this);

    /*隐藏进度条*/
    ui->widget_process->hide();

    this->netCard = netCard;

    /*不允许发送*/
    access = false;
    sendEnable(access);
    /*时间设置也不可用*/
    ui->spinBox_num->setEnabled(false);
    ui->spinBox_time->setEnabled(false);

    /*
     *
     * 判断是不是root权限
     *
     */
    if (netCard->getRoot())
    {

        qDebug() << "i am in creator gouzao";
        /*这里使用新建立的套接字，不是使用netCard中的套接字*/
        if (0 > (sock = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))))
        {
            qDebug() << "在creator中创建套接字失败";

            /*不允许发送*/
            access = false;
            sendEnable(access);

            return;
        }

        mtu = netCard->getMtu();
        buf = (char *)malloc(mtu);
        tmpBuf = (char *)malloc(mtu);
        memset(buf, 0, mtu);
        memset(tmpBuf, 0, mtu);

        sendPacket = new SendPacket;

        /*设置各个窗口*/
        macHeader = new MacHeader(buf, netCard, &saddr_ll);
        ipv4Header = new IPv4Header(buf, netCard);
        arpHeader = new ArpHeader(buf, netCard);
        tcpHeader = new TcpHeader(buf, netCard);
        udpHeader = new UdpHeader(buf, netCard);
        icmpHeader = new IcmpHeader(buf, netCard);
        customData = new CustomData(buf, tmpBuf);

        /*添加mac头部和数据段*/
        ui->verticalLayout_data->insertWidget(0, macHeader);
        ui->verticalLayout_data->insertWidget(4, customData);

        /*允许发送*/
        access = true;
        sendEnable(access);
        ui->spinBox_num->setEnabled(true);
        ui->spinBox_time->setEnabled(true);

        /*
         * 初始化网卡的变量
         */
        initThis();

        /*
         * 连接信号和槽
         */
        connect(macHeader, SIGNAL(macHeaderPro(int)), this, SLOT(addWidget(int)));
        connect(ipv4Header, SIGNAL(ipv4Pro(int)), this, SLOT(addWidget(int)));
        connect(netCard, SIGNAL(netChanged()), this, SLOT(initThis()));
        //发送结束
        connect(sendPacket, SIGNAL(sendFinish(int,int,int)), this, SLOT(getSendInfo(int,int,int)));
        //发送进度
        connect(sendPacket, SIGNAL(sendProcsee(int)), this, SLOT(sendProgress(int)));
    }
}

Creator::~Creator()
{
    delete netCard;
    delete buf;
    delete tmpBuf;
    shutdown(sock, SHUT_RDWR);

    delete ui;
}

//隐藏发送栏，用于信息接收
void Creator::hideSendFrame()
{
    ui->frame_send->hide();
}

/*
 *  数据发送
 */
int Creator::sendData()
{
    int index = 0;
    int len = 0;

    /*
     * 从第一层依次填充数据，传入mtu和index，返回填充的数据大小
     */
    len = macHeader->fillData(index, mtu);
    index += len;

    len = arpHeader->fillData(index, mtu);
    index += len;

    len = ipv4Header->fillData(index, mtu);
    index += len;

    len = tcpHeader->fillData(index, mtu);
    index += len;

    len = udpHeader->fillData(index, mtu);
    index += len;

    len = customData->fillData(index, mtu);
    index += len;

    if (index > mtu)
    {
        return mtu;
    }
    else
    {
        return index;
    }
}

/*
 * 初始化一些信息，当网卡改变的时候也初始化
 */
void Creator::initThis()
{
    /*重新设定硬件地址*/
//    memset(&saddr_ll, 0, sizeof(struct sockaddr_ll));
//    saddr_ll.sll_family = PF_PACKET;
    saddr_ll.sll_ifindex = netCard->getIndex();
    /*重新绑定网卡*/
    bind(sock, (struct sockaddr *)&saddr_ll, sizeof(struct sockaddr_ll));

    mtu = netCard->getMtu();

    /*重新分配空间要发送的内存空间为mtu的大小*/
    memset(buf, 0, mtu);
    memset(tmpBuf, 0, mtu);
}

void Creator::getSendInfo(int numSum, int sucessNum, int failNum)
{
    isSending = false;
    ui->pushButton_send->setText(tr("发送"));

//    ui->progressBar->hide();
//    QLabel * l = new QLabel(0);

//    l->setText(QString("一共发送数据报：%1 个，成功：%2 个，失败：%3 个")
//                       .arg(numSum)
//                       .arg(sucessNum)
//                       .arg(failNum));
//    l->show();
//    QDateTime n=QDateTime::currentDateTime();
//    QDateTime now;
//    do{
//        now=QDateTime::currentDateTime();
//    } while (n.secsTo(now)<=3);

    ui->widget_process->hide();
}

/*
 * 发送进度
 */
void Creator::sendProgress(int i)
{
    ui->progressBar->setValue(i);
}

/*
 * 只负责数据的发送
 */
void Creator::on_pushButton_send_clicked()
{
    if (isSending)
    {
        ui->pushButton_send->setText(tr("发送"));
        sendPacket->stop();
        isSending = false;
    }
    else
    {
        dataLen = sendData();
        if (dataLen < 60)
        {
            dataLen = 60;
        }

        ui->pushButton_send->setText(tr("结束"));
        num = ui->spinBox_num->value();
        timeSpace = ui->spinBox_time->value();

        ui->widget_process->show();
//        ui->progressBar->show();
        ui->progressBar->setRange(0, num);

        sendPacket->setSock(sock);
        sendPacket->setBuf(buf);
        sendPacket->setNum(num);
        sendPacket->setTimeSpace(timeSpace);
        sendPacket->setSendLen(dataLen);    //暂时先使用mtu
        sendPacket->setSaddr_ll(&saddr_ll);

        sendPacket->start();
        isSending = true;
    }

}

/*
 * 设置当前是否允许发送数据
 */
void Creator::sendEnable(bool b)
{
    ui->pushButton_send->setEnabled(b);
}

void Creator::addWidget(int i)
{
//    if (sender() == macHeader)
//    {
        switch (i) {
        case 0:
        {
            ui->verticalLayout_data->removeWidget(ipv4Header);
            ui->verticalLayout_data->removeWidget(arpHeader);
            ui->verticalLayout_data->removeWidget(tcpHeader);
            ui->verticalLayout_data->removeWidget(udpHeader);
            ui->verticalLayout_data->removeWidget(icmpHeader);
            ipv4Header->hide();
            arpHeader->hide();
            tcpHeader->hide();
            udpHeader->hide();
            icmpHeader->hide();

            if (ui->verticalLayout_data->indexOf(customData) < 0)
            {
                ui->verticalLayout_data->insertWidget(4, customData);
            }


            break;
        }
            /*层1*/
        case 1:
        {
            ui->verticalLayout_data->removeWidget(ipv4Header);
            ui->verticalLayout_data->removeWidget(arpHeader);
            ui->verticalLayout_data->removeWidget(tcpHeader);
            ui->verticalLayout_data->removeWidget(udpHeader);
            ui->verticalLayout_data->removeWidget(icmpHeader);
            ipv4Header->hide();
            tcpHeader->hide();
            udpHeader->hide();
            icmpHeader->hide();

            ui->verticalLayout_data->insertWidget(1, arpHeader);

            arpHeader->setIdentity(0);
            arpHeader->setDefaultInfo();
            arpHeader->show();

            break;
        }
        case 2:
        {
            ui->verticalLayout_data->removeWidget(ipv4Header);
            ui->verticalLayout_data->removeWidget(arpHeader);
            ui->verticalLayout_data->removeWidget(tcpHeader);
            ui->verticalLayout_data->removeWidget(udpHeader);
            ui->verticalLayout_data->removeWidget(icmpHeader);
            ipv4Header->hide();
            tcpHeader->hide();
            udpHeader->hide();
            icmpHeader->hide();

            ui->verticalLayout_data->insertWidget(1, arpHeader);

            arpHeader->setIdentity(1);
            arpHeader->setDefaultInfo();
            arpHeader->show();

            break;
        }
        case 3:
        {
            ui->verticalLayout_data->removeWidget(ipv4Header);
            ui->verticalLayout_data->removeWidget(arpHeader);
            ui->verticalLayout_data->removeWidget(tcpHeader);
            ui->verticalLayout_data->removeWidget(udpHeader);
            ui->verticalLayout_data->removeWidget(icmpHeader);
            arpHeader->hide();
            tcpHeader->hide();
            udpHeader->hide();
            icmpHeader->hide();

            ui->verticalLayout_data->insertWidget(1, ipv4Header);
            ipv4Header->show();

            break;
        }
            /*层2*/
        case 4:
        {
            ui->verticalLayout_data->removeWidget(ipv4Header);
            ui->verticalLayout_data->removeWidget(arpHeader);
            ui->verticalLayout_data->removeWidget(tcpHeader);
            ui->verticalLayout_data->removeWidget(udpHeader);
            ui->verticalLayout_data->removeWidget(icmpHeader);
            arpHeader->hide();
            udpHeader->hide();
            icmpHeader->hide();

            ui->verticalLayout_data->insertWidget(1, ipv4Header);
            ui->verticalLayout_data->insertWidget(2, tcpHeader);

            ipv4Header->show();
            tcpHeader->show();

            break;
        }
        case 5:
        {
            ui->verticalLayout_data->removeWidget(ipv4Header);
            ui->verticalLayout_data->removeWidget(arpHeader);
            ui->verticalLayout_data->removeWidget(tcpHeader);
            ui->verticalLayout_data->removeWidget(udpHeader);
            ui->verticalLayout_data->removeWidget(icmpHeader);
            arpHeader->hide();
            tcpHeader->hide();
            icmpHeader->hide();

            ui->verticalLayout_data->insertWidget(1, ipv4Header);
            ui->verticalLayout_data->insertWidget(2, udpHeader);

            ipv4Header->show();
            udpHeader->show();

            break;
        }
        case 6:
        {
            ui->verticalLayout_data->removeWidget(ipv4Header);
            ui->verticalLayout_data->removeWidget(arpHeader);
            ui->verticalLayout_data->removeWidget(tcpHeader);
            ui->verticalLayout_data->removeWidget(udpHeader);
            ui->verticalLayout_data->removeWidget(icmpHeader);
            arpHeader->hide();
            udpHeader->hide();
            tcpHeader->hide();

            ui->verticalLayout_data->insertWidget(1, ipv4Header);
            ui->verticalLayout_data->insertWidget(2, icmpHeader);

            ipv4Header->show();
            icmpHeader->show();

            break;
        }
        case 7:
            break;
        default:
            break;
        }
//    }
}
