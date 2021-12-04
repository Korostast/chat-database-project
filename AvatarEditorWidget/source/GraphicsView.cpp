#include <QWheelEvent>
#include <QGraphicsEllipseItem>
#include "../include/AvatarEditor.h"
#include "../include/GraphicsView.h"

qreal GraphicsView::zoomFactor = 1;

GraphicsView::GraphicsView(QWidget *parent) : QGraphicsView(parent) {}

// Mouse wheel zoom
void GraphicsView::wheelEvent(QWheelEvent *event) {
    auto *avatarEditor = qobject_cast<AvatarEditor *>(this->window());

    // Determine which way the mouse wheel has turned
    int angle = event->angleDelta().y();
    qreal factor;
    if (angle > 0)
        factor = ZOOM_STEP;
    else
        factor = -ZOOM_STEP;

    // Check if the image has become smaller than a circle
    qreal newZoomFactor = zoomFactor + factor;
    bool isTooSmallImage = newZoomFactor * avatarEditor->image->width() * avatarEditor->scaleFactor < RESULT_IMAGE_SIZE;
    isTooSmallImage |= newZoomFactor * avatarEditor->image->height() * avatarEditor->scaleFactor < RESULT_IMAGE_SIZE;
    if (newZoomFactor > MAX_ZOOM_LEVEL + EPS || newZoomFactor < MIN_ZOOM_LEVEL - EPS || isTooSmallImage)
        factor = 0;
    else
        zoomFactor = newZoomFactor;

    // Alter scene item matrix. It is like 'scale()' function, but more flexible
    QTransform transform = this->transform();
    transform.setMatrix(transform.m11() + factor, 0, 0,
                        0, transform.m22() + factor, 0,
                        0, 0, 1);
    this->setTransform(transform);
    qDebug() << transform;

    // Recalculate ellipse scale and bounds of image
    avatarEditor->ellipseItem->setScale(1 / zoomFactor);
    avatarEditor->ellipseItem->rightBound = avatarEditor->ellipseItem->originalRightBound -
                                            avatarEditor->ellipseItem->rect().width() *
                                            (avatarEditor->ellipseItem->scale() - 1);
    avatarEditor->ellipseItem->bottomBound = avatarEditor->ellipseItem->originalBottomBound -
                                             avatarEditor->ellipseItem->rect().height() *
                                             (avatarEditor->ellipseItem->scale() - 1);

    avatarEditor->ellipseItem->renderCroppedImage();
}
