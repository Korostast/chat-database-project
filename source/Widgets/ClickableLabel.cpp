#include "ClickableLabel.h"

ClickableLabel::ClickableLabel(QWidget *parent, Qt::WindowFlags f)
        : QLabel(parent) {}

void ClickableLabel::mouseReleaseEvent(QMouseEvent *event) {
    emit released();
}

int ClickableLabel::getPersonalChatUserId() const {
    return personalChatUserId;
}

void ClickableLabel::setPersonalChatUserId(int personalChatUserId) {
    ClickableLabel::personalChatUserId = personalChatUserId;
}
