#include "ClickableLabel.h"

ClickableLabel::ClickableLabel(QWidget *parent, Qt::WindowFlags f)
        : QLabel(parent), intPositiveData(-1) {}

void ClickableLabel::mouseReleaseEvent(QMouseEvent *event) {
    emit released();
}

int ClickableLabel::getPositiveIntData() const {
    return intPositiveData;
}

void ClickableLabel::setPositiveIntData(int intData) {
    if (intData < 0) {
        qCritical() << "Trying to set negative int to positive int data";
        return;
    }
    ClickableLabel::intPositiveData = intData;
}