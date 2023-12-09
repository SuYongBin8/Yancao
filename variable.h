#ifndef VARIABLE_H
#define VARIABLE_H
#include <QMainWindow>
#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QMessageBox>
#include <QDebug>
#include <vector>
#include <QDateTime>
#include <QMenu>
#include <QDir>
#include <QFileDialog>
#include <QDesktopServices>
#include <QTableWidget>
#include <math.h>
#include <savedata_thread.h>
#include <qelapsedtimer.h>
#include "QStandardItem"
#include <QMouseEvent>
#include <QtNetwork>

struct rowmsg
{
    int start;
    int count;
    int jiange;
};
extern int mode;//mode=1表示烘前录入，mode=2表示烘后计算
extern QList<QHash<QString,QHash <QString, QString>>> total_my_msg;//所有的信息数据
extern QHash<QString,QHash <QString, QString>> my_msg;//每个采样点和牌号对应的信息数据
extern int nbcompare;//表示该组多少个平行样算一个平均值
extern QStringList my_msg_rank;//保存某一次顺序录入的编号
extern QHash<QString,QStringList> total_my_msg_rank;//保存总的顺序录入的编号
extern QHash<QString,QStringList> combox;
extern bool isConnect;

extern bool tcpfree;//是否处于空闲状态（非接收数据）

#endif // CANTHREAD_H


