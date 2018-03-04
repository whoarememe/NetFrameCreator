#ifndef CREATOR_H
#define CREATOR_H

#include <QWidget>

#include <sys/socket.h>
#include <stdlib.h>

#include "netcard.h"
#include "macheader.h"
#include "ipv4header.h"
#include "arpheader.h"
#include "tcpheader.h"
#include "udpheader.h"
#include "icmpheader.h"
#include "customdata.h"
#include "sendpacket.h"

namespace Ui {
class Creator;
}

class Creator : public QWidget
{
    Q_OBJECT

public:
    explicit Creator(QWidget *parent = 0);

    Creator(NetCard * netCard);

    ~Creator();

    //隐藏发送栏
    void hideSendFrame();

    //返回要发送的数据的大小
    //顺次构造，传入DataIntex,返回时判断是不是大于mtu
    int sendData();

private slots:

    void on_pushButton_send_clicked();

    /*设置是否允许发送*/
    void sendEnable(bool);

    void addWidget(int);

    /*信息初始化*/
    void initThis();

    //发送结束,执行这个函数
    void getSendInfo(int numSum, int sucessNum, int failNum);

    //进度条
    void sendProgress(int i);

signals:

private:
    Ui::Creator *ui;

    bool isSending = false; //判断当前是不是正在发送

    int dataLen;  //数据长度

    int sock;

    struct sockaddr_ll saddr_ll;    //网卡默认数据结构

    int mtu;    //获得当前网卡的mtu

    NetCard * netCard;  //获得默认网卡的信息

    char * buf; //使用malloc设置buf为mtu大小

    char * tmpBuf;  //当数据大于mtu时临时保存

    int num;    //发送数量

    int timeSpace;  //时间间隔

    bool access;    //是否允许发送

    int sendLen;    //要发送的数据的大小

    /*
     * 各个网络头文件
     *
     */
    MacHeader * macHeader;

    IPv4Header * ipv4Header;

    ArpHeader * arpHeader;

    TcpHeader * tcpHeader;

    UdpHeader * udpHeader;

    IcmpHeader * icmpHeader;

    CustomData * customData;

    SendPacket * sendPacket;
};

#endif // CREATOR_H
