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

    int chatID;

    int userID;

    int messageID;

    QImage avatar;

    QString name;

    QString time;

    QString content;

    void contextMenuEvent(QContextMenuEvent *event) override;

public:

    int getChatID() const;

    void setChatID(int chatID);

    int getUserID() const;

    void setUserID(int userID);

    int getMessageID() const;

    void setMessageID(int id);

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