#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>

#include "creator.h"
#include "statusinfo.h"
#include "infodisplay.h"
#include "recvmessage.h"
#include "netcard.h"
#include "tools.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

private slots:
    void on_action_exit_triggered();

    void startPacket(bool);

private:
    Ui::MainWindow *ui;

    StatusInfo * statusInfo;    //状态信息显示栏

    //主功能
    QWidget * creatorWidget;    //信息发送窗口
    QVBoxLayout * creatorVBox;
    InfoDisplay * infoDisplay;  //信息显示栏
    Creator * creatorSend;  //信息发送栏

    //响应功能
    QWidget * responseWidget;   //信息发送窗口
    QVBoxLayout * responseVBox;
    Creator * responseAccept;   //信息接收处理
    Creator * responseSend; //信息发送

    //转发功能
    QWidget * transmitWidget;

    //防火墙
    QWidget * fireWallWidget;

    //工具
    QWidget * toolWidget;
    QVBoxLayout * toolVbox;
    Tools * tool;

    /*其他属性*/
    NetCard * netCard;

    RecvMessage * recvMessage;
};

#endif // MAINWINDOW_H
