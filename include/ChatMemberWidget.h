#ifndef CHATDATABASEPROJECT_CHATMEMBERWIDGET_H
#define CHATDATABASEPROJECT_CHATMEMBERWIDGET_H

#include <QWidget>
#include "Defines.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class ChatMemberWidget;
}
QT_END_NAMESPACE

class ChatMemberWidget : public QWidget {
Q_OBJECT

private:
    int id;

    QImage avatar;

    QString name;

    ROLE role;
public:
    int getId() const;

    void setId(int id);

    const QImage &getAvatar() const;

    void setAvatar(const QImage &avatar);

    const QString &getName() const;

    void setName(const QString &name);

    ROLE getRole() const;

    void setRole(ROLE role);

public:
    Ui::ChatMemberWidget *ui;

    explicit ChatMemberWidget(QWidget *parent = nullptr);

    ~ChatMemberWidget() override;

private slots:
    void removeMember();

    void changeMemberRole(int index);
};

#endif //CHATDATABASEPROJECT_CHATMEMBERWIDGET_H