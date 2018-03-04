#ifndef ARPHEADER_H
#define ARPHEADER_H

#include <QWidget>

//arp数据
#include <netinet/if_ether.h>
//arp头部
#include <net/if_arp.h>

#include "netcard.h"

namespace Ui {
class ArpHeader;
}

class ArpHeader : public QWidget
{
    Q_OBJECT

public:
    explicit ArpHeader(QWidget *parent = 0);

    ArpHeader(char * buf, NetCard * netCard);

    int fillData(int index, int mtu);

    ~ArpHeader();

    void setIdentity(int value);

public slots:
    /*设置默认显示信息*/
    void setDefaultInfo();

private:
    Ui::ArpHeader *ui;

    char * buf;

    NetCard * netCard;

    struct ether_arp * eth_arp;
    struct arphdr   * arph;

    int identity;   //判断是arp0还是rarp1
};

#endif // ARPHEADER_H
