#include "netcard.h"
#include <QDebug>
#include <QErrorMessage>
#include <QFile>

NetCard::NetCard()
{
    QErrorMessage * dialog = new QErrorMessage;
    dialog->setWindowTitle("错误信息提示！");

    /*初始化ifconf的长度*/
    ifc.ifc_len = sizeof(ifrBuff);

    /*设置缓冲区*/
    ifc.ifc_buf = (char *)ifrBuff;

    if (0 > (sock = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL)))) {

        dialog->setModal(true);
        dialog->showMessage("套接字创建失败，请确定您是否使用root登陆");

        /*权限不够*/
        root = 0;

        return;
    }

    /*获取所有接口的配置信息，将其保存至ifc中*/
    if (0 > (ioctl(sock, SIOCGIFCONF, (char *)&ifc))) {

        return;
    }

    /*计算网卡的数量*/
    cardNum = ifc.ifc_len / sizeof(struct ifreq);

    /*设置当前网卡索引为第一个网卡*/
    cardIndex = 0;

    root = 1;
}

NetCard::~NetCard()
{
    shutdown(sock, SHUT_RDWR);
}

/*
 * 获得网卡数量
 */
int NetCard::getCardNum()
{
    return this->cardNum;

}

/*
 * 获得当前默认网卡名称
 *
 */
char *NetCard::getCardName()
{
    return ifrBuff[cardIndex].ifr_name;

}

/*
 * 获得指定网卡名称
 *
 */
char *NetCard::getCardName(int i)
{
    if (i > cardNum)
    {
        return NULL;
    }

    return ifrBuff[i].ifr_name;

}

/*
 * 获得网卡的ip地址
 * 返回struct sockaddr类型
 */
sockaddr *NetCard::getIp()
{
    if (0 == ioctl(sock, SIOCGIFADDR, (char *)&ifrBuff[cardIndex]))
    {
        qDebug() << "get ip sucess";
        return &ifrBuff[cardIndex].ifr_addr;
    }
}

sockaddr *NetCard::getIp(int i)
{
    if (i >= cardNum)
    {
        return NULL;
    }

    if (0 == ioctl(sock, SIOCGIFADDR, (char *)&ifrBuff[i]))
    {
        qDebug() << "get ip sucess";
        return &ifrBuff[i].ifr_addr;
    }
}

/*
 * 获得网卡的mac地址
 * 返回struct sockaddr类型
 */
sockaddr *NetCard::getMac()
{
    if (0 == ioctl(sock, SIOCGIFHWADDR, (char *)&ifrBuff[cardIndex]))
    {
        qDebug() << "get mac sucess";
        return &ifrBuff[cardIndex].ifr_hwaddr;
    }
}

sockaddr *NetCard::getMac(int i)
{
    if (i >= cardNum)
    {
        return NULL;
    }

    if (0 == ioctl(sock, SIOCGIFHWADDR, (char *)&ifrBuff[i]))
    {
        qDebug() << "get mac sucess";
        return &ifrBuff[i].ifr_hwaddr;
    }
}

/*
 * 获得网卡的子网掩码
 * 返回struct sockaddr类型
 */
sockaddr *NetCard::getNetMask()
{
    if (0 == ioctl(sock, SIOCGIFNETMASK, (char *)&ifrBuff[cardIndex]))
    {
        qDebug() << "get netmask sucess";
        return &ifrBuff[cardIndex].ifr_netmask;
    }
}

sockaddr *NetCard::getNetMask(int i)
{
    if (i >= cardNum)
    {
        return NULL;
    }

    if (0 == ioctl(sock, SIOCGIFNETMASK, (char *)&ifrBuff[i]))
    {
        qDebug() << "get netmask sucess";
        return &ifrBuff[i].ifr_netmask;
    }
}

/*
 * 获得网卡的广播地址
 * 返回struct sockaddr类型
 */
sockaddr * NetCard::getBroad()
{
    if (0 == ioctl(sock, SIOCGIFBRDADDR, (char *)&ifrBuff[cardIndex]))
    {
        qDebug() << "get broad sucess";
        return &ifrBuff[cardIndex].ifr_broadaddr;
    }
}

sockaddr *NetCard::getBroad(int i)
{
    if (i > cardNum)
    {
        return NULL;
    }

    if (0 == ioctl(sock, SIOCGIFBRDADDR, (char *)&ifrBuff[i]))
    {
        qDebug() << "get broad sucess";
        return &ifrBuff[i].ifr_broadaddr;
    }
}

/*
 * 获得网卡的mtu大小
 * 返回int类型
 */
int NetCard::getMtu()
{
    if (0 == ioctl(sock, SIOCGIFMTU, (char *)&ifrBuff[cardIndex]))
    {
        qDebug() << "get mtu sucess";
        return ifrBuff[cardIndex].ifr_mtu;
    }
}

int NetCard::getMtu(int i)
{
    if (i >= cardNum)
    {
        return 0;
    }

    if (0 == ioctl(sock, SIOCGIFMTU, (char *)&ifrBuff[i]))
    {
        qDebug() << "get mtu sucess";
        return ifrBuff[i].ifr_mtu;
    }
}

/*
 * 获得接口状态
 * 返回无符号短整型
 */
short int NetCard::getFlag()
{
    if (0 == ioctl(sock, SIOCGIFFLAGS, (char *)&ifrBuff[cardIndex]))
    {
        qDebug() << "get flag sucess";
        return ifrBuff[cardIndex].ifr_flags;
    }
}

short NetCard::getFlag(int i)
{
    if (i >= cardNum)
    {
        return 0;
    }

    if (0 == ioctl(sock, SIOCGIFFLAGS, (char *)&ifrBuff[i]))
    {
        qDebug() << "get flag sucess";
        return ifrBuff[i].ifr_flags;
    }
}

/*
 * 获得接口索引
 * 返回无符号短整型
 */
int NetCard::getIndex()
{
    if (0 == ioctl(sock, SIOCGIFINDEX, (char *)&ifrBuff[cardIndex]))
    {
        qDebug() << "get index sucess";
        return ifrBuff[cardIndex].ifr_ifindex;
    }
}

int NetCard::getIndex(int i)
{
    if (i >= cardNum)
    {
        return -1;
    }

    if (0 == ioctl(sock, SIOCGIFINDEX, (char *)&ifrBuff[i]))
    {
        qDebug() << "get index sucess";
        return ifrBuff[i].ifr_ifindex;
    }
}

int NetCard::getCardIndex() const
{
    return cardIndex;
}

void NetCard::setCardIndex(int value)
{
    /*看看是不是设置的当前默认网卡是越界的*/
    if (value >= cardNum)
    {
        return;
    }

    cardIndex = value;
}

/*
 * 设置套接字
 */
void NetCard::setSock(int sock)
{
    this->sock = sock;
}

bool NetCard::setCardPromisc()
{
    /*获得当前网卡的标志*/
    this->getFlag();

    /*如果连接着*/
    if (ifrBuff[cardIndex].ifr_flags & IFF_RUNNING)
    {

    }
    /*设置为混杂模式*/
    /*如果不是混杂*/
    if (!(ifrBuff[cardIndex].ifr_flags & IFF_PROMISC))
    {
        ifrBuff[cardIndex].ifr_flags |= IFF_PROMISC;

        if ((ioctl(sock, SIOCSIFFLAGS, &ifrBuff[cardIndex])) < 0)
        {
            isPromisc = false;
            return false;
        }
        else
        {
            isPromisc = true;
            return true;
        }
    }
    /*现在正处于混杂*/
    else
    {
        isPromisc = true;
        return true;
    }
}

bool NetCard::cancelCardPromisc()
{
    /*获得当前网卡的标志*/
    this->getFlag();

    /*如果开启了网卡*/
    if (ifrBuff[cardIndex].ifr_flags & IFF_RUNNING)
    {

    }

    //先关掉网卡

    //如果处于混杂模式
    if ((ifrBuff[cardIndex].ifr_flags & IFF_PROMISC))
    {
        //标志位进行设定
        ifrBuff[cardIndex].ifr_flags &= 0xfeff;

        if ((ioctl(sock, SIOCSIFFLAGS, &ifrBuff[cardIndex])) < 0)
        {
            ifrBuff[cardIndex].ifr_flags |= IFF_UP;
            if (ioctl(sock, SIOCSIFFLAGS, &ifrBuff[cardIndex]) < 0)
                qDebug() << "网卡重启失败";

            isPromisc = true;
            return false;
        }
        else
        {
            ifrBuff[cardIndex].ifr_flags |= IFF_UP;
            if (ioctl(sock, SIOCSIFFLAGS, &ifrBuff[cardIndex]) < 0)
                qDebug() << "网卡重启失败";

            isPromisc = false;
            return true;
        }
    }
    /*现在未处于混杂*/
    else
    {
        isPromisc = false;
        return true;
    }

}

bool NetCard::getIsPromisc()
{
    this->getFlag();

    if (ifrBuff[cardIndex].ifr_flags & IFF_PROMISC)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void NetCard::sendSignalNetChange(int i)
{
    qDebug() << "i am sending signal netChanged";
    cardIndex = i;

    emit netChanged();
}

int NetCard::getRoot() const
{
    return root;
}

/*
 * 获得网关，暂时是读取/proc/net/route文件
 * 暂时先不获取了
 */
QString NetCard::getGateWay()
{
    QFile f("/proc/net/route");

    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "get route error";

        return NULL;
    }

    QTextStream t(&f);

}

/*
 * 得到套接字
 */
int NetCard::getSock()
{
    return this->sock;
}

