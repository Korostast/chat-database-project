#ifndef CHATDATABASEPROJECT_SEARCHPEOPLEWIDGET_H
#define CHATDATABASEPROJECT_SEARCHPEOPLEWIDGET_H

#include <QWidget>
#include "FriendWidget.h"
#include "Defines.h"
#include "AbstractFriendWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class SearchPeopleWidget;
}
QT_END_NAMESPACE

class SearchPeopleWidget : public AbstractFriendWidget {
Q_OBJECT

public:
    Ui::SearchPeopleWidget *ui;

    explicit SearchPeopleWidget(QWidget *parent = nullptr);

    ~SearchPeopleWidget() override;

    void setUsername(const QString &username) override;

    void setAvatar(const QImage &avatar) override;

private slots:
    void add_friend_button_released();

};

#endif //CHATDATABASEPROJECT_SEARCHPEOPLEWIDGET_H
