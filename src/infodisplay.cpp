#include "infodisplay.h"
#include "ui_infodisplay.h"

#include <QDebug>

InfoDisplay::InfoDisplay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InfoDisplay)
{
    ui->setupUi(this);

    ui->pushButton_start->setEnabled(true);
    ui->pushButton_stop->setEnabled(false);
    ui->textEdit->setReadOnly(true);

    stopFlag = true;
}

/*
 * 显示信息，每捕捉到一条信息就显示
 */
void InfoDisplay::showMessage(QString str)
{
    ui->textEdit->append(str);
}

void InfoDisplay::clearMessage()
{
    ui->textEdit->clear();
}

void InfoDisplay::setTitle(QString str)
{
    ui->label_title->setText(tr("信息显示：当前网卡--") + str);
}

InfoDisplay::~InfoDisplay()
{
    delete ui;
}

void InfoDisplay::on_pushButton_start_clicked()
{
    ui->pushButton_start->setEnabled(false);
    ui->pushButton_stop->setEnabled(true);
    emit willStart(true);
}

void InfoDisplay::on_pushButton_stop_clicked()
{
    ui->pushButton_start->setEnabled(true);
    ui->pushButton_stop->setEnabled(false);
    emit willStart(false);
}

void InfoDisplay::on_pushButton_clicked()
{
    emit(ui->lineEdit_filtter->text());

}
