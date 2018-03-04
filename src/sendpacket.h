#ifndef SENDPACKET_H
#define SENDPACKET_H

#include <QtCore>
#include <QThread>

#include <netpacket/packet.h>
#include <sys/types.h>
#include <sys/socket.h>

class SendPacket : public QThread
{
    Q_OBJECT

public:
    SendPacket();

    char *getBuf() const;
    void setBuf(char *value);

    int getNum() const;
    void setNum(int value);

    int getTimeSpace() const;
    void setTimeSpace(int value);

    int getSendLen() const;
    void setSendLen(int value);

    sockaddr_ll *getSaddr_ll() const;
    void setSaddr_ll(sockaddr_ll *value);

    int getSock() const;
    void setSock(int value);

public slots:
    void send();

    void stop();

protected:
    void run();

signals:
    //总的发送数量，成功数量，失败数量
    void sendFinish(int, int, int);

    //发送进度
    void sendProcsee(int);

private:
    //待发送数据
    char * buf;
    int numSum;     //实际发送的数量
    int sucessNum;  //成功的数量
    int failNum;    //失败数量
    int num;    //总数
    int timeSpace;  //时间间隔
    int sendLen;    //待发送数据长度
    int sucLen;     //实际发送的长度
    int sock;       //套接字

    struct sockaddr_ll * saddr_ll;
    bool stopped;
};

#endif // SENDPACKET_H
