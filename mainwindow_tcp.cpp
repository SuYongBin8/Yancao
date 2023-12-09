#include "mainwindow.h"
#include "ui_mainwindow.h"

bool isConnect=false;
bool tcpfree=true;//是否处于空闲状态（非接收数据）
//启动TCP服务器
void MainWindow::on_pushButton_28_clicked()
{
    //1.构造QTcpServer对象
    mServer = new QTcpServer(this);
    //2.连接客户端连接上来的信号
    QObject::connect(mServer,&QTcpServer::newConnection,this,[&](){
        //获取客户端连接上来的套接字
        mSocket = mServer->nextPendingConnection();
        //打印客户端的地址
        QHostAddress c_addr = mSocket->peerAddress();
        qDebug()<<c_addr.toString()<<"连接上来!";

        //使能发送输入框和按钮
        ui->lineEdit->setEnabled(true);
        ui->pushButton_29->setEnabled(true);
        isConnect=true;
        //连接readyRead信号和槽
        QObject::connect(mSocket,&QTcpSocket::readyRead,this,[&](){
            tcpfree=false;
            //接收数据
            QByteArray arr = mSocket->readAll();
            //转换成字符串
            QString str(arr);
            qDebug()<<str;
            //tcp共享数据
            QStringList aa = str.split("#");
            if(aa.length()<1)return;
            if(aa[0]=="add")//增加内容
            {
                ui->pushButton_8->setDisabled(true);//防止同时按下保存，乱掉
                int rank_index=0;
                QHash<QString,QHash <QString, QString>> msg;//每个采样点和牌号对应的信息数据
                for(int i=1;i<aa.length();i++)
                {
                    if(aa[i]=="rank")
                    {
                        rank_index=i;
                        break;
                    }
                    QStringList bb = aa[i].split("%");
                    if(bb.length()<1){ui->pushButton_8->setDisabled(false);return;}
                    QHash <QString, QString> msg1;//"表显值"<<"容器重量(g)"<<"烘前重量(g)"<<"烘后重量(g)"<<"样品水分(%)"<<"烘箱测试值(%)";
                    msg1.insert("工序点",bb[1]);
                    msg1.insert("牌号",bb[2]);
                    msg1.insert("验证部门","无");
                    msg1.insert("验证依据","无");
                    msg1.insert("水分仪编号","无");
                    msg1.insert("电子天平编号","无");
                    msg1.insert("烘箱编号","无");
                    msg1.insert("质检员","无");
                    msg1.insert("日期","无");
                    msg1.insert("表显值","0");
                    msg1.insert("容器重量",bb[3]);
                    msg1.insert("烘前重量",bb[4]);
                    msg1.insert("烘后重量","0");
                    msg1.insert("样品水分","-1%");
                    msg1.insert("烘箱测试值","-1%");
                    msg1.insert("总平均水分","-1%");
                    msg1.insert("nbcompare",bb[5]);
                    msg.insert(bb[0],msg1);
                }

                QStringList msg_rank;//保存某一次顺序录入的编号
                QStringList cc = aa[rank_index+2].split("%");
                if(cc.length()<1){ui->pushButton_8->setDisabled(false);return;}
                for(int i=0;i<cc.length();i++)
                {
                    msg_rank.append(cc[i]);
                }
                //检查同牌号工序点是否存在，存在就删掉
                QString gx = aa[rank_index+1].split("_")[0];
                QString ph = aa[rank_index+1].split("_")[1];
                int flag=0;
                for(int i=0;i<total_my_msg.length();i++)
                {
                    for (auto item = total_my_msg[i].begin(); item != total_my_msg[i].end(); ++item)
                    {
                        if(item.value()["牌号"]==ph &&item.value()["工序点"] == gx)//找到了有
                        {
                            total_my_msg.removeAt(i);
                            total_my_msg_rank.remove(aa[rank_index+1]);
                            flag=1;
                            break;
                        }
                    }
                    if(flag==1)break;
                }

                total_my_msg_rank.insert(aa[rank_index+1],msg_rank);
                total_my_msg.append(msg);
                writeTotal("D:/work/savedata/total.txt",false);
                writeTotal("D:/work/savedata/data.txt",true);
                ui->pushButton_8->setDisabled(false);//防止同时按下保存，乱掉

            }
            else if(aa[0]=="change")//烘后录入内容
            {
                QStringList bb = aa[1].split("%");
                if(bb.length()!=6)return;
                qDebug()<<bb;
                for(int i=0;i<total_my_msg.length();i++)
                {
                    for (auto item = total_my_msg[i].begin(); item != total_my_msg[i].end(); ++item)
                    {
                        if(item.key()==bb[0])
                        {
                            total_my_msg[i][bb[0]]["烘后重量"]=bb[1];//直接赋值给最后一个容器重量
                            total_my_msg[i][bb[0]]["样品水分"]=bb[2]+"%";
                            total_my_msg[i][bb[0]]["容器重量"]=bb[3];
                            total_my_msg[i][bb[0]]["烘前重量"]=bb[4];
                            if(bb[5]!="999"&&bb[5]!="999change")
                            {
                                total_my_msg[i][bb[0]]["表显值"]=bb[5];
                            }
                        }
                    }
                }
            }
            else if(aa[0]=="delete")//删除内容
            {
                try {


                QStringList paihao=aa[1].split("%");
                QStringList gongxudian=aa[2].split("%");
                if(paihao.length()<1||gongxudian.length()<1)return;
                paihao.removeLast();
                gongxudian.removeLast();
                bool flag=false;
                bool flag_all=false;
                for(int i=total_my_msg.length()-1;i>=0;i--)
                {
                    for (auto item = total_my_msg[i].begin(); item != total_my_msg[i].end(); ++item)
                    {
                        if(paihao.length()>0&&gongxudian.length()>0)
                        {
                            for(int j=0;j<paihao.length();j++)
                            {
                                if(paihao[j]=="全部")
                                {
                                    flag_all=true;
                                }
                                for(int m=0;m<gongxudian.length();m++)
                                {
                                    if(item.value()["牌号"]==paihao[j]&&gongxudian[m]=="全部")
                                    {
                                        total_my_msg.removeAt(i);
                                        total_my_msg_rank.remove(item.value()["工序点"]+"_"+item.value()["牌号"]);
                                        flag=true;
                                        break;
                                    }
                                    else if(item.value()["牌号"]==paihao[j]&&item.value()["工序点"]==gongxudian[m])
                                    {
                                        total_my_msg.removeAt(i);
                                        total_my_msg_rank.remove(item.value()["工序点"]+"_"+item.value()["牌号"]);
                                        flag=true;
                                        break;
                                    }
                                }
                            }
                        }
                        if(flag) break;
                    }
                }
                if(flag_all)
                {
                    my_msg_rank.clear();
                    my_msg.clear();
                    total_my_msg_rank.clear();
                    total_my_msg.clear();
                }

                writeTotal("D:/work/savedata/total.txt",false);
                writeTotal("D:/work/savedata/data.txt",true);
                } catch (...) {
                    QMessageBox::warning(this,"提示","tcp通信异常",QMessageBox::Yes);
                }
            }
            else if(aa[0]=="same"||aa[0]=="same1")//增加内容
            {
                static QString aa_all;
                static bool goon = false;
                if(aa[0]=="same")
                {
                    aa_all+=str;
                }
                else if(aa[0]=="same1")
                {
                    str.remove(0,6);
                    aa_all+=str;
                    if(aa[aa.length()-1]=="done")
                    {
                        goon=true;
                        aa = aa_all.split("#");
                        aa_all.clear();
                    }
                }
                if(goon==false)return;
                goon=false;

                total_my_msg_rank.clear();
                total_my_msg.clear();
                int rank_index=0;
                QHash<QString,QHash <QString, QString>> msg;//每个采样点和牌号对应的信息数据
                for(int i=1;i<aa.length();i++)
                {
                    if(aa[i]=="rank")
                    {
                        rank_index=i;
                        break;
                    }
                    if(aa[i]=="$")
                    {
                        total_my_msg.append(msg);
                        msg.clear();
                        continue;
                    }
                    QStringList bb = aa[i].split("%");
                    if(bb.length()<1)return;
                    QHash <QString, QString> msg1;//"表显值"<<"容器重量(g)"<<"烘前重量(g)"<<"烘后重量(g)"<<"样品水分(%)"<<"烘箱测试值(%)";
                    msg1.insert("工序点",bb[1]);
                    msg1.insert("牌号",bb[2]);
                    msg1.insert("验证部门",bb[3]);
                    msg1.insert("验证依据",bb[4]);
                    msg1.insert("水分仪编号",bb[5]);
                    msg1.insert("电子天平编号",bb[6]);
                    msg1.insert("烘箱编号",bb[7]);
                    msg1.insert("质检员",bb[8]);
                    msg1.insert("日期",bb[9]);
                    msg1.insert("表显值",bb[10]);
                    msg1.insert("容器重量",bb[11]);
                    msg1.insert("烘前重量",bb[12]);
                    msg1.insert("烘后重量",bb[13]);
                    msg1.insert("样品水分","-1%");
                    msg1.insert("烘箱测试值","-1%");
                    msg1.insert("总平均水分","-1%");
                    msg1.insert("nbcompare",bb[14]);
                    msg.insert(bb[0],msg1);

                }

                QStringList msg_rank;//保存某一次顺序录入的编号
                for(int i=rank_index+1;i<aa.length()-1;i++)
                {

                    QStringList cc = aa[i].split("*");
                    QStringList dd = cc[1].split("%");
                    dd.removeFirst();
                    total_my_msg_rank.insert(cc[0],dd);
                }



                writeTotal("D:/work/savedata/total.txt",false);
                writeTotal("D:/work/savedata/data.txt",true);
            }
            //显示
            update_TableWidget_2();
            ui->textEdit->clear();
            ui->textEdit->insertPlainText("rcv:"+str+"\n");
            tcpfree=true;
        });
    });

    //3.监听
    mServer->listen(QHostAddress::Any,ui->spinBox->value());

    //连接回车发送的信号和槽
    QObject::connect(ui->lineEdit_2,&QLineEdit::returnPressed,this,&MainWindow::on_pushButton_29_clicked);
    //禁止端口号和启动按钮
    ui->spinBox->setEnabled(false);
    ui->pushButton_28->setEnabled(false);

}

void MainWindow::on_pushButton_29_clicked()
{
    //获取发送的数据
    QString str = ui->lineEdit_2->text();
    //转换成QByteArray
    QByteArray arr;
    arr.append(str);
    //发送
    if(isConnect==false || mSocket->state()!=QAbstractSocket::ConnectedState)
    {
        QMessageBox::warning(this,"提示","设备未链接，请检查另一台设备是否正常",QMessageBox::Yes);
        return;
    }

    send_write_arr(0,arr);


}
void sleepMsec(int msec)
{
    QTime curTimer=QTime::currentTime().addMSecs(msec);
    while(QTime::currentTime()<curTimer)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents,100);
    }
}
void MainWindow::send_write_arr(int yanshi,QByteArray arr)
{
    if(yanshi>1)
    {
        do{
            //QThread::msleep(1);
            sleepMsec(1);
            yanshi--;
        }while(tcpfree==false&&yanshi>0);

        if(yanshi>1)
            mSocket->write(arr);
        else
            QMessageBox::warning(this,"提示","tcp发送异常",QMessageBox::Yes);
    }
    else
        mSocket->write(arr);
}

//连接服务器
void MainWindow::on_pushButton_30_clicked()
{
//    //1.创建QTcpSocket对象
//    mSocket1 = new QTcpSocket(this);
//    //2.连接连接成功的信号和槽
//    QObject::connect(mSocket1,&QTcpSocket::connected,this,[&](){
//        //使能发送按钮和发送输入框
//        ui->lineEdit_4->setEnabled(true);
//        ui->pushButton_31->setEnabled(true);

//        //禁止ip 端口号和连接按钮
//        ui->lineEdit_3->setEnabled(false);
//        ui->spinBox->setEnabled(false);
//        ui->pushButton_30->setEnabled(false);


//        //连接readyRead信号和槽
//        QObject::connect(mSocket1,&QTcpSocket::readyRead,this,[&](){
//            //接收数据
//            QByteArray arr = mSocket1->readAll();
//            //转换成字符串
//            QString str(arr);
//            //显示
//            ui->textEdit_2->insertPlainText("rcv:"+str+"\n");
//        });
//    });


//    //3.连接服务器
//    QString ip = ui->lineEdit_3->text();
//    quint16 port = ui->spinBox_2->value();
//    qDebug()<<ip<<port;
//    mSocket1->connectToHost(ip,port);

//    //连接回车发送的信号和槽
//    QObject::connect(ui->lineEdit_4,&QLineEdit::returnPressed,this,&MainWindow::on_pushButton_31_clicked);
}

//发送
void MainWindow::on_pushButton_31_clicked()
{
//    //获取发送的数据
//    QString str = ui->lineEdit_4->text();
//    //转换成QByteArray
//    QByteArray arr;
//    arr.append(str);

//    //发送
//    mSocket1->write(arr);

}

void MainWindow::on_pushButton_32_clicked()//同步数据至另一台
{
    QMessageBox msgBox;
    msgBox.setText("温馨提示：");
    msgBox.setInformativeText("是否同步数据？");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    int ret = msgBox.exec();
    if(ret == QMessageBox::Cancel)
    {return;}
    //tcp共享数据
    QByteArray arr;
    arr.append("same#");
    // 输入内容
    for(int i=0;i<total_my_msg.length();i++)
    {
        for (auto item = total_my_msg[i].begin(); item != total_my_msg[i].end(); ++item)
        {
            arr.append(item.key()+"%"+item.value()["工序点"]+"%"+item.value()["牌号"]+"%"+item.value()["验证部门"]
                  +"%"+item.value()["验证依据"]+"%"+item.value()["水分仪编号"]+"%"+item.value()["电子天平编号"]+"%"+item.value()["烘箱编号"]
                    +"%"+item.value()["质检员"]+"%"+item.value()["日期"]+"%"+item.value()["表显值"]+"%"+item.value()["容器重量"]+"%"
                    +item.value()["烘前重量"]+"%"+item.value()["烘后重量"]+"%"+item.value()["nbcompare"]+"#");
        }
        arr.append("$#");
        //发送
        if(isConnect==true && mSocket->state()==QAbstractSocket::ConnectedState)
        {
            send_write_arr(0,arr);
            sleepMsec(100);

            arr.clear();
            arr.append("same1#");
        }
    }

    arr.append("rank#");
    for (auto item = total_my_msg_rank.begin(); item != total_my_msg_rank.end(); ++item)
    {
        arr.append(item.key()+"*");
        for(int i=0;i<item.value().length();i++)
            arr.append("%"+item.value()[i]);
        arr.append("#");
    }
    arr.append("done");

    //发送
    if(isConnect==true && mSocket->state()==QAbstractSocket::ConnectedState)
    {
        send_write_arr(0,arr);
    }
}
