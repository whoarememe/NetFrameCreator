#ifndef TCPHEADER_H
#define TCPHEADER_H

#include <QWidget>

#include <linux/tcp.h>

#include "netcard.h"

namespace Ui {
class TcpHeader;
}

struct flag {
    unsigned int urg:1;
    unsigned int ack:1;
    unsigned int psh:1;
    unsigned int rst:1;
    unsigned int syn:1;
    unsigned int fin:1;
};

class TcpHeader : public QWidget
{
    Q_OBJECT

public:
    explicit TcpHeader(QWidget *parent = 0);

    TcpHeader(char * buf, NetCard * netCard);

    int fillData(int index, int mtu);

    /*Crc校验计算*/
    unsigned short checkSum(unsigned short *addr,int len);

    ~TcpHeader();

public slots:
    /*设置默认显示信息*/
    void setDefaultInfo();

private slots:
    void on_checkBox_urg_toggled(bool checked);

    void on_checkBox_ack_toggled(bool checked);

    void on_checkBox_psh_toggled(bool checked);

    void on_checkBox_rst_toggled(bool checked);

    void on_checkBox_syn_toggled(bool checked);

    void on_checkBox_fin_toggled(bool checked);

private:
    Ui::TcpHeader *ui;

    char * buf;

    NetCard * netCard;

    struct tcphdr * tcph;

    struct flag f;

};

#endif // TCPHEADER_H
