#include "statusinfo.h"
#include "ui_statusinfo.h"

#include <QNetworkInterface>
#include <QDebug>
#include <QFile>
#include <QCommandLineParser>

StatusInfo::StatusInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatusInfo)
{
    ui->setupUi(this);
}

StatusInfo::StatusInfo(NetCard *netCard):
    QWidget(0),
    ui(new Ui::StatusInfo)
{
    ui->setupUi(this);

    this->netCard = netCard;

    QFile ficmp("/proc/sys/net/ipv4/icmp_echo_ignore_all");
    QFile ftrans("/proc/sys/net/ipv4/ip_forward");

    if (!ficmp.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        icmpRes = false;
        qDebug() << "get icmp state error";
    }
    else
    {
        bool ok;

        QTextStream s(&ficmp);
        QString l = s.readLine();

        icmpRes = !l.toInt(&ok, 10);
    }

    if (!ftrans.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        trans = false;
        qDebug() << "get icmp state error";
    }
    else
    {
        bool ok;

        QTextStream s(&ftrans);
        QString l = s.readLine();

        icmpRes = l.toInt(&ok, 10);
    }

    /*如果正确获得权限那么添加网卡信息*/
    if (netCard->getRoot())
    {
        /*添加网卡名称选项*/
        for (int i = 0; i < netCard->getCardNum(); i++)
        {
            ui->ComboBox_netcard->insertItem(i, QString(QLatin1String(netCard->getCardName(i))));
        }

        /*设置当前网卡的默认值*/
        indexCard = ui->ComboBox_netcard->currentIndex();

        /*同时netCard的默认网卡也设置为同样的值*/
        netCard->setCardIndex(indexCard);

        showMessage();

        connect(this, SIGNAL(netCardIndex(int)), netCard, SLOT(sendSignalNetChange(int)));
    }

}

StatusInfo::~StatusInfo()
{
    delete ui;
}

void StatusInfo::showMessage()
{
    /*显示ipv4*/
    QString ipv4 = QString(QLatin1String(inet_ntoa(((struct sockaddr_in *)netCard->getIp())->sin_addr)));
    ui->LineEdit_ipv4->setText(ipv4);

    /*子网掩码*/
    QString netMask = QString(QLatin1String(inet_ntoa(((struct sockaddr_in *)netCard->getNetMask())->sin_addr)));
    ui->LineEdit_netmask->setText(netMask);

    /*硬件地址*/
    QString hwAddr = QString("%1:%2:%3:%4:%5:%6")
            .arg(QString::number((unsigned char)netCard->getMac()->sa_data[0], 16).toUpper())
            .arg(QString::number((unsigned char)netCard->getMac()->sa_data[1], 16).toUpper())
            .arg(QString::number((unsigned char)netCard->getMac()->sa_data[2], 16).toUpper())
            .arg(QString::number((unsigned char)netCard->getMac()->sa_data[3], 16).toUpper())
            .arg(QString::number((unsigned char)netCard->getMac()->sa_data[4], 16).toUpper())
            .arg(QString::number((unsigned char)netCard->getMac()->sa_data[5], 16).toUpper());
    ui->LineEdit_hardAddress->setText(hwAddr);

    /*广播地址*/
    QString broad = QString(QLatin1String(inet_ntoa(((struct sockaddr_in *)netCard->getBroad())->sin_addr)));
    ui->lineEdit_broadcast->setText(broad);

    /*mtu*/
    int mtu = netCard->getMtu();
    ui->LineEdit_mtu->setText(QString::number(mtu, 10));

    /*
     * 设置状态信息
     */
    ui->checkBox_openPromisc->setChecked(netCard->getIsPromisc());

    ui->checkBox_openIcmp->setChecked(icmpRes);

    ui->checkBox_openTrans->setChecked(trans);

}

void StatusInfo::on_ComboBox_netcard_currentIndexChanged(int index)
{
    indexCard = index;

    emit netCardIndex(index);

    showMessage();
}

void StatusInfo::setPacketNum(int num)
{
    ui->label_numPacRec->setText(QString::number(num, 10));
}

bool StatusInfo::getIcmpRes() const
{
    return icmpRes;
}

bool StatusInfo::getTrans() const
{
    return trans;
}

void StatusInfo::on_checkBox_openIcmp_toggled(bool checked)
{
    if (netCard->getRoot())
    {
        if (checked)
        {
            if (system("echo 0 > /proc/sys/net/ipv4/icmp_echo_ignore_all"))
            {
                icmpRes = true;
                ui->checkBox_openIcmp->setChecked(true);
            }
            else
            {
                return;
            }
        }
        else
        {
            if (system("echo 1 > /proc/sys/net/ipv4/icmp_echo_ignore_all"))
            {
                icmpRes = false;
                ui->checkBox_openIcmp->setChecked(false);
            }
            else
            {
                return;
            }
        }
    }
}

void StatusInfo::on_checkBox_openPromisc_toggled(bool checked)
{
    if (netCard->getRoot())
    {
        if (checked)
        {
            qDebug() << " i will set card promis";
//            if (netCard->setCardPromisc()) return;
//            else ui->checkBox_openPromisc->setChecked(false);
            ui->checkBox_openPromisc->setChecked(netCard->setCardPromisc());
        }
        else
        {
//            if (netCard->cancelCardPromisc()) return;
//            else ui->checkBox_openPromisc->setChecked(true);
            ui->checkBox_openPromisc->setChecked(netCard->cancelCardPromisc());
        }
    }

}

void StatusInfo::on_checkBox_openTrans_toggled(bool checked)
{
    if (netCard->getRoot())
    {
        if (checked)
        {
            if (system("echo 1 > /proc/sys/net/ipv4/ip_forward"))
            {
                trans = true;
                ui->checkBox_openTrans->setChecked(true);
            }
            else
            {
                return;
            }
        }
        else
        {
            if (system("echo 0 > /proc/sys/net/ipv4/ip_forward"))
            {
                trans = false;
                ui->checkBox_openTrans->setChecked(false);
            }
            else
            {
                return;
            }
        }
    }
}
