#include "../include/AvatarEditor.h"
#include <QGraphicsWidget>

GraphicsItem::GraphicsItem(const QPixmap &pixmap, QGraphicsItem *parent)
        : QGraphicsPixmapItem(pixmap, parent) {}

GraphicsEllipseItem::GraphicsEllipseItem(const QRectF &rect, QGraphicsItem *parent, qreal rightBound, qreal bottomBound)
        : QGraphicsEllipseItem(rect, parent) {
    this->rightBound = rightBound;
    this->bottomBound = bottomBound;
    this->oldRightBound = rightBound;
    this->oldBottomBound = bottomBound;
}

void GraphicsEllipseItem::renderCroppedImage() {
    if (x() < 0)
        setPos(0, y());
    else if (x() > rightBound)
        setPos(rightBound, y());

    if (y() < 0)
        setPos(x(), 0);
    else if (y() > bottomBound)
        setPos(x(), bottomBound);

    auto *avatarEditor = qobject_cast<AvatarEditor *>(this->scene()->parent());
    avatarEditor->putImage();
}

void GraphicsEllipseItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsItem::mouseMoveEvent(event);
    this->setCursor(Qt::ClosedHandCursor);
    renderCroppedImage();
}

void GraphicsEllipseItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    QGraphicsItem::hoverEnterEvent(event);
    this->setCursor(Qt::OpenHandCursor);
}

void GraphicsEllipseItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsItem::mouseReleaseEvent(event);
    this->setCursor(Qt::OpenHandCursor);
}

