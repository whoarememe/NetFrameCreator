#include "sendpacket.h"
#include <QDebug>
#include <QTime>

SendPacket::SendPacket()
{

}

char *SendPacket::getBuf() const
{
    return buf;
}

void SendPacket::setBuf(char *value)
{
    buf = value;
}

int SendPacket::getNum() const
{
    return num;
}

void SendPacket::setNum(int value)
{
    num = value;
}

int SendPacket::getTimeSpace() const
{
    return timeSpace;
}

void SendPacket::setTimeSpace(int value)
{
    timeSpace = value;
}

int SendPacket::getSendLen() const
{
    return sendLen;
}

void SendPacket::setSendLen(int value)
{
    sendLen = value;
}

sockaddr_ll *SendPacket::getSaddr_ll() const
{
    return saddr_ll;
}

void SendPacket::setSaddr_ll(sockaddr_ll *value)
{
    saddr_ll = value;
}

void SendPacket::stop()
{
    stopped = true;

    emit sendFinish(numSum, sucessNum, failNum);
}

void SendPacket::send()
{
    numSum = 0;
    sucessNum = 0;
    failNum = 0;

    while (!stopped && numSum < num)
    {
        int len = sendto(sock, buf, 128, 0, (struct sockaddr *)saddr_ll, sizeof(struct sockaddr_ll));
        if (len < 0)
        {
            failNum++;
            qDebug() << "send error";
        }
        else
        {
            sucessNum++;
            qDebug() << "send ok";
        }

        numSum++;
        emit sendProcsee(numSum);

        QTime n = QTime::currentTime();
        QTime now;
        //毫秒计时
        do {
            now = QTime::currentTime();
        } while(n.msecsTo(now) <= timeSpace);
    }

    stop();
}

void SendPacket::run()
{
    qDebug() << " i am send run func";
    stopped = false;
    while (!stopped) send();
}

int SendPacket::getSock() const
{
    return sock;
}

void SendPacket::setSock(int value)
{
    sock = value;
}
