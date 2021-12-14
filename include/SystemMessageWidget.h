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

    int messageId;

    int userId;

    QString content;

public:
    explicit SystemMessageWidget(QWidget *parent = nullptr);

    ~SystemMessageWidget() override;

    int getMessageId() const;

    void setMessageId(int id);

    int getUserId() const;

    void setUserId(int userId);

    const QString &getContent() const;

    void setContent(const QString &content);

};

#endif //CHATDATABASEPROJECT_SYSTEMMESSAGEWIDGET_H
