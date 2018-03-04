#ifndef STATUSINFO_H
#define STATUSINFO_H

#include <QWidget>
#include "netcard.h"

/*
 * 右侧状态栏,显示网卡信息以及系统配置等等
 * 暂时所有信息界面写到这一个文件中,先不分开
 * 但是信息获取比如网卡ip等等功能的实现要新建一个类统一好接口
 * 使用控制器模式,所有的功能由控制器控制,然后界面调用控制器
 */
namespace Ui {
class StatusInfo;
}

class StatusInfo : public QWidget
{
    Q_OBJECT

public:
    explicit StatusInfo(QWidget *parent = 0);

    StatusInfo(NetCard * netCard);

    ~StatusInfo();

    /*更新显示信息*/
    void showMessage();

    /*系统转发是否开启*/
    bool getTrans() const;

    bool getIcmpRes() const;


private slots:
    void on_ComboBox_netcard_currentIndexChanged(int index);

    void setPacketNum(int num);

    void on_checkBox_openIcmp_toggled(bool checked);

    void on_checkBox_openPromisc_toggled(bool checked);

    void on_checkBox_openTrans_toggled(bool checked);

signals:
    void netCardIndex(int);

private:
    Ui::StatusInfo *ui;

    NetCard * netCard;

    int indexCard;  //当前选择的网卡索引

    bool trans; //转发

    bool icmpRes;   //icmp响应是否开启
};

#endif // STATUSINFO_H
