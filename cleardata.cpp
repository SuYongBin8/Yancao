#include "cleardata.h"
#include "ui_cleardata.h"

ClearData::ClearData(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClearData)
{
    ui->setupUi(this);
}

ClearData::~ClearData()
{
    delete ui;
}

void ClearData::on_pushButton_clicked()//确定删除
{
    QMessageBox msgBox;
    msgBox.setText("温馨提示：");
    msgBox.setInformativeText("确实删除数据吗?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    int ret = msgBox.exec();
    if(ret == QMessageBox::Ok)
    {

        QStringList send;
        send<<ui->comboBox_2->currentText()<<"%"<<ui->comboBox->currentText();

        emit sendData(send);

        ui->comboBox->selectModel->clear();
        for(int i=0;i<ui->comboBox->popupModel->rowCount();i++)
        {
            ui->comboBox->popupModel->item(i)->setCheckState(Qt::Unchecked);
        }

        ui->comboBox_2->selectModel->clear();
        for(int i=0;i<ui->comboBox_2->popupModel->rowCount();i++)
        {
            ui->comboBox_2->popupModel->item(i)->setCheckState(Qt::Unchecked);
        }

    }
}

void ClearData::on_pushButton_2_clicked()//取消
{
    qDebug()<<ui->comboBox->currentText().length()<<ui->comboBox->currentText();
    this->close();
}

void ClearData::setCombox(QStringList com)//设置combox
{
//    for(int i=ui->comboBox->count()-1;i>=0;i--)
//        ui->comboBox->removeItem(i);
    ui->comboBox->clear();
ui->comboBox->selectModel->clear();
    QStringList m_LogictActIDMap;
    for(int i=0;i<com.length();i++)
    {
        if(com[i]=="请选择...")continue;
        m_LogictActIDMap.append(com[i]);
    }
    m_LogictActIDMap.append("全部");
    ui->comboBox->addItems(m_LogictActIDMap);

}

void ClearData::setCombox1(QStringList com)//设置combox
{
    ui->comboBox_2->clear();
    ui->comboBox_2->selectModel->clear();
    QStringList m_LogictActIDMap;
    for(int i=0;i<com.length();i++)
    {
        if(com[i]=="请选择...")continue;
        m_LogictActIDMap.append(com[i]);
    }
    m_LogictActIDMap.append("全部");
    ui->comboBox_2->addItems(m_LogictActIDMap);
}

