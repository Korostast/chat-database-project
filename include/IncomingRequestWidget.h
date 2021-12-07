#ifndef CHATDATABASEPROJECT_INCOMINGREQUESTWIDGET_H
#define CHATDATABASEPROJECT_INCOMINGREQUESTWIDGET_H

#include <QWidget>
#include "FriendWidget.h"
#include "Defines.h"
#include "AbstractFriendWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class IncomingRequestWidget;
}
QT_END_NAMESPACE

class IncomingRequestWidget : public AbstractFriendWidget {
Q_OBJECT

public:
    Ui::IncomingRequestWidget *ui;

    explicit IncomingRequestWidget(QWidget *parent = nullptr);

    ~IncomingRequestWidget() override;

    void setUsername(const QString &username) override;

    void setAvatar(const QImage &avatar) override;

private slots:
    void accept_request_button_released();

    void decline_request_button_released();

};

#endif //CHATDATABASEPROJECT_INCOMINGREQUESTWIDGET_H
