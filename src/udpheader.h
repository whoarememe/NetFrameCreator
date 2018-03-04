#ifndef UDPHEADER_H
#define UDPHEADER_H

#include <QWidget>

#include <linux/udp.h>

#include "netcard.h"

namespace Ui {
class UdpHeader;
}

class UdpHeader : public QWidget
{
    Q_OBJECT

public:
    explicit UdpHeader(QWidget *parent = 0);

    UdpHeader(char * buf, NetCard * netCard);

    int fillData(int index, int mtu);

    ~UdpHeader();

public slots:
    /*设置默认显示信息*/
    void setDefaultInfo();

private:
    Ui::UdpHeader *ui;

    char * buf;

    NetCard * netCard;

    struct udphdr * udph;

};

#endif // UDPHEADER_H
