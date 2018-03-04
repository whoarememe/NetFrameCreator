#ifndef CUSTOMDATA_H
#define CUSTOMDATA_H

#include <QWidget>

namespace Ui {
class CustomData;
}

class CustomData : public QWidget
{
    Q_OBJECT

public:
    explicit CustomData(QWidget *parent = 0);

    CustomData(char * buf, char * tmpBuf);

    int fillData(int index, int mtu);

    ~CustomData();

private:
    Ui::CustomData *ui;

    char * buf;

    char * tmpBuf;
};

#endif // CUSTOMDATA_H
