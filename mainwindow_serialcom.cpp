#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::FindSerial1Port()//天平
{
    ui->cbo_port1->clear();
    const auto infos = QSerialPortInfo::availablePorts();
    for(const QSerialPortInfo &info : infos)
    {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            ui->cbo_port1->addItem(info.portName());
            serial.close();
        }
    }
}

void MainWindow::FindSerial2Port()//扫码
{
    ui->cbo_port2->clear();
    const auto infos = QSerialPortInfo::availablePorts();
    for(const QSerialPortInfo &info : infos)
    {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            ui->cbo_port2->addItem(info.portName());
            serial.close();
        }
    }
}

void MainWindow::on_open1_clicked()//打开天平的串口
{
    if(serial1.isOpen())
    {
        serial1.close();
        ui->open1->setText("打开串口");
        ui->label_12->setText("未连接");
        ui->label_31->setText("未连接");
    }
    else
    {
        serial1.setPortName("COM7");//(ui->cbo_port1->currentText());
        if(!serial1.open(QIODevice::ReadWrite))
        {
            QMessageBox::warning(this,"提示","串口打开失败！ ",QMessageBox::Yes);
            return ;
        }
        serial1.setBaudRate(ui->cbo_baudRate1->currentText().toInt());
        serial1.setDataBits(QSerialPort::Data8);
        serial1.setParity(QSerialPort::NoParity);
        serial1.setStopBits(QSerialPort::OneStop);
        serial1.setFlowControl(QSerialPort::NoFlowControl);
        ui->open1->setText("关闭串口");
        ui->label_12->setText("已连接");
        ui->label_31->setText("已连接");

    }
}


void MainWindow::on_open2_clicked()//打开扫码的串口
{
    if(serial2.isOpen())
    {
        serial2.close();

        ui->open2->setText("打开串口");
        ui->label_11->setText("未连接");
        ui->label_30->setText("未连接");
    }
    else
    {
        serial2.setPortName("COM6");//(ui->cbo_port2->currentText());

        if(!serial2.open(QIODevice::ReadWrite))
        {
            QMessageBox::warning(this,"提示","串口打开失败！",QMessageBox::Yes);
            return ;
        }

        serial2.setBaudRate(ui->cbo_baudRate2->currentText().toInt());
        serial2.setDataBits(QSerialPort::Data8);
        serial2.setParity(QSerialPort::NoParity);
        serial2.setStopBits(QSerialPort::OneStop);
        serial2.setFlowControl(QSerialPort::NoFlowControl);
        ui->open2->setText("关闭串口");
        ui->label_11->setText("已连接");
        ui->label_30->setText("已连接");
    }
}

//收到扫码数据
void MainWindow::serial1_rcv()
{
    QString Number = serial1.readAll();
    Port1_str.append(Number);

    if(Port1_str.contains('\r'))
    {

        //QString  = QString(bytes);
        Port1_str = Port1_str.simplified();
        emit signalQRCode(Port1_str);   //发送完整二维码数据信号

        QDateTime current_time = QDateTime::currentDateTime();
        QString log = current_time.toString("yyyy-MM-dd hh:mm:ss") + ": " + Port1_str + " (扫码模块数据)";
        ui->textEdit_recv1->append(log);
        Port1_str.clear();
    }
}

//收到天平数据
void MainWindow::serial2_rcv()
{
    QString Number = serial2.readAll();
    Port2_str.append(Number);

    if(Port2_str.contains('\r'))
    {
        Port2_str = Port2_str.simplified();
        emit signalWeight(Port2_str);   //发送完整天平数据信号

        QDateTime current_time = QDateTime::currentDateTime();
        QString log = current_time.toString("yyyy-MM-dd hh:mm:ss") + ": " + Port2_str + " (天平数据)";
        ui->textEdit_recv2->append(log);
        Port2_str.clear();

    }
}
QList<QHash<QString,QHash <QString, QString>>> total_my_msg;//所有的信息数据
QHash<QString,QHash <QString, QString>> my_msg;//每个采样点和牌号对应的信息数据
QStringList my_msg_rank;//保存某一次顺序录入的编号
QHash<QString,QStringList> total_my_msg_rank;//保存总的顺序录入的编号
QString cur_erweima="000000";
int mode2=0;//数据处理阶段是否有找到扫码的数据
// 二维码数据处理
void MainWindow::QRCodeHandle(QString msg)
{
try {
    if(msg.length()!=6||msg[0]!='0')return;//过滤掉一些不正常的东西
    if(mode==1)
    {
        for(int i=0;i<total_my_msg.length();i++)
        {
            for (auto item = total_my_msg[i].begin(); item != total_my_msg[i].end(); ++item)
            {
                if(item.key()==msg)//找到了有
                {
                    //QMessageBox::warning(this,"提示","该二维码已经在其他牌号工序点使用过，请删除后再使用！",QMessageBox::Yes);
                    return;
                }
            }
        }
    }
    cur_erweima=msg;
    for (auto item = my_msg.begin(); item != my_msg.end(); ++item)
    {
        if(item.key()==msg)return; //已经扫过了
    }



    if(mode==1)//录入信息
    {
        my_msg_rank.append(cur_erweima);
        QHash <QString, QString> msg1;//"表显值"<<"容器重量(g)"<<"烘前重量(g)"<<"烘后重量(g)"<<"样品水分(%)"<<"烘箱测试值(%)";
        msg1.insert("工序点",ui->comboBox->currentText());
        msg1.insert("牌号",ui->comboBox_2->currentText());
        msg1.insert("验证部门",ui->comboBox_3->currentText());
        msg1.insert("验证依据",ui->comboBox_4->currentText());
        msg1.insert("水分仪编号",ui->comboBox_5->currentText());
        msg1.insert("电子天平编号",ui->comboBox_6->currentText());
        msg1.insert("烘箱编号",ui->comboBox_7->currentText());
        msg1.insert("质检员",ui->comboBox_8->currentText());
        msg1.insert("日期",ui->dateEdit_ManuDate->text());
        msg1.insert("表显值","0");
        msg1.insert("容器重量","0");
        msg1.insert("烘前重量","0");
        msg1.insert("烘后重量","0");
        msg1.insert("样品水分","-1%");
        msg1.insert("烘箱测试值","-1%");
        msg1.insert("总平均水分","-1%");
        msg1.insert("nbcompare",QString::number(nbcompare));
        my_msg.insert(msg,msg1);
        update_TableWidget();
    }
    else if(mode==2)
    {
        for(int i=0;i<total_my_msg.length();i++)
        {
            for (auto item = total_my_msg[i].begin(); item != total_my_msg[i].end(); ++item)
            {

                if(item.key()==cur_erweima)//找到了有
                {
                    mode2=1;
                    update_TableWidget_2();
                    for(int j=0;j<ui->tableWidget_2->rowCount();j++)
                    {
                        if(ui->tableWidget_2->item(j,2)->text()==cur_erweima)
                        {
                            ui->tableWidget_2->item(j,6)->setBackgroundColor(QColor(255,255,255));
                            ui->tableWidget_2->scrollToItem(ui->tableWidget_2->item(j,6), QAbstractItemView::PositionAtCenter);
                        }
                    }
                    return;
                }
            }

        }
        //如果没找到
        mode2=0;
        QMessageBox::warning(this,"提示","找不到该数据的信息",QMessageBox::Yes);

    }
    } catch (...) {
        QMessageBox::warning(this,"提示","程序出现异常，请重试！",QMessageBox::Yes);
    }
}


//  重量数据处理
void MainWindow::WeightHandle(QString msg)
{
try {


    double weight;
    if(msg.split(" ").length()<=0)return;//过滤掉一些不正常的东西

    if(msg.split(" ")[0]=="N")//主站
    {
        weight = msg.split(" ")[1].toDouble();
    }
    else if(msg.split(" ").length()==2 && msg.split(" ")[1]=='g')
    {
        weight = msg.split(" ")[0].toDouble();
    }
    else
    {
        return;
    }

//    if(msg.split(" ")[0]=="S")//从站
//    {
//        weight = msg.split(" ")[2].toDouble();
//    }
//    else
//    {
//        return;
//    }
    ui->label_Weight->setText(QString::number(weight));
    ui->label_Weight_2->setText(QString::number(weight));
//防止第一个录入每扫到盒号，按确认键闪退，因为没有cur_erweima，访问my_msg[cur_erweima]["容器重量"]会有问题
    bool is_cur_erweima_exist = false;
    for (auto item = my_msg.begin(); item != my_msg.end(); ++item)
    {
        if(item.key()==cur_erweima)//找到了有，通过
        {
            is_cur_erweima_exist = true;
            break;
        }
    }
    if(!is_cur_erweima_exist)return;

    if(mode==1)
    {
        if(weight<0)//空盒重量
        {
            my_msg[cur_erweima]["容器重量"]=QString::number(-weight);//直接赋值给最后一个容器重量
        }
        else
        {
            my_msg[cur_erweima]["烘前重量"]=QString::number(weight);//直接赋值给最后一个烘前重量
            if(my_msg[cur_erweima]["烘前重量"].toDouble()<4.0||my_msg[cur_erweima]["烘前重量"].toDouble()>6.0)
            {
                QMessageBox::warning(this,"提示","重量不符合要求，请保持重量在4-6g之间！",QMessageBox::Yes);
                return;
            }

        }
        update_TableWidget();

    }
    else if(mode==2)//烘后模式
    {
        if(mode2==1)//有找到
        {
            if(weight>0)//重量zhon
            {
                for(int i=0;i<total_my_msg.length();i++)
                {
                    for (auto item = total_my_msg[i].begin(); item != total_my_msg[i].end(); ++item)
                    {
                        if(item.key()==cur_erweima)
                        {
                            total_my_msg[i][cur_erweima]["烘后重量"]=QString::number(weight-total_my_msg[i][cur_erweima]["容器重量"].toDouble());//直接赋值给最后一个容器重量

                            double w_qian = total_my_msg[i][cur_erweima]["烘前重量"].toDouble();
                            double w_hou = total_my_msg[i][cur_erweima]["烘后重量"].toDouble();
                            total_my_msg[i][cur_erweima]["样品水分"]=QString::number(100*(w_qian-w_hou)/w_qian,'f',2)+"%";
                            update_TableWidget_2();
                            writeTotal("D:/work/savedata/total.txt",false);

                            //tcp共享数据
                            QByteArray arr;
                            arr.append("change#"+cur_erweima+"%"+total_my_msg[i][cur_erweima]["烘后重量"]+"%"+QString::number(100*(w_qian-w_hou)/w_qian,'f',2)
                                    +"%"+total_my_msg[i][cur_erweima]["容器重量"]+"%"+total_my_msg[i][cur_erweima]["烘前重量"]
                                    +"%999");
                            //发送
                            if(isConnect==true && mSocket->state()==QAbstractSocket::ConnectedState)
                            {
                                send_write_arr(0,arr);
                            }
                        }
                    }
                }

            }
        }
    }

    } catch (...) {
        QMessageBox::warning(this,"提示","程序出现异常，请重试！",QMessageBox::Yes);
    }
}
