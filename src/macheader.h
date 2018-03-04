#ifndef MACHEADER_H
#define MACHEADER_H

#include <QWidget>

#include "netcard.h"
#include "netpacket/packet.h"
/*以太网首部*/
#include "net/ethernet.h"

namespace Ui {
class MacHeader;
}

class MacHeader : public QWidget
{
    Q_OBJECT

public:
    explicit MacHeader(QWidget *parent = 0);

    MacHeader(char * buf, NetCard * netCard, struct sockaddr_ll * saddr_ll);

    int fillData(int index, int mtu);

    ~MacHeader();

public slots:
    /*设置默认显示信息*/
    void setDefaultInfo();

private slots:
    void on_comboBox_currentIndexChanged(int index);

signals:
    void macHeaderPro(int);

private:
    Ui::MacHeader *ui;

    char * buf;

    NetCard * netCard;

    /*定义默认的目的地址*/
    uchar dstAddr[6] = {0x12, 0x34, 0x56, 0x78, 0x90, 0xff};
    uchar srcAddr[6] = {0x22, 0x22, 0x33, 0x44, 0x55, 0xff};   //来源地址

    struct ether_header * eth_h;  //定义以太网头部

    struct sockaddr_ll * saddr_ll;  //以太网的目的地址由他确定，类似sockaddr_in的目的ip
};

#endif // MACHEADER_H
