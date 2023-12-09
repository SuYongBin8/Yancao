#ifndef CLEARDATA_H
#define CLEARDATA_H
#include <qmessagebox.h>
#include <QWidget>
#include <QThread>
#include <qdebug.h>
namespace Ui {
class ClearData;
}

class ClearData : public QWidget
{
    Q_OBJECT

public:
    explicit ClearData(QWidget *parent = nullptr);
    ~ClearData();


private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void setCombox(QStringList com);//设置combox

    void setCombox1(QStringList com);//设置combox

signals:
    void sendData(QStringList com);//设置combox


private:
    Ui::ClearData *ui;
};

#endif // CLEARDATA_H
