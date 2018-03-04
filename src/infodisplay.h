#ifndef INFODISPLAY_H
#define INFODISPLAY_H

#include <QWidget>

#include "netcard.h"

namespace Ui {
class InfoDisplay;
}

class InfoDisplay : public QWidget
{
    Q_OBJECT

public:
    explicit InfoDisplay(QWidget *parent = 0);

    InfoDisplay(NetCard * netCard);

    void showMessage(QString str);

    /*清空数据*/
    void clearMessage();

    void setTitle(QString str);

    ~InfoDisplay();

private slots:
    void on_pushButton_start_clicked();

    void on_pushButton_stop_clicked();

    void on_pushButton_clicked();

signals:
    void willStart(bool);  //是否开始的信号

    void queryStr(QString);

private:
    Ui::InfoDisplay *ui;

    NetCard * netCard;

    bool stopFlag;    //判断是不是停止了抓包
};

#endif // INFODISPLAY_H
