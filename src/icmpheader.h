#ifndef ICMPHEADER_H
#define ICMPHEADER_H

#include <QWidget>

#include "netcard.h"

namespace Ui {
class IcmpHeader;
}

class IcmpHeader : public QWidget
{
    Q_OBJECT

public:
    explicit IcmpHeader(QWidget *parent = 0);

    IcmpHeader(char * buf, NetCard * netCard);

    ~IcmpHeader();

private:
    Ui::IcmpHeader *ui;

    char * buf;

    NetCard * netCard;
};

#endif // ICMPHEADER_H
