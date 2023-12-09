#include "mainwindow.h"
#include "ui_mainwindow.h"
//界面操作相关的代码
bool enable_move=true;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 默认页面
    ui->stackedWidget->setCurrentIndex(0);

    connect(&serial1,SIGNAL(readyRead()),this,SLOT(serial1_rcv()));

    connect(&serial2,SIGNAL(readyRead()),this,SLOT(serial2_rcv()));

    //自定义槽函数连接
    // 二维码信号
    connect(this,SIGNAL(signalQRCode(QString)),this,SLOT(QRCodeHandle(QString)));
    // 重量信号
    connect(this,SIGNAL(signalWeight(QString)),this,SLOT(WeightHandle(QString)));

    FindSerial1Port();//搜索串口并置于列表中
    FindSerial2Port();
    ui->open1->click();
    ui->open2->click();

    ui->dateEdit_ManuDate->setDate(QDate::currentDate());
    ui->dateEdit_ManuDate->setCalendarPopup(true);
    TableWidgetInit();
    readTxt();
    readComD();
    ui->comboBox_12->setCurrentIndex(1);//创造一个变化
    connect(ui->btn1, &QPushButton::clicked, [=]() {
        this->showMinimized();
    });
    connect(ui->btn2, &QPushButton::clicked, [=]() {
        if (this->isFullScreen()){
            this->showNormal();
            enable_move=true;
            ui->btn2->setIcon(QIcon("D:/work/Yan/resource/full.png"));
        }
        else{
            enable_move=false;
            this->showFullScreen();
            ui->btn2->setIcon(QIcon("D:/work/Yan/resource/small.png"));
        }
        });
    connect(ui->btn3, &QPushButton::clicked, [=]() {
        QMessageBox msgBox;
        msgBox.setText("温馨提示：");
        msgBox.setInformativeText("是否确认关闭？");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Cancel);
        int ret = msgBox.exec();
        if(ret == QMessageBox::Ok)
        {
            form->close();
            this->close();
        }
    });
    setWindowFlags(Qt::FramelessWindowHint);//直接隐藏掉，这样更好

    m_timer = new QTimer(this);
    // 开启计算显示帧率的定时器 1000ms
    m_timer->start(100);
    //将定时器超时信号与槽(功能函数)联系起来
    connect(m_timer, SIGNAL(timeout()), this, SLOT(slotTimeout()));

    form = new ClearData();
    connect(this,SIGNAL(sendCombox(QStringList)),form,SLOT(setCombox(QStringList)));//给子窗口传数据
    connect(this,SIGNAL(sendCombox1(QStringList)),form,SLOT(setCombox1(QStringList)));//给子窗口传数据
    connect(form,SIGNAL(sendData(QStringList)),this,SLOT(getData(QStringList)));//接收子窗口数据

    ui->pushButton_7->hide();
    ui->pushButton_9->hide();
    ui->pushButton_10->hide();
    ui->pushButton_11->hide();
    ui->pushButton_14->hide();
    ui->pushButton_15->hide();

    ui->pushButton_28->click();//tcp连接


    ui->comboBox_12->setCurrentIndex(0);

}

MainWindow::~MainWindow()
{
    delete form;
    delete ui;

}

void MainWindow::slotTimeout()
{
    if(ui->stackedWidget->currentIndex()==2)
    {
        ui->pushButton_16->hide();
        ui->pushButton_5->hide();
        ui->pushButton_6->hide();
        ui->pushButton_2->hide();
    }
    else
    {
        ui->pushButton_16->show();
        ui->pushButton_5->show();
        ui->pushButton_6->show();
        ui->pushButton_2->show();
    }

    if(ui->comboBox_10->currentIndex()==0)
    {
        ui->comboBox_11->hide();
    }
    else
    {
        ui->comboBox_11->show();
    }

    if(mode==2)
    {
        ui->pushButton_23->setDisabled(true);
    }
    else
    {
        ui->pushButton_23->setDisabled(false);
    }
}
void MainWindow::on_pushButton_5_clicked()
{

    if(mode==1)
    {
        QMessageBox msgBox;
        msgBox.setText("温馨提示：");
        msgBox.setInformativeText("该工序样品是否已完成？");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Cancel);
        int ret = msgBox.exec();
        if(ret == QMessageBox::Ok)
        {
            if(my_msg_rank.length()==0)
            {
                QMessageBox::warning(this,"提示","已保存，不需要重复保存。",QMessageBox::Yes);
                ui->comboBox->setCurrentIndex(0);
                ui->comboBox_2->setCurrentIndex(0);
                ui->stackedWidget->setCurrentIndex(0);
                return;
            }
            else if(ui->comboBox->currentText()!="710E比对"&&ui->comboBox->currentText()!="烘箱比对"&&(my_msg_rank.length()%2!=0||my_msg_rank.length()<1))
            {
                QMessageBox::warning(this,"提示","录入信息异常，请检查！",QMessageBox::Yes);
                return ;
            }
            else if(ui->comboBox->currentText()=="710E比对"&&(my_msg_rank.length()%4!=0||my_msg_rank.length()<1))
            {
                QMessageBox::warning(this,"提示","录入信息异常，请检查！",QMessageBox::Yes);
                return ;
            }
            else if(ui->comboBox->currentText()=="烘箱比对"&&(my_msg_rank.length()%3!=0||my_msg_rank.length()<1))
            {
                QMessageBox::warning(this,"提示","录入信息异常，请检查！",QMessageBox::Yes);
                return ;
            }
            total_my_msg_rank.insert(ui->comboBox->currentText()+"_"+ui->comboBox_2->currentText(),my_msg_rank);
            my_msg_rank.clear();
            total_my_msg.append(my_msg);
            my_msg.clear();
            //保存到本地
            writeTotal("D:/work/savedata/total.txt",false);
            QMessageBox::warning(this,"提示","保存成功！",QMessageBox::Yes);


        }
        else
            return;
    }
    ui->comboBox->setCurrentIndex(0);
    ui->comboBox_2->setCurrentIndex(0);
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_pushButton_6_clicked()
{
    QMessageBox msgBox;
    msgBox.setText("温馨提示：");
    if(mode==1)
    {
        msgBox.setInformativeText("该工序样品是否已完成？");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Cancel);
        int ret = msgBox.exec();
        if(ret == QMessageBox::Ok)
        {

            if(my_msg_rank.length()==0)
            {
                QMessageBox::warning(this,"提示","已保存，不需要重复保存。",QMessageBox::Yes);
                ui->stackedWidget->setCurrentIndex(1);
                return;
            }
            else if(ui->comboBox->currentText()!="710E比对"&&ui->comboBox->currentText()!="烘箱比对"&&(my_msg_rank.length()%2!=0||my_msg_rank.length()<1))
            {
                QMessageBox::warning(this,"提示","录入信息异常，请检查！",QMessageBox::Yes);
                return ;
            }
            else if(ui->comboBox->currentText()=="710E比对"&&(my_msg_rank.length()%4!=0||my_msg_rank.length()<1))
            {
                QMessageBox::warning(this,"提示","录入信息异常，请检查！",QMessageBox::Yes);
                return ;
            }
            else if(ui->comboBox->currentText()=="烘箱比对"&&(my_msg_rank.length()%3!=0||my_msg_rank.length()<1))
            {
                QMessageBox::warning(this,"提示","录入信息异常，请检查！",QMessageBox::Yes);
                return ;
            }
            total_my_msg_rank.insert(ui->comboBox->currentText()+"_"+ui->comboBox_2->currentText(),my_msg_rank);
            my_msg_rank.clear();
            total_my_msg.append(my_msg);
            my_msg.clear();
            //保存到本地
            writeTotal("D:/work/savedata/total.txt",false);
            QMessageBox::warning(this,"提示","保存成功！",QMessageBox::Yes);


        }
        else
            return;
    }
    ui->stackedWidget->setCurrentIndex(1);

}

void MainWindow::on_pushButton_7_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_pushButton_2_clicked()
{
    QMessageBox msgBox;
    msgBox.setText("温馨提示：");
    if(mode==1)
    {
        msgBox.setInformativeText("该工序样品是否已完成？");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Cancel);
        int ret = msgBox.exec();
        if(ret == QMessageBox::Ok)
        {

            if(my_msg_rank.length()==0)
            {
                QMessageBox::warning(this,"提示","已保存，不需要重复保存。",QMessageBox::Yes);
                ui->stackedWidget->setCurrentIndex(3);
                return;
            }
            else if(ui->comboBox->currentText()!="710E比对"&&ui->comboBox->currentText()!="烘箱比对"&&(my_msg_rank.length()%2!=0||my_msg_rank.length()<1))
            {
                QMessageBox::warning(this,"提示","录入信息异常，请检查！",QMessageBox::Yes);
                return ;
            }
            else if(ui->comboBox->currentText()=="710E比对"&&(my_msg_rank.length()%4!=0||my_msg_rank.length()<1))
            {
                QMessageBox::warning(this,"提示","录入信息异常，请检查！",QMessageBox::Yes);
                return ;
            }
            else if(ui->comboBox->currentText()=="烘箱比对"&&(my_msg_rank.length()%3!=0||my_msg_rank.length()<1))
            {
                QMessageBox::warning(this,"提示","录入信息异常，请检查！",QMessageBox::Yes);
                return ;
            }
            total_my_msg_rank.insert(ui->comboBox->currentText()+"_"+ui->comboBox_2->currentText(),my_msg_rank);
            my_msg_rank.clear();
            total_my_msg.append(my_msg);
            my_msg.clear();
            //保存到本地
            writeTotal("D:/work/savedata/total.txt",false);
            QMessageBox::warning(this,"提示","保存成功！",QMessageBox::Yes);


        }
        else
            return;
    }
    ui->stackedWidget->setCurrentIndex(3);

}
void MainWindow::on_pushButton_14_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}
int nbcompare=0;//表示该组多少个平行样算一个平均值
int mode=0;//mode=1表示烘前录入，mode=2表示烘后计算
void MainWindow::on_pushButton_3_clicked()//烘前
{
    if(ui->comboBox_2->currentIndex()==0||ui->comboBox->currentIndex()==0)
    {
        QMessageBox::warning(this,"警告","请选择工序点和牌号！");
        return;
    }
    for(int i=0;i<total_my_msg.length();i++)
    {
        for (auto item = total_my_msg[i].begin(); item != total_my_msg[i].end(); ++item)
        {
            if(item.value()["牌号"]==ui->comboBox_2->currentText()&&item.value()["工序点"]==ui->comboBox->currentText())//找到了有
            {
                QMessageBox msgBox;
                msgBox.setText("温馨提示：");
                msgBox.setInformativeText("该牌号工序点已使用过,会覆盖原来的数据,确定继续使用吗？");
                msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                msgBox.setDefaultButton(QMessageBox::No);
                int ret = msgBox.exec();
                if(ret == QMessageBox::No)
                {
                    return;
                }
                break;
            }
        }
    }
    ui->label_35->setText(ui->comboBox_2->currentText());
    ui->label_39->setText(ui->comboBox->currentText());
    mode = 1;
    for(int i = ui->tableWidget->rowCount() - 1;i >= 0; i--)
    {
        ui->tableWidget->removeRow(i);
    }
    ui->stackedWidget->setCurrentIndex(2);
    if(ui->comboBox->currentText()=="烘箱比对")
        nbcompare=3;
    else if(ui->comboBox->currentText()=="710E比对")
        nbcompare=4;
    else
        nbcompare=2;

    my_msg_rank.clear();
    my_msg.clear();
}

void MainWindow::on_pushButton_4_clicked()//烘后
{
    mode = 2;
    ui->stackedWidget->setCurrentIndex(4);
    my_msg.clear();
    my_msg_rank.clear();
    update_TableWidget_2();
}
int cc=0;
void MainWindow::on_pushButton_9_clicked()//模拟发送信号
{
    cc++;
    emit signalQRCode("00889"+QString::number(cc));   //发送完整二维码数据信号
}

void MainWindow::on_pushButton_10_clicked()
{
    emit signalWeight("N -23.334 g");   //发送完整天平数据信号
}

void MainWindow::on_pushButton_11_clicked()
{
    emit signalWeight("N 5.088 g");   //发送完整天平数据信号
}
int ccc=0;
void MainWindow::on_pushButton_15_clicked()
{

    ccc++;
    emit signalQRCode("00889"+QString::number(ccc));   //发送完整二维码数据信号
}

void MainWindow::on_pushButton_13_clicked()//增加或减少combox内容
{
    if(ui->comboBox_9->currentIndex()==0)//工序点
    {
        QStringList itemsInComboBox;
        for (int index = 0; index < ui->comboBox_13->count(); index++)
            itemsInComboBox << ui->comboBox_13->itemText(index);
        if(ui->comboBox_10->currentIndex()==0)//增加内容
        {
            if(ui->lineEdit->text()!=NULL)
            {
                for(int i=0;i<itemsInComboBox.length();i++)
                {
                    if(itemsInComboBox[i]==ui->lineEdit->text())
                    {
                        QMessageBox::warning(this,"提示","该内容已存在！",QMessageBox::Yes);
                        return;
                    }
                }
                ui->comboBox_13->insertItem(ui->comboBox_13->count(),ui->lineEdit->text());
            }
            else
            {
                QMessageBox::warning(this,"提示","输入不能为空！",QMessageBox::Yes);
                return ;
            }
        }
        else//减少内容
        {
            ui->lineEdit->setText(ui->comboBox_11->currentText());
            for(int i=0;i<itemsInComboBox.length();i++)
            {
                if(itemsInComboBox[i]==ui->lineEdit->text())
                {
                    ui->comboBox_13->removeItem(i);
                    break;
                }
            }

            //关联绑定的也要删除掉
            for (auto item = combox.begin(); item != combox.end(); ++item)
            {
                for(int i=0;i<item.value().length();i++)
                {
                    if(item.value()[i]==ui->lineEdit->text())
                    {
                        item.value().removeAt(i);
                    }
                }
            }
            writeComD();

        }
        QMessageBox::warning(this,"提示","操作成功",QMessageBox::Yes);
    }
    else if(ui->comboBox_9->currentIndex()==1)//牌号
    {
        QStringList itemsInComboBox;
        for (int index = 0; index < ui->comboBox_2->count(); index++)
            itemsInComboBox << ui->comboBox_2->itemText(index);
        if(ui->comboBox_10->currentIndex()==0)//增加内容
        {
            if(ui->lineEdit->text()!=NULL)
            {
                for(int i=0;i<itemsInComboBox.length();i++)
                {
                    if(itemsInComboBox[i]==ui->lineEdit->text())
                    {
                        QMessageBox::warning(this,"提示","该内容已存在！",QMessageBox::Yes);
                        return;
                    }
                }
                ui->comboBox_2->insertItem(ui->comboBox_2->count(),ui->lineEdit->text());
            }
            else
            {
                QMessageBox::warning(this,"提示","输入不能为空！",QMessageBox::Yes);
                return ;
            }
        }
        else//减少内容
        {

            ui->lineEdit->setText(ui->comboBox_11->currentText());
            for(int i=0;i<itemsInComboBox.length();i++)
            {
                if(itemsInComboBox[i]==ui->lineEdit->text())
                {
                    ui->comboBox_2->removeItem(i);
                    break;
                }
            }
            //关联绑定的也要删除掉
            for (auto item = combox.begin(); item != combox.end(); ++item)
            {
                if(item.key()==ui->lineEdit->text())
                {
                    combox.remove(ui->lineEdit->text());
                }
            }
            writeComD();
        }
        QMessageBox::warning(this,"提示","操作成功",QMessageBox::Yes);
    }
    else if(ui->comboBox_9->currentIndex()==2)
    {
        QStringList itemsInComboBox;
        for (int index = 0; index < ui->comboBox_3->count(); index++)
            itemsInComboBox << ui->comboBox_3->itemText(index);
        if(ui->comboBox_10->currentIndex()==0)//增加内容
        {
            if(ui->lineEdit->text()!=NULL)
            {
                for(int i=0;i<itemsInComboBox.length();i++)
                {
                    if(itemsInComboBox[i]==ui->lineEdit->text())
                    {
                        QMessageBox::warning(this,"提示","该内容已存在！",QMessageBox::Yes);
                        return;
                    }
                }
                ui->comboBox_3->insertItem(ui->comboBox_3->count(),ui->lineEdit->text());
            }
            else
            {
                QMessageBox::warning(this,"提示","输入不能为空！",QMessageBox::Yes);
                return ;
            }
        }
        else//减少内容
        {
            ui->lineEdit->setText(ui->comboBox_11->currentText());
            for(int i=0;i<itemsInComboBox.length();i++)
            {
                if(itemsInComboBox[i]==ui->lineEdit->text())
                {
                    ui->comboBox_3->removeItem(i);
                    break;
                }
            }
        }
    }
    else if(ui->comboBox_9->currentIndex()==3)
    {
        QStringList itemsInComboBox;
        for (int index = 0; index < ui->comboBox_4->count(); index++)
            itemsInComboBox << ui->comboBox_4->itemText(index);
        if(ui->comboBox_10->currentIndex()==0)//增加内容
        {
            if(ui->lineEdit->text()!=NULL)
            {
                for(int i=0;i<itemsInComboBox.length();i++)
                {
                    if(itemsInComboBox[i]==ui->lineEdit->text())
                    {
                        QMessageBox::warning(this,"提示","该内容已存在！",QMessageBox::Yes);
                        return;
                    }
                }
                ui->comboBox_4->insertItem(ui->comboBox_4->count(),ui->lineEdit->text());
            }
            else
            {
                QMessageBox::warning(this,"提示","输入不能为空！",QMessageBox::Yes);
                return ;
            }
        }
        else//减少内容
        {
            ui->lineEdit->setText(ui->comboBox_11->currentText());
            for(int i=0;i<itemsInComboBox.length();i++)
            {
                if(itemsInComboBox[i]==ui->lineEdit->text())
                {
                    ui->comboBox_4->removeItem(i);
                    break;
                }
            }
        }
    }
    else if(ui->comboBox_9->currentIndex()==4)
    {
        QStringList itemsInComboBox;
        for (int index = 0; index < ui->comboBox_5->count(); index++)
            itemsInComboBox << ui->comboBox_5->itemText(index);
        if(ui->comboBox_10->currentIndex()==0)//增加内容
        {
            if(ui->lineEdit->text()!=NULL)
            {
                for(int i=0;i<itemsInComboBox.length();i++)
                {
                    if(itemsInComboBox[i]==ui->lineEdit->text())
                    {
                        QMessageBox::warning(this,"提示","该内容已存在！",QMessageBox::Yes);
                        return;
                    }
                }
                ui->comboBox_5->insertItem(ui->comboBox_5->count(),ui->lineEdit->text());
            }
            else
            {
                QMessageBox::warning(this,"提示","输入不能为空！",QMessageBox::Yes);
                return ;
            }
        }
        else//减少内容
        {
            ui->lineEdit->setText(ui->comboBox_11->currentText());
            for(int i=0;i<itemsInComboBox.length();i++)
            {
                if(itemsInComboBox[i]==ui->lineEdit->text())
                {
                    ui->comboBox_5->removeItem(i);
                    break;
                }
            }
        }
    }
    else if(ui->comboBox_9->currentIndex()==5)
    {
        QStringList itemsInComboBox;
        for (int index = 0; index < ui->comboBox_6->count(); index++)
            itemsInComboBox << ui->comboBox_6->itemText(index);
        if(ui->comboBox_10->currentIndex()==0)//增加内容
        {
            if(ui->lineEdit->text()!=NULL)
            {
                for(int i=0;i<itemsInComboBox.length();i++)
                {
                    if(itemsInComboBox[i]==ui->lineEdit->text())
                    {
                        QMessageBox::warning(this,"提示","该内容已存在！",QMessageBox::Yes);
                        return;
                    }
                }
                ui->comboBox_6->insertItem(ui->comboBox_6->count(),ui->lineEdit->text());
            }
            else
            {
                QMessageBox::warning(this,"提示","输入不能为空！",QMessageBox::Yes);
                return ;
            }
        }
        else//减少内容
        {
            ui->lineEdit->setText(ui->comboBox_11->currentText());
            for(int i=0;i<itemsInComboBox.length();i++)
            {
                if(itemsInComboBox[i]==ui->lineEdit->text())
                {
                    ui->comboBox_6->removeItem(i);
                    break;
                }
            }
        }
    }
    else if(ui->comboBox_9->currentIndex()==6)
    {
        QStringList itemsInComboBox;
        for (int index = 0; index < ui->comboBox_7->count(); index++)
            itemsInComboBox << ui->comboBox_7->itemText(index);
        if(ui->comboBox_10->currentIndex()==0)//增加内容
        {
            if(ui->lineEdit->text()!=NULL)
            {
                for(int i=0;i<itemsInComboBox.length();i++)
                {
                    if(itemsInComboBox[i]==ui->lineEdit->text())
                    {
                        QMessageBox::warning(this,"提示","该内容已存在！",QMessageBox::Yes);
                        return;
                    }
                }
                ui->comboBox_7->insertItem(ui->comboBox_7->count(),ui->lineEdit->text());
            }
            else
            {
                QMessageBox::warning(this,"提示","输入不能为空！",QMessageBox::Yes);
                return ;
            }
        }
        else//减少内容
        {
            ui->lineEdit->setText(ui->comboBox_11->currentText());
            for(int i=0;i<itemsInComboBox.length();i++)
            {
                if(itemsInComboBox[i]==ui->lineEdit->text())
                {
                    ui->comboBox_7->removeItem(i);
                    break;
                }
            }
        }
    }
    else if(ui->comboBox_9->currentIndex()==7)
    {
        QStringList itemsInComboBox;
        for (int index = 0; index < ui->comboBox_8->count(); index++)
            itemsInComboBox << ui->comboBox_8->itemText(index);
        if(ui->comboBox_10->currentIndex()==0)//增加内容
        {
            if(ui->lineEdit->text()!=NULL)
            {
                for(int i=0;i<itemsInComboBox.length();i++)
                {
                    if(itemsInComboBox[i]==ui->lineEdit->text())
                    {
                        QMessageBox::warning(this,"提示","该内容已存在！",QMessageBox::Yes);
                        return;
                    }
                }
                ui->comboBox_8->insertItem(ui->comboBox_8->count(),ui->lineEdit->text());
            }
            else
            {
                QMessageBox::warning(this,"提示","输入不能为空！",QMessageBox::Yes);
                return ;
            }
        }
        else//减少内容
        {
            ui->lineEdit->setText(ui->comboBox_11->currentText());
            for(int i=0;i<itemsInComboBox.length();i++)
            {
                if(itemsInComboBox[i]==ui->lineEdit->text())
                {
                    ui->comboBox_8->removeItem(i);
                    break;
                }
            }
        }
    }
    writeTxt();
    readTxt();

    int n = ui->comboBox_9->currentIndex();
    if(n==0)
    {
        ui->comboBox_9->setCurrentIndex(1);
        QThread::msleep(10);
        ui->comboBox_9->setCurrentIndex(n);
    }
    else
    {
        ui->comboBox_9->setCurrentIndex(0);
        QThread::msleep(10);
        ui->comboBox_9->setCurrentIndex(n);
    }

}



void MainWindow::on_pushButton_16_clicked()//清空数据
{

    QStringList itemsInComboBox;
    for (int index = 0; index < ui->comboBox_13->count(); index++)
        itemsInComboBox << ui->comboBox_13->itemText(index);
    emit sendCombox(itemsInComboBox);
    itemsInComboBox.clear();
    for (int index = 0; index < ui->comboBox_2->count(); index++)
        itemsInComboBox << ui->comboBox_2->itemText(index);
    emit sendCombox1(itemsInComboBox);
    form->show();

}

void MainWindow::on_pushButton_17_clicked()//导入数据
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("文件对话框！"),"D:/work/savedata/",
                                tr("本本文件(*txt)"));

    if(fileName.length()<1)return;
    my_msg_rank.clear();
    my_msg.clear();
    total_my_msg_rank.clear();
    total_my_msg.clear();
    readTotal(fileName);

    update_TableWidget_2();
}



void MainWindow::on_pushButton_19_clicked()//删除当前行
{
    int cur_row = ui->tableWidget->currentRow();
    if(cur_row==-1)
    {
        QMessageBox::warning(this,"提示","请先选中其中一行",QMessageBox::Yes);
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("温馨提示：");
        msgBox.setInformativeText("确实要删除该行吗?");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Cancel);
        int ret = msgBox.exec();
        if(ret == QMessageBox::Ok)
        {
            QString cur=my_msg_rank[cur_row];
            my_msg.remove(cur);
            my_msg_rank.removeAt(cur_row);
            update_TableWidget();
        }
    }

}

void MainWindow::on_pushButton_20_clicked()//上调该行
{
    int cur_row = ui->tableWidget->currentRow();
    if(cur_row==-1)
    {
        QMessageBox::warning(this,"提示","请先选中其中一行",QMessageBox::Yes);
    }
    else if(cur_row>=1)
    {
        QString tmp = my_msg_rank[cur_row-1];
        my_msg_rank[cur_row-1]=my_msg_rank[cur_row];
        my_msg_rank[cur_row]=tmp;
        update_TableWidget();
        ui->tableWidget->selectRow(cur_row-1);
    }
}

void MainWindow::on_pushButton_21_clicked()//下调该行
{
    int cur_row = ui->tableWidget->currentRow();
    if(cur_row==-1)
    {
        QMessageBox::warning(this,"提示","请先选中其中一行",QMessageBox::Yes);
    }
    else if(cur_row<my_msg_rank.length()-1)
    {
        QString tmp = my_msg_rank[cur_row+1];
        my_msg_rank[cur_row+1]=my_msg_rank[cur_row];
        my_msg_rank[cur_row]=tmp;
        update_TableWidget();
        ui->tableWidget->selectRow(cur_row+1);
    }
}

void MainWindow::on_comboBox_9_currentIndexChanged(int index)
{
    if(index==0)//工序点
    {
        ui->comboBox_11->clear();
        for (int i = 0; i < ui->comboBox->count(); i++)
            ui->comboBox_11->insertItem(i,ui->comboBox->itemText(i));
    }
    else if(index==1)//牌号
    {
        ui->comboBox_11->clear();
        for (int i = 0; i < ui->comboBox_2->count(); i++)
            ui->comboBox_11->insertItem(i,ui->comboBox_2->itemText(i));
    }
    else if(index==2)
    {
        ui->comboBox_11->clear();
        for (int i = 0; i < ui->comboBox_3->count(); i++)
            ui->comboBox_11->insertItem(i,ui->comboBox_3->itemText(i));
    }
    else if(index==3)
    {
        ui->comboBox_11->clear();
        for (int i = 0; i < ui->comboBox_4->count(); i++)
            ui->comboBox_11->insertItem(i,ui->comboBox_4->itemText(i));
    }
    else if(index==4)
    {
        ui->comboBox_11->clear();
        for (int i = 0; i < ui->comboBox_5->count(); i++)
            ui->comboBox_11->insertItem(i,ui->comboBox_5->itemText(i));
    }
    else if(index==5)
    {
        ui->comboBox_11->clear();
        for (int i = 0; i < ui->comboBox_6->count(); i++)
            ui->comboBox_11->insertItem(i,ui->comboBox_6->itemText(i));
    }
    else if(index==6)
    {
        ui->comboBox_11->clear();
        for (int i = 0; i < ui->comboBox_7->count(); i++)
            ui->comboBox_11->insertItem(i,ui->comboBox_7->itemText(i));
    }
    else if(index==7)
    {
        ui->comboBox_11->clear();
        for (int i = 0; i < ui->comboBox_8->count(); i++)
            ui->comboBox_11->insertItem(i,ui->comboBox_8->itemText(i));
    }
    else
        ui->comboBox_11->clear();
}


//实现鼠标拖拽移动
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_bDrag = true;
        //获得鼠标的初始位置
        mouseStartPoint = event->globalPos();

        //mouseStartPoint = event->pos();
        //获得窗口的初始位置
        windowTopLeftPoint = this->frameGeometry().topLeft();
        qDebug()<<mouseStartPoint<<windowTopLeftPoint;
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(m_bDrag)
    {	//这里if语句判断，让鼠标在特定控件上进行拖动才有效，比如自绘标题栏控件
        if(!ui->label_36->underMouse()||enable_move==false){
            return;
        }
        //获得鼠标移动的距离
        QPoint distance = event->globalPos() - mouseStartPoint;
        //QPoint distance = event->pos() - mouseStartPoint;
        //改变窗口的位置
        this->move(windowTopLeftPoint + distance);
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_bDrag = false;
    }
}

void MainWindow::getData(QStringList com)//删除工序点牌号
{
    if(com.length()<2||total_my_msg.length()<1)return;
    QStringList paihao;
    QStringList gongxudian;
    int ff=0;
    for(int i=0;i<com.length();i++)
    {
        if(com[i]=="%")
        {
            ff=i;
            break;
        }
    }
    for(int i=0;i<ff;i++)
    {
        paihao<<com[i];
    }
    for(int i=ff+1;i<com.length();i++)
    {
        gongxudian<<com[i];
    }
    qDebug()<<paihao<<gongxudian;
    //tcp共享数据
    QByteArray arr;
    arr.append("delete#");
    for(int i=0;i<paihao.length();i++)
    {
        arr.append(paihao[i]+"%");
    }
    arr.append("#");
    for(int i=0;i<gongxudian.length();i++)
    {
        arr.append(gongxudian[i]+"%");
    }
    //发送
    if(isConnect==true && mSocket->state()==QAbstractSocket::ConnectedState)
    {
        send_write_arr(0,arr);
    }

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
    update_TableWidget_2();
    writeTotal("D:/work/savedata/total.txt",false);

}



void MainWindow::on_pushButton_23_clicked()//单纯返回称量系统pushButton_18
{
    ui->pushButton_5->setDisabled(false);
    ui->pushButton_6->setDisabled(false);
    ui->pushButton_2->setDisabled(false);
    ui->pushButton_17->setDisabled(false);


    ui->pushButton->setDisabled(false);
    ui->stackedWidget->setCurrentIndex(2);
    update_TableWidget();
}

void MainWindow::on_pushButton_24_clicked()//前往计算模块
{
    ui->pushButton_5->setDisabled(true);
    ui->pushButton_6->setDisabled(true);
    ui->pushButton_2->setDisabled(true);
    ui->pushButton_17->setDisabled(true);


    ui->pushButton->setDisabled(true);

    ui->stackedWidget->setCurrentIndex(4);
    update_TableWidget_2();
}

QHash<QString,QStringList> combox;
void MainWindow::on_pushButton_25_clicked()//绑定牌号和工序点
{
    for (auto item = combox.begin(); item != combox.end(); ++item)
    {
        if(item.key()==ui->comboBox_12->currentText())
        {
            combox.remove(ui->comboBox_12->currentText());
        }
    }
    QStringList msg;
    msg.append("请选择...");
    msg.append(ui->comboBox_13->currentText());
    combox.insert(ui->comboBox_12->currentText(),msg);

    for (auto item = combox.begin(); item != combox.end(); ++item)
    {
        qDebug()<<item.key()<<item.value();
    }
    writeComD();
    QMessageBox::warning(this,"提示","绑定成功",QMessageBox::Yes);
}

void MainWindow::on_comboBox_2_currentTextChanged(const QString &arg1)
{//combox
    ui->comboBox->clear();
    bool flag=false;
    for (auto item = combox.begin(); item != combox.end(); ++item)
    {
        if(item.key()==arg1)flag=true;
    }
    if(!flag)//没找到，就直接全部
    {
        ui->comboBox->addItem("请选择...");
        for(int i=0;i<ui->comboBox_13->count();i++)
        {
            ui->comboBox->addItem(ui->comboBox_13->itemText(i));
        }
    }
    else
    {
        for(int i=0;i<combox[arg1].length();i++)
        {
            ui->comboBox->addItem(combox[arg1][i]);
        }
    }
}






void MainWindow::on_comboBox_12_currentIndexChanged(int index)//关联绑定显示
{
    ui->comboBox_13->selectModel->clear();
    QStringList selects = combox[ui->comboBox_12->currentText()];
    int pcount = ui->comboBox_13->popupModel->rowCount();
    QList<int> selectindex;
    for(int j=0;j<pcount;j++)//先全部取消选中
    {
        QStandardItem* clickItem=ui->comboBox_13->popupModel->item(j);
        clickItem->setCheckState(Qt::Unchecked);
    }

    for(int i=0;i<selects.length();i++)
    {
        for(int j=0;j<pcount;j++)
        {
            QStandardItem* clickItem=ui->comboBox_13->popupModel->item(j);
            QString text=clickItem->text();
            if(selects[i]==text)
            {
                selectindex.append(j);
                clickItem->setCheckState(Qt::Checked);
                QStandardItem* item=new QStandardItem(QIcon("://del.png"),selects[i]);
                ui->comboBox_13->selectModel->appendRow(item);
            }
        }

    }
}
