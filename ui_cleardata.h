/********************************************************************************
** Form generated from reading UI file 'cleardata.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLEARDATA_H
#define UI_CLEARDATA_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include "MutiCombox.h"

QT_BEGIN_NAMESPACE

class Ui_ClearData
{
public:
    QLabel *label;
    MutiComboBox *comboBox_2;
    MutiComboBox *comboBox;
    QLabel *label_2;
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QWidget *ClearData)
    {
        if (ClearData->objectName().isEmpty())
            ClearData->setObjectName(QString::fromUtf8("ClearData"));
        ClearData->resize(721, 357);
        label = new QLabel(ClearData);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(30, 150, 151, 101));
        QFont font;
        font.setPointSize(23);
        label->setFont(font);
        comboBox_2 = new MutiComboBox(ClearData);
        comboBox_2->addItem(QString());
        comboBox_2->addItem(QString());
        comboBox_2->setObjectName(QString::fromUtf8("comboBox_2"));
        comboBox_2->setGeometry(QRect(140, 10, 571, 111));
        QFont font1;
        font1.setPointSize(34);
        comboBox_2->setFont(font1);
        comboBox = new MutiComboBox(ClearData);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setGeometry(QRect(140, 140, 571, 111));
        comboBox->setFont(font1);
        comboBox->setIconSize(QSize(16, 16));
        label_2 = new QLabel(ClearData);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(30, 20, 111, 101));
        label_2->setFont(font);
        pushButton = new QPushButton(ClearData);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(380, 270, 331, 81));
        pushButton->setFont(font);
        pushButton_2 = new QPushButton(ClearData);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(10, 270, 291, 81));
        pushButton_2->setFont(font);

        retranslateUi(ClearData);

        comboBox_2->setCurrentIndex(0);
        comboBox->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(ClearData);
    } // setupUi

    void retranslateUi(QWidget *ClearData)
    {
        ClearData->setWindowTitle(QCoreApplication::translate("ClearData", "Form", nullptr));
        label->setText(QCoreApplication::translate("ClearData", "\345\267\245\345\272\217\347\202\271\357\274\232", nullptr));
        comboBox_2->setItemText(0, QCoreApplication::translate("ClearData", "\350\257\267\351\200\211\346\213\251...", nullptr));
        comboBox_2->setItemText(1, QCoreApplication::translate("ClearData", "A\347\272\247\346\242\227\344\270\235(\345\216\246\351\227\250)", nullptr));

        comboBox->setItemText(0, QCoreApplication::translate("ClearData", "\350\257\267\351\200\211\346\213\251...", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("ClearData", "\346\235\276\346\225\243A", nullptr));
        comboBox->setItemText(2, QCoreApplication::translate("ClearData", "\346\235\276\346\225\243B", nullptr));
        comboBox->setItemText(3, QCoreApplication::translate("ClearData", "\345\212\240\346\226\231B", nullptr));
        comboBox->setItemText(4, QCoreApplication::translate("ClearData", "\345\242\236\346\270\251\345\242\236\346\271\277B", nullptr));
        comboBox->setItemText(5, QCoreApplication::translate("ClearData", "\344\270\235\345\212\240\346\226\231\345\211\215A", nullptr));
        comboBox->setItemText(6, QCoreApplication::translate("ClearData", "\347\203\230\347\256\261\346\257\224\345\257\271", nullptr));
        comboBox->setItemText(7, QCoreApplication::translate("ClearData", "710E\346\257\224\345\257\271", nullptr));

        label_2->setText(QCoreApplication::translate("ClearData", "\347\211\214\345\217\267\357\274\232", nullptr));
        pushButton->setText(QCoreApplication::translate("ClearData", "\347\241\256\345\256\232", nullptr));
        pushButton_2->setText(QCoreApplication::translate("ClearData", "\345\217\226\346\266\210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ClearData: public Ui_ClearData {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLEARDATA_H
