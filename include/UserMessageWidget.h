#ifndef CHATDATABASEPROJECT_USERMESSAGEWIDGET_H
#define CHATDATABASEPROJECT_USERMESSAGEWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
    class UserMessageWidget;
}
QT_END_NAMESPACE

class UserMessageWidget : public QWidget {
Q_OBJECT

private:

    int chatId;

    int messageId;

    QImage avatar;

    QString name;

    QString time;

    QString content;

    void contextMenuEvent(QContextMenuEvent *event) override;

public:

    int getChatId() const;

    void setChatId(int chatId);

    int getMessageId() const;

    void setMessageId(int id);

    const QImage & getAvatar() const;

    void setAvatar(const QImage &image);

    const QString &getName() const;

    void setName(const QString &name);

    const QString &getTime() const;

    void setTime(const QString &time);

    const QString &getContent() const;

    void setContent(const QString &content);

public:
    Ui::UserMessageWidget *ui;

    explicit UserMessageWidget(QWidget *parent = nullptr);

    ~UserMessageWidget() override;

    static int countRows(const QString &text);
};

#endif //CHATDATABASEPROJECT_USERMESSAGEWIDGET_H
