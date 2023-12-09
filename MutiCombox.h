#ifndef MUTICOMBOBOX_H
#define MUTICOMBOBOX_H

#include <QWidget>
#include <QListView>
#include <QStandardItemModel>
#include <QComboBox>
class MutiComboBox : public QComboBox
{
    Q_OBJECT

public:
    MutiComboBox(QWidget *parent = nullptr);
    ~MutiComboBox() override;
    void addItem(const QString &text, const QVariant &userData = QVariant());
    void addItem(const QIcon &icon, const QString &text, const QVariant &userData = QVariant());
    void addItems(const QStringList &texts);
    QStringList currentText();
    QList<int> currentIndex();
protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
    void showPopup() override;
    void hidePopup() override;
public:
    bool isPermitHidePopup;
    QListView* selectItemView;
    QStandardItemModel* selectModel;
    QListView* popupView;
    QStandardItemModel* popupModel;
    void selectItemViewPress(QPoint pos);


};


#endif // MUTICOMBOBOX_H
