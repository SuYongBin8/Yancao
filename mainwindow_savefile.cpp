#include "mainwindow.h"
#include "ui_mainwindow.h"

//保存文件
void MainWindow::writeTxt()
{

    QString txtpath="D:/work/savedata";//QDir::currentPath();
    // 文件位置
    QFile file(txtpath+"/combox.txt");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
        return;
    }
    // 文件流
    QTextStream stream(&file);
    // 输入内容
    stream << "combox1\n";
    for(int i=0;i<ui->comboBox_13->count();i++)
        stream << ui->comboBox_13->itemText(i)<<"\n";
    stream << "combox2\n";
    for(int i=0;i<ui->comboBox_2->count();i++)
        stream << ui->comboBox_2->itemText(i)<<"\n";
    stream << "combox3\n";
    for(int i=0;i<ui->comboBox_3->count();i++)
        stream << ui->comboBox_3->itemText(i)<<"\n";
    stream << "combox4\n";
    for(int i=0;i<ui->comboBox_4->count();i++)
        stream << ui->comboBox_4->itemText(i)<<"\n";
    stream << "combox5\n";
    for(int i=0;i<ui->comboBox_5->count();i++)
        stream << ui->comboBox_5->itemText(i)<<"\n";
    stream << "combox6\n";
    for(int i=0;i<ui->comboBox_6->count();i++)
        stream << ui->comboBox_6->itemText(i)<<"\n";
    stream << "combox7\n";
    for(int i=0;i<ui->comboBox_7->count();i++)
        stream << ui->comboBox_7->itemText(i)<<"\n";
    stream << "combox8\n";
    for(int i=0;i<ui->comboBox_8->count();i++)
        stream << ui->comboBox_8->itemText(i)<<"\n";
    file.close();
}

void MainWindow::readTxt()
{
    // 返回值
    QList<QString> strs;
    QString txtpath="D:/work/savedata";//QDir::currentPath();
    // 文件位置
    QFile file(txtpath+"/combox.txt");

    if(!file.open(QIODevice::ReadOnly))
    {
        return;
    }
    // 文件流
    QTextStream stream(&file);
    ui->comboBox->clear();
    ui->comboBox_2->clear();
    ui->comboBox_3->clear();
    ui->comboBox_4->clear();
    ui->comboBox_5->clear();
    ui->comboBox_6->clear();
    ui->comboBox_7->clear();
    ui->comboBox_8->clear();
    ui->comboBox_12->clear();
    ui->comboBox_13->clear();
    // 一行一行的读
    while(!stream.atEnd())
    {
        QString line = stream.readLine();

        strs.append(line);
    }
    file.close();
    int a1,a2,a3,a4,a5,a6,a7,a8;
    for(int o=0;o<strs.length();o++)
    {
        if(strs[o]=="combox1")a1=o;
        if(strs[o]=="combox2")a2=o;
        if(strs[o]=="combox3")a3=o;
        if(strs[o]=="combox4")a4=o;
        if(strs[o]=="combox5")a5=o;
        if(strs[o]=="combox6")a6=o;
        if(strs[o]=="combox7")a7=o;
        if(strs[o]=="combox8")a8=o;
    }
    for(int i=a1+1;i<a2;i++)
    {
        ui->comboBox->addItem(strs[i]);
        if(strs[i]=="请选择...")continue;
        ui->comboBox_13->addItem(strs[i]);
    }
    for(int i=a2+1;i<a3;i++)
    {
        ui->comboBox_2->addItem(strs[i]);
        if(strs[i]=="请选择...")continue;
        ui->comboBox_12->addItem(strs[i]);
    }
    for(int i=a3+1;i<a4;i++)
        ui->comboBox_3->addItem(strs[i]);
    for(int i=a4+1;i<a5;i++)
        ui->comboBox_4->addItem(strs[i]);
    for(int i=a5+1;i<a6;i++)
        ui->comboBox_5->addItem(strs[i]);
    for(int i=a6+1;i<a7;i++)
        ui->comboBox_6->addItem(strs[i]);
    for(int i=a7+1;i<a8;i++)
        ui->comboBox_7->addItem(strs[i]);
    for(int i=a8+1;i<strs.length();i++)
        ui->comboBox_8->addItem(strs[i]);

}

//保存文件
void MainWindow::writeTotal(QString fileName,bool isAppend)//保存total_my_msg_rank、total_my_msg
{

    // 文件位置
    QFile file(fileName);
    if(isAppend)
    {
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
        {
            return;
        }
    }
    else
    {
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
        {
            return;
        }
    }
    // 文件流
    QTextStream stream(&file);
    if(isAppend)//追加方式要加入时间
    {
        stream << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << "\n";
        stream << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz") << "\n";
    }
    // 输入内容
    for(int i=0;i<total_my_msg.length();i++)
    {

        stream << "total_my_msg%"+QString::number(i)+"%"+QString::number(total_my_msg[i].size())+"\n";
        for (auto item = total_my_msg[i].begin(); item != total_my_msg[i].end(); ++item)
        {
            stream << item.key()+"%"+item.value()["工序点"]+"%"+item.value()["牌号"]+"%"+item.value()["验证部门"]
                  +"%"+item.value()["验证依据"]+"%"+item.value()["水分仪编号"]+"%"+item.value()["电子天平编号"]+"%"+item.value()["烘箱编号"]
                    +"%"+item.value()["质检员"]+"%"+item.value()["日期"]+"%"+item.value()["表显值"]+"%"+item.value()["容器重量"]+"%"
                    +item.value()["烘前重量"]+"%"+item.value()["烘后重量"]+"%"+item.value()["nbcompare"]+"\n";
        }

    }

    stream << "total_my_msg_rank%"+QString::number(total_my_msg_rank.size())+"\n";
    for (auto item = total_my_msg_rank.begin(); item != total_my_msg_rank.end(); ++item)
    {
        stream << item.key();
        for(int i=0;i<item.value().length();i++)
            stream << "%"<<item.value()[i];
        stream << "\n";
    }


    file.close();
}
void MainWindow::readTotal(QString fileName)
{

    // 返回值
    QList<QString> strs;
    // 文件位置
    QFile file(fileName);

    if(!file.open(QIODevice::ReadOnly))
    {
        return;
    }
    // 文件流
    QTextStream stream(&file);
    // 一行一行的读
    while(!stream.atEnd())
    {
        QString line = stream.readLine();
        strs.append(line);
    }
    file.close();
    if(strs.length()<1)return;
    if(strs[0].split("o")[0]!='t'||strs.length()<2)return;
    for(int o=0;o<strs.length();o++)
    {

        if(strs[o].split("%")[0]=="total_my_msg")
        {
            for (int i=1;i<strs[o].split("%")[2].toInt()+1;i++)
            {
                QHash <QString, QString> msg1;//"表显值"<<"容器重量(g)"<<"烘前重量(g)"<<"烘后重量(g)"<<"样品水分(%)"<<"烘箱测试值(%)";
                msg1.insert("工序点",strs[o+i].split("%")[1]);
                msg1.insert("牌号",strs[o+i].split("%")[2]);
                msg1.insert("验证部门",strs[o+i].split("%")[3]);
                msg1.insert("验证依据",strs[o+i].split("%")[4]);
                msg1.insert("水分仪编号",strs[o+i].split("%")[5]);
                msg1.insert("电子天平编号",strs[o+i].split("%")[6]);
                msg1.insert("烘箱编号",strs[o+i].split("%")[7]);
                msg1.insert("质检员",strs[o+i].split("%")[8]);
                msg1.insert("日期",strs[o+i].split("%")[9]);
                msg1.insert("表显值",strs[o+i].split("%")[10]);
                msg1.insert("容器重量",strs[o+i].split("%")[11]);
                msg1.insert("烘前重量",strs[o+i].split("%")[12]);
                msg1.insert("烘后重量",strs[o+i].split("%")[13]);
                msg1.insert("样品水分","-1%");
                msg1.insert("烘箱测试值","-1%");
                msg1.insert("总平均水分","-1%");
                msg1.insert("nbcompare",strs[o+i].split("%")[14]);
                my_msg.insert(strs[o+i].split("%")[0],msg1);
            }
            total_my_msg.append(my_msg);
            my_msg.clear();
        }
        else if(strs[o].split("%")[0]=="total_my_msg_rank")
        {
            for (int i=1;i<1+strs[o].split("%")[1].toInt();i++)
            {
                for (int j=1;j<strs[o+i].split("%").length();j++)
                {
                    my_msg_rank.append(strs[o+i].split("%")[j]);
                }
                total_my_msg_rank.insert(strs[o+i].split("%")[0],my_msg_rank);

                my_msg_rank.clear();
            }

        }
    }

}

//保存文件
void MainWindow::writeComD()//保存工序点和牌号的分类信息
{
    QString txtpath="D:/work/savedata";//QDir::currentPath();
    // 文件位置
    QFile file(txtpath+"/comd.txt");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
        return;
    }
    // 文件流
    QTextStream stream(&file);

    for (auto item = combox.begin(); item != combox.end(); ++item)
    {
        stream << item.key();
        for(int i=0;i<item.value().length();i++)
            stream << "%"<<item.value()[i];
        stream << "\n";
    }
}

void MainWindow::readComD()//保存工序点和牌号的分类信息
{
    // 返回值
    QList<QString> strs;
    QString txtpath="D:/work/savedata";//QDir::currentPath();
    // 文件位置
    QFile file(txtpath+"/comd.txt");

    if(!file.open(QIODevice::ReadOnly))
    {
        return;
    }
    // 文件流
    QTextStream stream(&file);
    // 一行一行的读
    while(!stream.atEnd())
    {
        QString line = stream.readLine();
        strs.append(line);
    }
    file.close();
    QStringList msg;
    combox.clear();
    for(int o=0;o<strs.length();o++)
    {
        for (int i=1;i<strs[o].split("%").length();i++)
        {
            msg.append(strs[o].split("%")[i]);
        }
        combox.insert(strs[o].split("%")[0],msg);
        msg.clear();
    }
}
