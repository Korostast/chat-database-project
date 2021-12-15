#ifndef CHATDATABASEPROJECT_SYSTEMMESSAGEWIDGET_H
#define CHATDATABASEPROJECT_SYSTEMMESSAGEWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
    class SystemMessageWidget;
}
QT_END_NAMESPACE

class SystemMessageWidget : public QWidget {
Q_OBJECT

private:
    Ui::SystemMessageWidget *ui;

    int messageID;

    int userID;

    QString content;

public:
    explicit SystemMessageWidget(QWidget *parent = nullptr);

    ~SystemMessageWidget() override;

    int getMessageID() const;

    void setMessageID(int id);

    int getUserID() const;

    void setUserID(int userID);

    const QString &getContent() const;

    void setContent(const QString &content);

};

#endif //CHATDATABASEPROJECT_SYSTEMMESSAGEWIDGET_H