#include "ClickableLabel.h"

ClickableLabel::ClickableLabel(QWidget *parent, Qt::WindowFlags f)
        : QLabel(parent), intPositiveData(-1) {
    setAttribute(Qt::WA_Hover, true);
    setMouseTracking(true);
}

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

void ClickableLabel::enterEvent(QEnterEvent *event) {
    qDebug() << "Entering Clickable";
    QFont f = font();
    f.setUnderline(true);
    setFont(f);
}

void ClickableLabel::leaveEvent(QEvent *event) {
    qDebug() << "Left";
    QFont f = font();
    f.setUnderline(false);
    setFont(f);
}