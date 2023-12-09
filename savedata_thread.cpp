#include "savedata_thread.h"


sonThread::sonThread(QObject *parent) : QThread(parent)
{
    table = new QTableWidget;
}

void sonThread::run()
{

    // 创建EXCEL文件
    QXlsx::Document xlsxDocument(name);


    // 设置单元格样式
    QXlsx::Format titleFormat;//标题单元格样式
    titleFormat.setFontBold(true);//字体加粗
    titleFormat.setHorizontalAlignment(QXlsx::Format::AlignHCenter);//横向居中
    titleFormat.setVerticalAlignment(QXlsx::Format::AlignVCenter);//纵向居中

    QXlsx::Format contentFormat;//内容单元格样式
    contentFormat.setHorizontalAlignment(QXlsx::Format::AlignHCenter);//横向居中
    contentFormat.setVerticalAlignment(QXlsx::Format::AlignVCenter);//纵向居中

    //合并单元格
//    xlsxDocument.mergeCells(QXlsx::CellRange(1, 1, 2, 1));
//    xlsxDocument.mergeCells(QXlsx::CellRange(1, 2, 2, 2));
//    for(int i = 1; i <= 20; i++) {
//        xlsxDocument.mergeCells(QXlsx::CellRange(1, 2 * i + 1, 1, 2 * i + 2));
//    }
// 写入标题行  "工序点"<<"牌号"<<"样品序号"<<"表显值"<<"容器重量(g)"<<"烘前重量(g)"<<"烘后重量(g)"<<"样品水分(%)"<<"烘箱测试值(%)"<<"总平均水分(%)";

    xlsxDocument.write(1, 1, QString(" 工序点 "), titleFormat);
    xlsxDocument.write(1, 2, QString("牌号"), titleFormat);
    xlsxDocument.write(1, 3, QString("样品序号"), titleFormat);
    xlsxDocument.write(1, 4, QString("表显值"), titleFormat);
    xlsxDocument.write(1, 5, QString("容器重量(g)"), titleFormat);
    xlsxDocument.write(1, 6, QString(" 烘前重量(g) "), titleFormat);
    xlsxDocument.write(1, 7, QString("烘后重量(g)"), titleFormat);
    xlsxDocument.write(1, 8, QString("样品水分(%)"), titleFormat);
    xlsxDocument.write(1, 9, QString("烘箱测试值(%)"), titleFormat);
    xlsxDocument.write(1, 10, QString("总平均水分(%)"), titleFormat);

    QString str;
    int length =table->rowCount();
    int dd=0;
    for(int i=0;i<length;i++)
    {
        int flag = table->item(i,6)->text().toInt();//取出字符串
        qDebug()<<isAll<<flag<<dd;
        if(isAll==0&&flag==0){dd++;continue;}

        for(int j = 0; j < 9; j++)
        {
            str = table->item(i,j)->text();//取出字符串
            if(str=="-1%")
                xlsxDocument.write(i+2-dd, j+1, " ", contentFormat);
            else
                xlsxDocument.write(i+2-dd, j+1, str, contentFormat);
        }
    }
    // 保存文件
    xlsxDocument.saveAs(name);
}
