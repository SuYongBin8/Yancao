#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "cleardata.h"
#include <variable.h>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //串口
    QSerialPort serial1,serial2;
    QString Port1_str,Port2_str;

    void FindSerial1Port();

    void FindSerial2Port();


    void TableWidgetInit();
    void update_TableWidget();
    void update_TableWidget_2();

    void writeTxt();
    void readTxt();
    void writeComD();
    void readComD();
    void calc();
    void panduan();
    void writeTotal(QString fileName,bool isAppend);
    void readTotal(QString fileName);
    void send_write_arr(int yanshi,QByteArray arr);
    //鼠标拖动窗体移动
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    bool        m_bDrag;
    QPoint      mouseStartPoint;
    QPoint      windowTopLeftPoint;


signals:
    //自定义信号
    void signalWeight(QString msg);

    void signalQRCode(QString msg);

    void sendCombox(QStringList com);
    void sendCombox1(QStringList com);

private slots:
    void getData(QStringList com);

    void slotTimeout();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_2_clicked();

    void QRCodeHandle(QString msg);

    void WeightHandle(QString msg);

    void serial1_rcv();

    void serial2_rcv();

    void on_open1_clicked();

    void on_open2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_19_clicked();

    void on_pushButton_20_clicked();

    void on_pushButton_21_clicked();

    void on_comboBox_9_currentIndexChanged(int index);


    void on_pushButton_23_clicked();

    void on_pushButton_24_clicked();

    void on_pushButton_25_clicked();

    void on_comboBox_2_currentTextChanged(const QString &arg1);

    void on_pushButton_27_clicked();

    void on_tableWidget_2_cellChanged(int row, int column);

    void on_pushButton_28_clicked();

    void on_pushButton_29_clicked();

    void on_pushButton_30_clicked();

    void on_pushButton_31_clicked();

    void on_pushButton_32_clicked();

    void on_comboBox_12_currentIndexChanged(int index);

    void on_tableWidget_cellChanged(int row, int column);

private:

    Ui::MainWindow *ui;
    QTimer *m_timer;                                // 定时器
    ClearData *form;

    QTcpServer *mServer;
    QTcpSocket *mSocket;
    QTcpSocket *mSocket1;
};
#endif // MAINWINDOW_H
