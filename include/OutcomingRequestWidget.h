#ifndef CHATDATABASEPROJECT_OUTCOMINGREQUESTWIDGET_H
#define CHATDATABASEPROJECT_OUTCOMINGREQUESTWIDGET_H

#include <QWidget>
#include "FriendWidget.h"
#include "Defines.h"
#include "AbstractFriendWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class OutcomingRequestWidget;
}
QT_END_NAMESPACE

class OutcomingRequestWidget : public AbstractFriendWidget {
Q_OBJECT

public:
    Ui::OutcomingRequestWidget *ui;

    explicit OutcomingRequestWidget(QWidget *parent = nullptr);

    ~OutcomingRequestWidget() override;

    void setUsername(const QString &username) override;

    void setAvatar(const QImage &avatar) override;

private slots:
    void remove_request_button_released();

};

#endif //CHATDATABASEPROJECT_OUTCOMINGREQUESTWIDGET_H
