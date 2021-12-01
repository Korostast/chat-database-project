#ifndef CHATDATABASEPROJECT_CHATDIALOG_H
#define CHATDATABASEPROJECT_CHATDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
    class ChatDialog;
}
QT_END_NAMESPACE

class ChatDialog : public QDialog {
Q_OBJECT

public:
    explicit ChatDialog(QWidget *parent = nullptr);

    ~ChatDialog() override;

private:
    Ui::ChatDialog *ui;
};

#endif //CHATDATABASEPROJECT_CHATDIALOG_H
