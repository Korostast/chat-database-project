#include "../include/AvatarEditor.h"
#include <QGraphicsWidget>

GraphicsItem::GraphicsItem(const QPixmap &pixmap, QGraphicsItem *parent)
        : QGraphicsPixmapItem(pixmap, parent) {}

GraphicsEllipseItem::GraphicsEllipseItem(const QRectF &rect, QGraphicsItem *parent, qreal rightBound, qreal bottomBound)
        : QGraphicsEllipseItem(rect, parent), rightBound(rightBound), bottomBound(bottomBound) {
    this->originalRightBound = rightBound;
    this->originalBottomBound = bottomBound;
}

// Update ellipse position according to boundaries of the image
void GraphicsEllipseItem::updateEllipsePosition() {
    if (x() < 0)
        setPos(0, y());
    else if (x() > rightBound)
        setPos(rightBound, y());

    if (y() < 0)
        setPos(x(), 0);
    else if (y() > bottomBound)
        setPos(x(), bottomBound);
}

void GraphicsEllipseItem::renderCroppedImage() {
    updateEllipsePosition();
    auto *avatarEditor = qobject_cast<AvatarEditor *>(this->scene()->parent()); // TODO can I do something with this?
    avatarEditor->updatePreview();
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

