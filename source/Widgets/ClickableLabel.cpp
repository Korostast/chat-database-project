#include "ClickableLabel.h"

ClickableLabel::ClickableLabel(QWidget *parent, Qt::WindowFlags f)
        : QLabel(parent) {}

ClickableLabel::~ClickableLabel() = default;

void ClickableLabel::mouseReleaseEvent(QMouseEvent *event) {
    emit released();
}
