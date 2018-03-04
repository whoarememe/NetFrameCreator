#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>
#include <QThread>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /*网卡信息初始化*/
    netCard = new NetCard();

    /*
     * 右侧信息初始化
     */
    statusInfo = new StatusInfo(netCard);
    ui->verticalLayout_main_right->addWidget(statusInfo);

    //creator窗口
    creatorWidget = new QWidget();
    creatorVBox = new QVBoxLayout();
    infoDisplay = new InfoDisplay();
    creatorSend = new Creator(netCard);
    creatorVBox->addWidget(infoDisplay, 0);
    creatorVBox->addWidget(creatorSend, 1);
    creatorWidget->setLayout(creatorVBox);
    creatorVBox->setMargin(0);

    qDebug() << " i am in mainwindow , and i last creator";
    //response窗口
    responseWidget = new QWidget();
    responseVBox = new QVBoxLayout();
    responseAccept = new Creator(netCard);
    responseSend = new Creator(netCard);
    responseAccept->hideSendFrame();
    responseVBox->addWidget(responseAccept, 0);
    responseVBox->addWidget(responseSend, 1);
    responseVBox->setMargin(0);
    responseWidget->setLayout(responseVBox);

    qDebug() << " i am in mainwindow , and i last response";
    //转发窗口
    transmitWidget = new QWidget();

    //防火墙
    fireWallWidget = new QWidget;

    //工具
    toolWidget = new QWidget;
    toolVbox = new QVBoxLayout;
    tool = new Tools;

    toolVbox->addWidget(tool);
    toolVbox->setMargin(2);
    toolWidget->setLayout(toolVbox);

    /*接收信息初始化，将其扔到线程中去, 使用旧的方法*/
    recvMessage = new RecvMessage(netCard, infoDisplay, statusInfo);

    ui->tabWidget->setMovable(true);
    ui->tabWidget->addTab(creatorWidget, tr("构造"));
    ui->tabWidget->addTab(responseWidget, tr("响应"));
    ui->tabWidget->addTab(transmitWidget, tr("转发"));
    ui->tabWidget->addTab(fireWallWidget, tr("防火墙"));
    ui->tabWidget->addTab(toolWidget, tr("工具"));

    /*开始暂停抓包*/
    connect(infoDisplay, SIGNAL(willStart(bool)), this, SLOT(startPacket(bool)));
    connect(infoDisplay, SIGNAL(queryStr(QString)), recvMessage, SLOT(setFiltrate(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_exit_triggered()
{
    this->close();
}

void MainWindow::startPacket(bool b)
{
    if (b)
    {
        recvMessage->start();
    }
    else
    {
        recvMessage->stop();
    }
}

