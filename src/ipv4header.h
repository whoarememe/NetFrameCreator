#ifndef IPV4HEADER_H
#define IPV4HEADER_H

#include <QWidget>

#include <netinet/ip.h>

#include "netcard.h"

namespace Ui {
class IPv4Header;
}

class IPv4Header : public QWidget
{
    Q_OBJECT

public:
    explicit IPv4Header(QWidget *parent = 0);

    IPv4Header(char * buf, NetCard * netCard);

    int fillData(int index, int mtu);

    ~IPv4Header();

public slots:
    /*设置默认显示信息*/
    void setDefaultInfo();

signals:
    void ipv4Pro(int);

private slots:
    void on_comboBox_pro_currentIndexChanged(int index);

private:
    Ui::IPv4Header *ui;

    char * buf;

    NetCard * netCard;

    struct ip * ipV4;
};

#endif // IPV4HEADER_H
