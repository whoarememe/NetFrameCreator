#include "recvmessage.h"

#include <QDebug>
#include <unistd.h>

RecvMessage::RecvMessage()
{

}

RecvMessage::~RecvMessage()
{
    shutdown(sock, SHUT_RDWR);
}

RecvMessage::RecvMessage(NetCard *netCard, InfoDisplay *infoDisplay, StatusInfo *statusInfo)
{
    /*进行注册*/
    qRegisterMetaType<QTextBlock> ("QTextBlock");
    qRegisterMetaType<QTextCursor> ("QTextCursor");

    this->netCard = netCard;

    this->infoDisplay = infoDisplay;

    this->statusInfo = statusInfo;

    memset(&saddr_ll, 0, sizeof(struct sockaddr_ll));
    saddr_ll.sll_family = PF_PACKET;

    if (!netCard->getRoot())
    {
        infoDisplay->showMessage(tr("请使用root用户登陆，否则可能不能使用某些功能"));
    }
    else
    {
        if (0 > (sock = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))))
        {
            infoDisplay->showMessage(tr("抱歉，出了一些错误，您可能无法查看收到的数据"));

            return;
        }

        initThis();

        /*当网卡更改时，显示*/
        connect(netCard, SIGNAL(netChanged()), this, SLOT(initThis()));
        /*数据报数量显示*/
        connect(this, SIGNAL(pactNumChange(int)), statusInfo, SLOT(setPacketNum(int)));
    }
}

RecvMessage::RecvMessage(NetCard *netCard, Creator *creator)
{
    this->netCard = netCard;

    this->creator = creator;

}

void RecvMessage::initThis()
{
    /*更换网卡*/
    saddr_ll.sll_ifindex = netCard->getIndex();
    /*进行绑定*/
    bind(sock, (struct sockaddr *)&saddr_ll, sizeof(struct sockaddr_ll));

    qDebug() << " i am set title";
    infoDisplay->setTitle(QString(QLatin1String(netCard->getCardName())));

    /*更换网卡时是否清空*/
    infoDisplay->clearMessage();
}

void RecvMessage::run()
{
    isStop = false;

    recvMessage();
}

void RecvMessage::recvMessage()
{
    while (1)
    {
        qDebug() << "i am recving";

        if (isStop)
        {
            QTime n = QTime::currentTime();
            QTime now;

            do {
                now = QTime::currentTime();
            } while(n.msecsTo(now) <= 5);

            break;
        }
        else
        {
            recvSize = recvfrom(sock, buf, MAX_LEN, 0, NULL, NULL);
            if (recvSize < 20) continue;
            else
            if (recvSize >= MAX_LEN) continue;
            else
            {
                analyseMessage(buf, recvSize);
                packetSum++;
                emit pactNumChange(packetSum);
            }
        }
    }

    isStop = true;
}

void RecvMessage::stop()
{
    isStop = true;
}

void RecvMessage::setFiltrate(QString str)
{
    filtrate = str;

    /*求解next数组*/
    int len = str.length();
    int i = 1;
    int j = 0;

    next[1] = 0;
    next[2] = 1;

    for (i = 3; i < len; i++)
    {
        j = i;
        for (; j > 2; j--)
        {
            if (filtrate.at(next[j-1]) == filtrate.at(j-1))
            {
                next[j] = next[j-1] + 1;
            }
            else
            {
                next[j] = 1;
            }
        }
    }


}

void RecvMessage::analyseMessage(char *bufPac, int size)
{
    eth_h = (struct ether_header *)bufPac;

    qDebug() << "i am in analy";
    if (filtrate == "" | filtrate == NULL)
    {
        showPacket(bufPac, size);
    }
    else
    {
        showByFiltrate(bufPac, size);
    }


}

void RecvMessage::showByFiltrate(char *bufPac, int size)
{

}

/*传入的都现实，就怕你不传入*/
void RecvMessage::showPacket(char *bufPac, int size)
{
    dstMac = QString("%1:%2:%3:%4:%5:%6")
            .arg(QString::number((unsigned char)eth_h->ether_dhost[0], 16).toUpper())
            .arg(QString::number((unsigned char)eth_h->ether_dhost[1], 16).toUpper())
            .arg(QString::number((unsigned char)eth_h->ether_dhost[2], 16).toUpper())
            .arg(QString::number((unsigned char)eth_h->ether_dhost[3], 16).toUpper())
            .arg(QString::number((unsigned char)eth_h->ether_dhost[4], 16).toUpper())
            .arg(QString::number((unsigned char)eth_h->ether_dhost[5], 16).toUpper());
    srcMac = QString("%1:%2:%3:%4:%5:%6")
            .arg(QString::number((unsigned char)eth_h->ether_shost[0], 16).toUpper())
            .arg(QString::number((unsigned char)eth_h->ether_shost[1], 16).toUpper())
            .arg(QString::number((unsigned char)eth_h->ether_shost[2], 16).toUpper())
            .arg(QString::number((unsigned char)eth_h->ether_shost[3], 16).toUpper())
            .arg(QString::number((unsigned char)eth_h->ether_shost[4], 16).toUpper())
            .arg(QString::number((unsigned char)eth_h->ether_shost[5], 16).toUpper());

    /*协议判断*/
    switch (ntohs(eth_h->ether_type)) {
    case 0x0800:
        macPro = tr("Ip");
        break;

    case 0x0806:
        macPro = tr("Arp");
        break;

    case 0x8035:
        macPro = tr("Rarp");
        break;

    default:
        break;
    }

    infoDisplay->showMessage(dstMac + "   ===>   " + srcMac + "   " + "Proto: " + macPro);
}
