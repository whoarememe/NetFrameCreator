#ifndef RECVMESSAGE_H
#define RECVMESSAGE_H

#include <QtCore>
#include <QMetaType>
#include <QTextBlock>
#include <QTextCursor>

#include <netpacket/packet.h>
#include <netinet/if_ether.h>

#include "netcard.h"
#include "creator.h"
#include "infodisplay.h"
#include "statusinfo.h"

#define MAX_LEN 4096

class RecvMessage: public QThread
{
    Q_OBJECT

public:
    RecvMessage();

    ~RecvMessage();

    RecvMessage(NetCard * netCard, InfoDisplay * infoDisplay, StatusInfo * statusInfo);

    RecvMessage(NetCard *netCard, Creator * creator);

    /*接收信息，仅仅是接收*/
    void recvMessage();

    /*对收到的包进行分析,顺便过滤也可以在这里使用*/
    /*之后换成listwidget进行展示，以后会将字符串传送给listwidget或者数据全部传输给listwidget*/
    void analyseMessage(char * bufPac, int size);

    /*只用来显示包*/
    void showPacket(char *bufPac, int size);

    void showByFiltrate(char *bufPac, int size);

public slots:
    /*进行一些初始化设置*/
    void initThis();

    /*停止接收信息*/
    void stop();

    /*过滤字符串设置*/
    void setFiltrate(QString str);

signals:
    /*数据报数量改变*/
    void pactNumChange(int);

protected:
    /*执行线程*/
    void run();

private:
    int sock;

    struct ifreq ifr;

    struct sockaddr_ll saddr_ll;    //用来选定网卡

    bool isStop;  //判断是不是暂停了,一开始默认是暂停的

    char buf[MAX_LEN]; //一般不会超过这个数值的

    int recvSize;  //记录接收的包的大小

    int packetSum = 0;  //记录包的总数

    /*各种数据报头*/
    struct ether_header * eth_h;
    struct ether_arp * eth_arp;
    struct arphdr * arpHdr;
    //...未完

    NetCard * netCard;

    Creator * creator;

    InfoDisplay * infoDisplay;

    StatusInfo * statusInfo;

    QString filtrate;   //过滤字符串
    int next[64];

    /*暂时保留*/
    QString dstMac;
    QString srcMac;
    QString macPro;
};

#endif // RECVMESSAGE_H
