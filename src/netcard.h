#ifndef NETCARD_H
#define NETCARD_H

#include <QtCore>
#include <QString>

#include <net/if.h>
#include <sys/ioctl.h>
#include <netinet/ip.h>
#include <string.h>
#include <linux/if_ether.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

typedef unsigned char uchar;

#define MAC_ADDR_LEN 6
#define IP_ADDR_LEN 4
#define NETMASK_ADDR_LEN 4
#define BROAD_ADDR_LEN 4

class NetCard: public QObject
{
    Q_OBJECT

public:
    NetCard();

    ~NetCard();

    /*获得接口数量*/
    int getCardNum();

    /*获得网卡名称*/
    char * getCardName();

    /*获得指定网卡的名称*/
    char * getCardName(int i); 

    /*获得ip*/
    struct sockaddr * getIp();

    /*获得指定网卡的ip*/
    struct sockaddr * getIp(int i);

    /*获得网卡mac地址*/
    struct sockaddr * getMac();

    /*获得指定网卡mac地址*/
    struct sockaddr * getMac(int i);

    /*获得网卡子网掩码*/
    struct sockaddr * getNetMask();

    /*获得指定网卡子网掩码*/
    struct sockaddr * getNetMask(int i);

    /*获得网卡广播地址*/
    struct sockaddr * getBroad();

    /*获得指定网卡广播地址*/
    struct sockaddr * getBroad(int i);

    /*获得网卡MTU*/
    int getMtu();

    /*获得指定网卡MTU*/
    int getMtu(int i);

    /*获得网卡状态*/
    short int getFlag();

    /*获得指定网卡状态*/
    short int getFlag(int i);

    /*获得网卡接口的索引*/
    int getIndex();

    /*获得指定网卡接口的索引*/
    int getIndex(int i);

    /*获得当前默认网卡索引*/
    int getCardIndex() const;

    /*判断是不是root*/
    int getRoot() const;

    /*获得默认网关地址,暂时通过读文件，不通过内核态*/
    QString getGateWay();

    /*获得套接字*/
    int getSock();

    //设置
    /*设置当前默认网卡*/
    void setCardIndex(int value);

    /*设置套接字*/
    void setSock(int sock);

    /*设置当前网卡为混杂模式*/
    bool setCardPromisc();

    /*取消当前网卡的混杂模式*/
    bool cancelCardPromisc();

    bool getIsPromisc();

signals:
    void netChanged();

public slots:
    /*网卡改变了*/
    void sendSignalNetChange(int i);

private:

    bool isPromisc; //当前网卡是否处于混杂模式

    int root;   //

    int cardIndex;  //默认网卡索引

    int cardNum;    //网卡数量

    int sock;

    struct ifconf ifc;    //网卡信息

    struct ifreq ifrBuff[5];   //保存所有网卡信息，默认网卡最多为5个

};

#endif // NETCARD_H
