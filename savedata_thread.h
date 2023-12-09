#ifndef SAVEDATA_THREAD_H
#define SAVEDATA_THREAD_H
#include "qdatetime.h"
#include <QTableWidget>
#include <QThread>
#include <QDebug>
#include <QStringListModel>
#include <qmessagebox.h>
#include <QFile>
#include <QTimer>
#include <QInputDialog>
#include <QFileDialog>
// 引入串口通信的两个头文件(第一步)
#include <QElapsedTimer>
#include "xlsx/xlsxdocument.h"

class sonThread : public QThread
{
    Q_OBJECT
public:
    explicit sonThread(QObject *parent = nullptr);
    void run();

public:
    QTableWidget *table;
    QString name;
    int isAll;
};


#endif // SAVEDATA_THREAD_H
