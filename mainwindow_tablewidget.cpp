#include "mainwindow.h"
#include "ui_mainwindow.h"
//表格操作相关的代码

void MainWindow::TableWidgetInit()
{

    // 初始化tablewidget

    QStringList listHeader;
    listHeader <<"样品序号"<<"容器重量(g)"<<"烘前重量(g)";
    ui->tableWidget->setColumnCount(listHeader.count());
    ui->tableWidget->setHorizontalHeaderLabels(listHeader);
    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color:#285a70;color:#ffffff;}");
    ui->tableWidget->horizontalHeader()->setFont(QFont("song", 20));

    ui->tableWidget->setStyleSheet("QScrollBar::vertical{width:16px;border:1px solid #86b9d1;"
                                     "padding-top:16px;padding-bottom:16px;background-color: #86b9d1;}QScrollBar::horizontal{"
                                                        "border: 1px solid #86b9d1;background-color:#e6f8ff;height: 16px;}");

    for(int i=0;i<listHeader.length();i++)
        ui->tableWidget->setColumnWidth(i,350);

    ui->tableWidget_2->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);


    //ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->setEditTriggers(QAbstractItemView::DoubleClicked);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    listHeader.clear();
    listHeader <<"工序点"<<"牌号"<<"样品序号"<<"仪表测试值(%)"<<"容器重量"<<"烘前重量"<<"烘后重量"<<"样品水分(%)"<<"烘箱测试值(%)";//<<"总平均水分(%)";
    ui->tableWidget_2->setColumnCount(listHeader.count());
    ui->tableWidget_2->setHorizontalHeaderLabels(listHeader);
    ui->tableWidget_2->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color:#285a70;color:#ffffff;}");
    ui->tableWidget_2->horizontalHeader()->setFont(QFont("song", 12));

    ui->tableWidget_2->setStyleSheet("QScrollBar::vertical{width:16px;border:1px solid #86b9d1;"
                                     "padding-top:16px;padding-bottom:16px;background-color: #86b9d1;}QScrollBar::horizontal{"
                                                        "border: 1px solid #86b9d1;background-color:#e6f8ff;height: 16px;}");

    for(int i=0;i<listHeader.length();i++)
            ui->tableWidget_2->setColumnWidth(i,120);

    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //ui->tableWidget_2->verticalHeader()->hide();
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::DoubleClicked);
    ui->tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);


}

void MainWindow::update_TableWidget()
{
    TableWidgetInit();
    for(int i = ui->tableWidget->rowCount() - 1;i >= 0; i--)
    {
        ui->tableWidget->removeRow(i);
    }

    int row =0;
    for(int i=0;i<my_msg.size();i++)//因为hash表是乱的，确保显示的顺序是录入的顺序
    {
        for (auto item = my_msg.begin(); item != my_msg.end(); ++item)
        {
            if(item.key()!=my_msg_rank[i])continue;//莫名其妙会出现空，将其删除
            ui->tableWidget->insertRow(row);

            ui->tableWidget->setItem(row, 0, new QTableWidgetItem(item.key()));
            ui->tableWidget->item(row,0)->setTextAlignment(Qt::AlignCenter | Qt::AlignHCenter);
            ui->tableWidget->item(row,0)->setBackgroundColor(QColor(233,233,233));

            for (auto item1 = item.value().begin(); item1 != item.value().end(); ++item1)
            {
                QString key,value;
                key = item1.key();//"样品序号"<<"容器重量(g)"<<"烘前重量(g)""
                value = item1.value();
                if(key=="容器重量")
                {
                    ui->tableWidget->setItem(row, 1, new QTableWidgetItem(value));
                    ui->tableWidget->item(row,1)->setTextAlignment(Qt::AlignCenter | Qt::AlignHCenter);
                    ui->tableWidget->item(row,1)->setBackgroundColor(QColor(233,233,233));
                }
                else if(key=="烘前重量")
                {
                    ui->tableWidget->setItem(row, 2, new QTableWidgetItem(value));
                    ui->tableWidget->item(row,2)->setTextAlignment(Qt::AlignCenter | Qt::AlignHCenter);
                    ui->tableWidget->item(row,2)->setBackgroundColor(QColor(233,233,233));
                }
            }
            row++;
        }

    }
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    QFont font;
    font.setPixelSize(30);
    font.setBold(true);
    ui->tableWidget->verticalHeader()->setFont(font);
    //显示最后一行
       ui->tableWidget->scrollToBottom();
}

void MainWindow::on_pushButton_8_clicked()//保存当前录入
{
    //检查录入的重量是否合理
    bool isgood=true;
    for (auto item = my_msg.begin(); item != my_msg.end(); ++item)
    {
        if(item.value()["容器重量"]=="0"||item.value()["烘前重量"].toDouble()<4||item.value()["烘前重量"].toDouble()>6)//有问题
        {
            isgood = false;
            for(int i=0;i<my_msg_rank.length();i++)
            {
                if(my_msg_rank[i]==item.key())
                {
                    if(item.value()["容器重量"]=="0")
                    {
                        ui->tableWidget->item(i,1)->setBackgroundColor(QColor(255,0,0));
                    }
                    if(item.value()["烘前重量"].toDouble()<4||item.value()["烘前重量"].toDouble()>6)
                    {
                        ui->tableWidget->item(i,2)->setBackgroundColor(QColor(255,0,0));
                    }
                }
            }
        }
    }
    if(isgood==false)//有问题，直接标红并返回
    {
        QMessageBox::warning(this,"提示","数据录入异常，请检查",QMessageBox::Yes);
        return;
    }
    //检查录入数量的合理性，如有问题，直接返回。
    if(my_msg_rank.length()==0)
    {
        QMessageBox::warning(this,"提示","已保存，不需要重复保存。",QMessageBox::Yes);
        return ;
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

    QMessageBox msgBox;
    msgBox.setText("温馨提示：");
    msgBox.setInformativeText("是否已全部录入完毕，确认保存？");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    int ret = msgBox.exec();
    if(ret == QMessageBox::Cancel)
    {
        return;
    }

    //检查同牌号工序点是否存在，存在就删掉
    int flag=0;
    for(int i=0;i<total_my_msg.length();i++)
    {
        for (auto item = total_my_msg[i].begin(); item != total_my_msg[i].end(); ++item)
        {
            if(item.value()["牌号"]==ui->comboBox_2->currentText()&&item.value()["工序点"]==ui->comboBox->currentText())//找到了有
            {
                total_my_msg.removeAt(i);
                total_my_msg_rank.remove(ui->comboBox->currentText()+"_"+ui->comboBox_2->currentText());
                flag=1;
                break;
            }
        }
        if(flag==1)break;
    }

    total_my_msg_rank.insert(ui->comboBox->currentText()+"_"+ui->comboBox_2->currentText(),my_msg_rank);
    total_my_msg.append(my_msg);


    //保存到本地
    writeTotal("D:/work/savedata/total.txt",false);
    writeTotal("D:/work/savedata/data.txt",true);
    QMessageBox::warning(this,"提示","保存成功！",QMessageBox::Yes);

    //需要与另一台电脑共享数据
    //tcp共享数据
    QByteArray arr;
    arr.append("add#");


    for (auto item = my_msg.begin(); item != my_msg.end(); ++item)
    {
        arr.append(item.key()+"%"+item.value()["工序点"]+"%"+item.value()["牌号"]+"%"+item.value()["容器重量"]+"%"
                +item.value()["烘前重量"]+"%"+item.value()["nbcompare"]+"#");
    }
    arr.append("rank#");
    arr.append(ui->comboBox->currentText()+"_"+ui->comboBox_2->currentText()+"#");
    for(int m=0;m<my_msg_rank.length();m++)
    {
        arr.append(my_msg_rank[m]+"%");
    }
    //发送
    if(isConnect==true && mSocket->state()==QAbstractSocket::ConnectedState)
    {
        send_write_arr(500,arr);
    }
}

void MainWindow::on_pushButton_12_clicked()//取消返回重选
{
    QMessageBox msgBox;
    msgBox.setText("温馨提示：");
    msgBox.setInformativeText("是否保存并确定退出？");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::No| QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    int ret = msgBox.exec();
    if(ret == QMessageBox::Ok)
    {
//************************和保存的代码一样*****************************************************
//        QMessageBox msgBox;
//        msgBox.setText("温馨提示：");
//        msgBox.setInformativeText("是否已全部录入完毕，确认保存？");
//        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
//        msgBox.setDefaultButton(QMessageBox::Cancel);
//        int ret = msgBox.exec();
//        if(ret == QMessageBox::Cancel)
//        {
//            return;
//        }

        if(my_msg_rank.length()==0)
        {
            QMessageBox::warning(this,"提示","已保存，不需要重复保存。",QMessageBox::Yes);
            return ;
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
        //检查同牌号工序点是否存在，存在就删掉
        int flag=0;
        for(int i=0;i<total_my_msg.length();i++)
        {
            for (auto item = total_my_msg[i].begin(); item != total_my_msg[i].end(); ++item)
            {
                if(item.value()["牌号"]==ui->comboBox_2->currentText()&&item.value()["工序点"]==ui->comboBox->currentText())//找到了有
                {
                    total_my_msg.removeAt(i);
                    total_my_msg_rank.remove(ui->comboBox->currentText()+"_"+ui->comboBox_2->currentText());
                    flag=1;
                    break;
                }
            }
            if(flag==1)break;
        }

        total_my_msg_rank.insert(ui->comboBox->currentText()+"_"+ui->comboBox_2->currentText(),my_msg_rank);
        total_my_msg.append(my_msg);

        //保存到本地
        writeTotal("D:/work/savedata/total.txt",false);
        writeTotal("D:/work/savedata/data.txt",true);
        QMessageBox::warning(this,"提示","保存成功！",QMessageBox::Yes);
        //tcp共享数据
        QByteArray arr;
        arr.append("add#");


        for (auto item = my_msg.begin(); item != my_msg.end(); ++item)
        {
            arr.append(item.key()+"%"+item.value()["工序点"]+"%"+item.value()["牌号"]+"%"+item.value()["容器重量"]+"%"
                    +item.value()["烘前重量"]+"%"+item.value()["nbcompare"]+"#");
        }
        arr.append("rank#");
        arr.append(ui->comboBox->currentText()+"_"+ui->comboBox_2->currentText()+"#");
        for(int m=0;m<my_msg_rank.length();m++)
        {
            arr.append(my_msg_rank[m]+"%");
        }

//&%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        my_msg.clear();
        my_msg_rank.clear();
        mode=0;
        ui->comboBox->setCurrentIndex(0);
        ui->comboBox_2->setCurrentIndex(0);
        ui->stackedWidget->setCurrentIndex(0);

        //发送
        if(isConnect==true && mSocket->state()==QAbstractSocket::ConnectedState)
        {
            send_write_arr(0,arr);
        }
    }
    else if(ret == QMessageBox::No)
    {
        my_msg.clear();
        my_msg_rank.clear();
        mode=0;
        ui->comboBox->setCurrentIndex(0);
        ui->comboBox_2->setCurrentIndex(0);
        ui->stackedWidget->setCurrentIndex(0);
    }
}
QList<rowmsg> row_msg;
rowmsg row_msg_;
void MainWindow::update_TableWidget_2()
{
    QColor co1=QColor(0,255,255);
    QColor co2=QColor(255,255,0);
    TableWidgetInit();
    for(int i = ui->tableWidget_2->rowCount() - 1;i >= 0; i--)
    {
        ui->tableWidget_2->removeRow(i);
    }
    row_msg.clear();
    int row =0;//total_my_msg
    for(int j=0;j<total_my_msg.size();j++)//因为hash表是乱的，确保显示的顺序是录入的顺序
    {
        for(int i=0;i<total_my_msg[j].size();i++)//因为hash表是乱的，确保显示的顺序是录入的顺序
        {
            row_msg_.count=total_my_msg[j].size();
            for (auto item = total_my_msg[j].begin(); item != total_my_msg[j].end(); ++item)
            {
                if(item.key()!=total_my_msg_rank[item.value()["工序点"]+"_"+item.value()["牌号"]][i])continue;//编号
                ui->tableWidget_2->insertRow(row);
                ui->tableWidget_2->setItem(row, 2, new QTableWidgetItem(item.key()));
                ui->tableWidget_2->item(row,2)->setTextAlignment(Qt::AlignCenter | Qt::AlignHCenter);
                if(j%2==0)
                    ui->tableWidget_2->item(row,2)->setBackgroundColor(co1);
                else
                    ui->tableWidget_2->item(row,2)->setBackgroundColor(co2);
                for (auto item1 = item.value().begin(); item1 != item.value().end(); ++item1)
                {
                    QString key,value;
                    key = item1.key();//"样品序号"<<"仪表测试值(%)"<<"容器重量"<<"烘前重量"<<"烘后重量"<<"样品水分(%)"<<"烘箱测试值(%)<<"总平均水分(%)"
                    value = item1.value();
                    if(key=="工序点")
                    {
                        ui->tableWidget_2->setItem(row, 0, new QTableWidgetItem(value));
                        ui->tableWidget_2->item(row,0)->setTextAlignment(Qt::AlignCenter | Qt::AlignHCenter);
                        if(j%2==0)
                            ui->tableWidget_2->item(row,0)->setBackgroundColor(co1);
                        else
                            ui->tableWidget_2->item(row,0)->setBackgroundColor(co2);
                    }
                    else if(key=="牌号")
                    {
                        ui->tableWidget_2->setItem(row, 1, new QTableWidgetItem(value));
                        ui->tableWidget_2->item(row,1)->setTextAlignment(Qt::AlignCenter | Qt::AlignHCenter);
                        if(j%2==0)
                            ui->tableWidget_2->item(row,1)->setBackgroundColor(co1);
                        else
                            ui->tableWidget_2->item(row,1)->setBackgroundColor(co2);
                    }
                    else if(key=="容器重量")
                    {
                        ui->tableWidget_2->setItem(row, 4, new QTableWidgetItem(value));
                        ui->tableWidget_2->item(row,4)->setTextAlignment(Qt::AlignCenter | Qt::AlignHCenter);
                        if(j%2==0)
                            ui->tableWidget_2->item(row,4)->setBackgroundColor(co1);
                        else
                            ui->tableWidget_2->item(row,4)->setBackgroundColor(co2);
                    }
                    else if(key=="烘前重量")
                    {
                        ui->tableWidget_2->setItem(row, 5, new QTableWidgetItem(value));
                        ui->tableWidget_2->item(row,5)->setTextAlignment(Qt::AlignCenter | Qt::AlignHCenter);
                        if(j%2==0)
                            ui->tableWidget_2->item(row,5)->setBackgroundColor(co1);
                        else
                            ui->tableWidget_2->item(row,5)->setBackgroundColor(co2);
                    }
                    else if(key=="烘后重量")
                    {
                        ui->tableWidget_2->setItem(row, 6, new QTableWidgetItem(value));
                        ui->tableWidget_2->item(row,6)->setTextAlignment(Qt::AlignCenter | Qt::AlignHCenter);
                        if(j%2==0)
                            ui->tableWidget_2->item(row,6)->setBackgroundColor(co1);
                        else
                            ui->tableWidget_2->item(row,6)->setBackgroundColor(co2);
                    }
                    else if(key=="样品水分")
                    {
                        ui->tableWidget_2->setItem(row, 7, new QTableWidgetItem(value));
                        ui->tableWidget_2->item(row,7)->setTextAlignment(Qt::AlignCenter | Qt::AlignHCenter);
                        if(j%2==0)
                            ui->tableWidget_2->item(row,7)->setBackgroundColor(co1);
                        else
                            ui->tableWidget_2->item(row,7)->setBackgroundColor(co2);
                    }
                    else if(key=="烘箱测试值")
                    {
                        ui->tableWidget_2->setItem(row, 8, new QTableWidgetItem(value));
                        ui->tableWidget_2->item(row,8)->setTextAlignment(Qt::AlignCenter | Qt::AlignHCenter);
                        if(j%2==0)
                            ui->tableWidget_2->item(row,8)->setBackgroundColor(co1);
                        else
                            ui->tableWidget_2->item(row,8)->setBackgroundColor(co2);
                    }
                    else if(key=="总平均水分")
                    {
//                        ui->tableWidget_2->setItem(row, 9, new QTableWidgetItem(value));
//                        ui->tableWidget_2->item(row,9)->setTextAlignment(Qt::AlignCenter | Qt::AlignHCenter);
//                        ui->tableWidget_2->item(row,9)->setBackgroundColor(QColor(0,255,255));
                    }
                    else if(key=="表显值")
                    {
                        ui->tableWidget_2->setItem(row, 3, new QTableWidgetItem(value));
                        ui->tableWidget_2->item(row,3)->setTextAlignment(Qt::AlignCenter | Qt::AlignHCenter);
                        if(j%2==0)
                            ui->tableWidget_2->item(row,3)->setBackgroundColor(co1);
                        else
                            ui->tableWidget_2->item(row,3)->setBackgroundColor(co2);
                    }
                    else if(key=="nbcompare")
                    {
                        row_msg_.jiange=value.toInt();
                    }

                }
                row++;
            }

        }
        row_msg.append(row_msg_);
    }
    QFont font;
    font.setPixelSize(30);
    font.setBold(true);
    ui->tableWidget_2->verticalHeader()->setFont(font);
//合并单元格
    int row_count=0;
    for(int i=0;i<row_msg.length();i++)
    {
        if(row_msg[i].count==0||row_msg[i].jiange==0)continue;
        for(int j=0;j<row_msg[i].count/row_msg[i].jiange;j++)
        {
            ui->tableWidget_2->setSpan(row_count+j*row_msg[i].jiange,8,row_msg[i].jiange,1);
            ui->tableWidget_2->setSpan(row_count+j*row_msg[i].jiange,3,row_msg[i].jiange,1);
        }
        row_count+=row_msg[i].count;
    }
}
void MainWindow::on_pushButton_clicked()
{
    double time_Start = (double)clock();
    static double time_last=0;
    if(time_Start-time_last>1000)
    {
        writeTotal("D:/work/savedata/data.txt",true);

    }
    time_last=time_Start;

    calc();
qDebug()<<"sssssssss";
    panduan();
qDebug()<<"sssssssss";
    writeTotal("D:/work/savedata/total.txt",false);
 //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%和导出excel按钮的代码一样%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    if(ui->tableWidget_2->rowCount()<=0)
    {
        QMessageBox::warning(this, "Warning", "数据为空");
        return;
    }
    QString name = ui->tableWidget_2->item(0,1)->text()+"_"+ui->tableWidget_2->item(0,0)->text();
    QString fileName = QFileDialog::getSaveFileName(this,
            "请选择保存路径",
            "C:/Users/Administrator/Desktop/data/"+name+"_"+QDate::currentDate().toString(tr("yyyy-MM-dd ")) + QTime::currentTime().toString(tr("hh.mm.ss")) + ".xlsx",
            tr("(*.xlsx)"));
    if(fileName.isEmpty()) {
        QMessageBox::warning(this, "Warning", "所选路径无效");
        return;
    }

    sonThread *sonthread = new sonThread;  //创建子线程对象
    sonthread->table=ui->tableWidget_2;  //将主界面UI指针赋给子线程中的指针对象
    sonthread->name = fileName;
    sonthread->isAll = 1;//全部导出
    sonthread->start();  //启动子线程

}

void MainWindow::calc()
{
    int row_count=0;
    double sum=0;
    for(int k=0;k<ui->tableWidget_2->rowCount();k++)
    {
        double qian = ui->tableWidget_2->item(k,5)->text().toDouble();
        double hou = ui->tableWidget_2->item(k,6)->text().toDouble();
        ui->tableWidget_2->setItem(k,7, new QTableWidgetItem(QString::number(100*(qian-hou)/qian,'f',2)+"%"));
    }

    for(int i=0;i<row_msg.length();i++)
    {
        qDebug()<<row_msg[i].count<<row_msg[i].jiange;
        if(row_msg[i].count==0||row_msg[i].jiange==0)continue;
        for(int j=0;j<row_msg[i].count/row_msg[i].jiange;j++)
        {
            ui->tableWidget_2->setSpan(row_count+j*row_msg[i].jiange,8,row_msg[i].jiange,1);
            ui->tableWidget_2->setSpan(row_count+j*row_msg[i].jiange,3,row_msg[i].jiange,1);
            //计算平均值
            sum=0;
            for(int m=0;m<row_msg[i].jiange;m++)
            {
                sum += ui->tableWidget_2->item(row_count+j*row_msg[i].jiange+m,7)->text().split('%')[0].toDouble();
            }
            ui->tableWidget_2->setItem(row_count+j*row_msg[i].jiange,8, new QTableWidgetItem(QString::number(sum/row_msg[i].jiange,'f',2)+"%"));
        }
//        ui->tableWidget_2->setSpan(row_count,9,row_msg[i].count,1);
//        sum=0;
//        for(int m=0;m<row_msg[i].count;m++)
//            sum += ui->tableWidget_2->item(row_count+m,7)->text().split('%')[0].toDouble();
//        ui->tableWidget_2->setItem(row_count,9, new QTableWidgetItem(QString::number(sum/row_msg[i].count,'f',2)+"%"));
        row_count+=row_msg[i].count;

    }
}

void MainWindow::panduan()
{
    int row_count=0;
    for(int i=0;i<row_msg.length();i++)
    {
        if(row_msg[i].count==0||row_msg[i].jiange==0)continue;
        if(row_msg[i].jiange==2)
        {
            for(int j=0;j<row_msg[i].count/row_msg[i].jiange;j++)
            {
                double aa = ui->tableWidget_2->item(row_count+j*2,7)->text().split('%')[0].toDouble();
                double bb = ui->tableWidget_2->item(row_count+j*2+1,7)->text().split('%')[0].toDouble();
                if(fabs(aa-bb)>0.3)//报警
                {
                    ui->tableWidget_2->item(row_count+j*2,7)->setBackgroundColor(QColor(255,0,0));
                    ui->tableWidget_2->item(row_count+j*2+1,7)->setBackgroundColor(QColor(255,0,0));
                }
            }

        }
        else if(row_msg[i].jiange==3)
        {
            for(int j=0;j<row_msg[i].count/row_msg[i].jiange;j++)
            {
                double aa = ui->tableWidget_2->item(row_count+j*3,7)->text().split('%')[0].toDouble();
                double bb = ui->tableWidget_2->item(row_count+j*3+1,7)->text().split('%')[0].toDouble();
                double cc = ui->tableWidget_2->item(row_count+j*3+2,7)->text().split('%')[0].toDouble();
                if(fabs(aa-bb)>0.3||fabs(aa-cc)>0.3||fabs(cc-bb)>0.3)//报警
                {
                    ui->tableWidget_2->item(row_count+j*3,7)->setBackgroundColor(QColor(255,0,0));
                    ui->tableWidget_2->item(row_count+j*3+1,7)->setBackgroundColor(QColor(255,0,0));
                    ui->tableWidget_2->item(row_count+j*3+2,7)->setBackgroundColor(QColor(255,0,0));
                }
            }
        }
        else if(row_msg[i].jiange==4)
        {
            for(int j=0;j<row_msg[i].count/row_msg[i].jiange;j++)
            {
                double aa = ui->tableWidget_2->item(row_count+j*4,7)->text().split('%')[0].toDouble();
                double bb = ui->tableWidget_2->item(row_count+j*4+1,7)->text().split('%')[0].toDouble();
                double cc = ui->tableWidget_2->item(row_count+j*4+2,7)->text().split('%')[0].toDouble();
                double dd = ui->tableWidget_2->item(row_count+j*4+3,7)->text().split('%')[0].toDouble();
                if(fabs(aa-bb)>0.3||fabs(aa-cc)>0.3||fabs(aa-dd)>0.3||fabs(bb-cc)>0.3||fabs(bb-dd)>0.3||fabs(cc-dd)>0.3)//报警
                {
                    ui->tableWidget_2->item(row_count+j*4,7)->setBackgroundColor(QColor(255,0,0));
                    ui->tableWidget_2->item(row_count+j*4+1,7)->setBackgroundColor(QColor(255,0,0));
                    ui->tableWidget_2->item(row_count+j*4+2,7)->setBackgroundColor(QColor(255,0,0));
                    ui->tableWidget_2->item(row_count+j*4+3,7)->setBackgroundColor(QColor(255,0,0));
                }

            }
        }
        row_count+=row_msg[i].count;
    }
}


void MainWindow::on_pushButton_18_clicked()
{
    if(ui->tableWidget_2->rowCount()<=0)
    {
        QMessageBox::warning(this, "Warning", "数据为空");
        return;
    }
    QString name = ui->tableWidget_2->item(0,1)->text()+"_"+ui->tableWidget_2->item(0,0)->text();
    QString fileName = QFileDialog::getSaveFileName(this,
            "请选择保存路径",
            "C:/Users/Administrator/Desktop/data/"+name+"_"+QDate::currentDate().toString(tr("yyyy-MM-dd ")) + QTime::currentTime().toString(tr("hh.mm.ss")) + ".xlsx",
            tr("(*.xlsx)"));
    if(fileName.isEmpty()) {
        QMessageBox::warning(this, "Warning", "所选路径无效");
        return;
    }

    sonThread *sonthread = new sonThread;  //创建子线程对象
    sonthread->table=ui->tableWidget_2;  //将主界面UI指针赋给子线程中的指针对象
    sonthread->name = fileName;
    sonthread->isAll = 1;//全部导出
    sonthread->start();  //启动子线程
}


void MainWindow::on_pushButton_27_clicked()//删除选中
{
try {


    int select = ui->tableWidget_2->currentRow();
    if(select<0)
    {
        QMessageBox::warning(this, "Warning", "请先选中一行");
        return;
    }
    if(select<0||select>ui->tableWidget_2->rowCount()-1)return;//防止超出范围

    QString gongX = ui->tableWidget_2->item(select,0)->text();
    QString paiH = ui->tableWidget_2->item(select,1)->text();
    qDebug()<<paiH<<gongX;

    //添加 如果样品水分和烘后水分无数据，就提示删除样品无效
    for(int i=total_my_msg.length()-1;i>=0;i--)
    {
        for (auto item = total_my_msg[i].begin(); item != total_my_msg[i].end(); ++item)
        {
            if(item.value()["牌号"]==paiH&&item.value()["工序点"]==gongX)
            {
                if(item.value()["烘后重量"].toDouble()<1.0)
                {
                    qDebug()<<item.value()["烘后重量"].toDouble();

                    QMessageBox::warning(this,"提示","删除样品无效",QMessageBox::Yes);
                    return;
                }
            }
        }
    }




    QMessageBox msgBox;
    msgBox.setText("温馨提示：");
    msgBox.setInformativeText("是否确认删除？");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    int ret = msgBox.exec();
    if(ret == QMessageBox::Ok)
    {

        //tcp共享数据
        QByteArray arr;
        arr.append("delete#");
        arr.append(paiH+"%");
        arr.append("#");
        arr.append(gongX+"%");

        bool flag=false;
        for(int i=total_my_msg.length()-1;i>=0;i--)
        {
            for (auto item = total_my_msg[i].begin(); item != total_my_msg[i].end(); ++item)
            {
                if(item.value()["牌号"]==paiH&&item.value()["工序点"]==gongX)
                    {
                        total_my_msg.removeAt(i);
                        total_my_msg_rank.remove(item.value()["工序点"]+"_"+item.value()["牌号"]);
                        flag=true;
                        break;
                    }
            }
            if(flag)break;
        }

        update_TableWidget_2();
        calc();
        writeTotal("D:/work/savedata/total.txt",false);
        //发送
        if(isConnect==true && mSocket->state()==QAbstractSocket::ConnectedState)
        {
            send_write_arr(50,arr);
        }
    }
    } catch (...) {
        QMessageBox::warning(this,"提示","删除数据异常，请重试！",QMessageBox::Yes);
    }
}

void MainWindow::on_tableWidget_2_cellChanged(int row, int column)
{
    double time_Start = (double)clock();
    static double time_last=0;
    if(time_Start-time_last>800)
    {

        if(column==3||column==4||column==5||column==6)
        {
            QString bianH = ui->tableWidget_2->item(row,2)->text();
            for(int j=0;j<total_my_msg.size();j++)//因为hash表是乱的，确保显示的顺序是录入的顺序
            {
                for (auto item = total_my_msg[j].begin(); item != total_my_msg[j].end(); ++item)
                {
                    if(item.key()==bianH)
                    {
                        item.value()["表显值"]=ui->tableWidget_2->item(row,3)->text();
                        item.value()["容器重量"]=ui->tableWidget_2->item(row,4)->text();
                        item.value()["烘前重量"]=ui->tableWidget_2->item(row,5)->text();
                        item.value()["烘后重量"]=ui->tableWidget_2->item(row,6)->text();
                        double w_qian = item.value()["烘前重量"].toDouble();
                        double w_hou = item.value()["烘后重量"].toDouble();
                        item.value()["样品水分"]=QString::number(100*(w_qian-w_hou)/w_qian,'f',2)+"%";
                        ui->tableWidget_2->item(row,7)->setText(item.value()["样品水分"]);
                        writeTotal("D:/work/savedata/total.txt",false);
                        //tcp共享数据

                        QByteArray arr;
                        if(column==3)
                        {
                            arr.append("change#"+item.key()+"%"+item.value()["烘后重量"]+"%"+QString::number(100*(w_qian-w_hou)/w_qian,'f',2)+
                                    "%"+item.value()["容器重量"]+"%"+item.value()["烘前重量"]+"%"+item.value()["表显值"]);
                        }
                        else
                        {
                            arr.append("change#"+item.key()+"%"+item.value()["烘后重量"]+"%"+QString::number(100*(w_qian-w_hou)/w_qian,'f',2)+
                                    "%"+item.value()["容器重量"]+"%"+item.value()["烘前重量"]+"%999");
                        }

                        qDebug()<<ui->tableWidget_2->item(row,3)->text()<<arr;
                        //发送
                        if(isConnect==true && mSocket->state()==QAbstractSocket::ConnectedState)
                        {
                            if(column==6)
                                send_write_arr(500,arr);
                            else
                                send_write_arr(0,arr);
                        }
                        break;
                    }
                }
            }
        }

    }
    time_last=time_Start;
}

void MainWindow::on_tableWidget_cellChanged(int row, int column)
{
    double time_Start = (double)clock();
    static double time_last=0;
    if(time_Start-time_last>800)
    {
        if(column==2)
        {
            double zhongliang = ui->tableWidget->item(row,column)->text().toDouble();
            if(zhongliang<4.0||zhongliang>6.0)
            {
                ui->tableWidget->item(row,column)->setBackgroundColor(QColor(255,0,0));
            }
        }
    }
    time_last=time_Start;
}
