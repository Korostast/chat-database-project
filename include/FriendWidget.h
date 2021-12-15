#ifndef CHATDATABASEPROJECT_FRIENDWIDGET_H
#define CHATDATABASEPROJECT_FRIENDWIDGET_H

#include <QWidget>
#include "Defines.h"
#include "AbstractFriendWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class FriendWidget;
}
QT_END_NAMESPACE

class FriendWidget : public AbstractFriendWidget {
Q_OBJECT

public:

    Ui::FriendWidget *ui;

    explicit FriendWidget(QWidget *parent = nullptr);

    ~FriendWidget() override;

public:

    void setUsername(const QString &username) override;

    void setAvatar(const QImage &avatar) override;

private slots:

    void friend_remove_button_released();

};

#endif //CHATDATABASEPROJECT_FRIENDWIDGET_H