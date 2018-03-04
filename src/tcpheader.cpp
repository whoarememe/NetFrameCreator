#include "tcpheader.h"
#include "ui_tcpheader.h"

TcpHeader::TcpHeader(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TcpHeader)
{
    ui->setupUi(this);
}

TcpHeader::TcpHeader(char *buf, NetCard *netCard):
    QWidget(0),
    ui(new Ui::TcpHeader)
{
    ui->setupUi(this);

    this->buf = buf;

    this->netCard = netCard;

    setDefaultInfo();

    memset(&f, 0, sizeof(struct flag));

    connect(netCard, SIGNAL(netChanged()), this, SLOT(setDefaultInfo()));

}

int TcpHeader::fillData(int index, int mtu)
{
    if (this->isHidden())
    {
        return 0;
    }

    bool ok;

    tcph = (struct tcphdr *)(buf + index);

    tcph->source = ui->lineEdit_srcPort->text().toUShort(&ok, 16);
    tcph->dest = ui->lineEdit_dstPort->text().toUShort(&ok, 16);
    tcph->dest = ui->lineEdit_32seq->text().toInt(&ok, 16);
    tcph->ack_seq = ui->lineEdit_32con->text().toInt(&ok, 16);
    tcph->res1 = ui->lineEdit_4len->text().toUShort(&ok, 16);

    tcph->urg = f.urg;
    tcph->ack = f.ack;
    tcph->psh = f.psh;
    tcph->rst = f.rst;
    tcph->syn = f.syn;
    tcph->fin = f.fin;

    tcph->window = ui->lineEdit_16window->text().toUShort(&ok, 16);
    /*校验和*/
    tcph->check = checkSum((unsigned short *)tcph, sizeof(struct tcphdr));

    tcph->urg_ptr = ui->lineEdit_16emerg->text().toUShort(&ok, 16);

    return sizeof(struct tcphdr);
}

/*
 * 进行crc校验
 */
unsigned short TcpHeader::checkSum(unsigned short *addr, int len)
{
    register int nleft = len;
    register int sum = 0;
    register unsigned short *w = addr;
    short answer = 0;

    while(nleft > 1)
    {
        sum += *w++;
        nleft -= 2;
    }

    if(nleft == 1)
    {
        *(unsigned char *)(&answer) = *(unsigned char *)w;
        sum += answer;
    }

    /*右移16位计算*/
    sum = (sum>>16)+(sum&0xffff);
    sum += (sum>>16);
    answer =~ sum;

    return answer;
}

TcpHeader::~TcpHeader()
{
    delete ui;
}

void TcpHeader::setDefaultInfo()
{
    QTime time;
    time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);

    //随机设置，以后根据右侧状态栏的进程进行设置
    ui->lineEdit_srcPort->setText(QString::number((unsigned short)(qrand()%65536) + 1024, 16));
    ui->lineEdit_dstPort->setText(QString::number((unsigned short)(qrand()%65536), 16));
    ui->lineEdit_32seq->setText(QString::number(qrand(), 16));
    ui->lineEdit_32con->setText(QString::number(qrand(), 16));

    ui->lineEdit_4len->setText(QString("4"));
    ui->lineEdit_16window->setText(QString::number((unsigned short)(qrand()%65536), 16));
    ui->lineEdit_16sum->setPlaceholderText(QString("程序自动计算或手动输入"));
    ui->lineEdit_16emerg->setText(QString::number((unsigned short)(qrand()%65536), 16));
    ui->lineEdit_32op->setPlaceholderText(QString("由首部长度确定"));


}

void TcpHeader::on_checkBox_urg_toggled(bool checked)
{
    if (checked)
    {
        f.urg = 1;
    }
    else
    {
        f.urg = 0;
    }
}

void TcpHeader::on_checkBox_ack_toggled(bool checked)
{
    if (checked)
    {
        f.ack = 1;
    }
    else
    {
        f.ack = 0;
    }
}

void TcpHeader::on_checkBox_psh_toggled(bool checked)
{
    if (checked)
    {
        f.psh = 1;
    }
    else
    {
        f.psh = 0;
    }
}

void TcpHeader::on_checkBox_rst_toggled(bool checked)
{
    if (checked)
    {
        f.rst = 1;
    }
    else
    {
        f.rst = 0;
    }
}

void TcpHeader::on_checkBox_syn_toggled(bool checked)
{
    if (checked)
    {
        f.syn = 1;
    }
    else
    {
        f.syn = 0;
    }
}

void TcpHeader::on_checkBox_fin_toggled(bool checked)
{
    if (checked)
    {
        f.fin = 1;
    }
    else
    {
        f.fin = 0;
    }
}
