#ifndef CHATDATABASEPROJECT_USERMESSAGEFORM_H
#define CHATDATABASEPROJECT_USERMESSAGEFORM_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
    class UserMessageForm;
}
QT_END_NAMESPACE

class UserMessageForm : public QWidget {
Q_OBJECT

private:
    int id;

    QImage avatar;

    QString name;

    QString time;

    QString content;
public:
    int getId() const;

    void setId(int id);

    const QImage & getAvatar() const;

    void setAvatar(const QImage &image);

    const QString &getName() const;

    void setName(const QString &name);

    const QString &getTime() const;

    void setTime(const QString &time);

    const QString &getContent() const;

    void setContent(const QString &content);

public:
    Ui::UserMessageForm *ui;

    explicit UserMessageForm(QWidget *parent = nullptr);

    ~UserMessageForm() override;

};

#endif //CHATDATABASEPROJECT_USERMESSAGEFORM_H
