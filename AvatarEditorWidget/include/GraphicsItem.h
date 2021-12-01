#ifndef UNTITLED13_GRAPHICSITEM_H
#define UNTITLED13_GRAPHICSITEM_H

#include <QGraphicsItem>

class GraphicsItem : public QGraphicsPixmapItem {
public:
    explicit GraphicsItem(const QPixmap &pixmap, QGraphicsItem *parent = nullptr);
};


class GraphicsEllipseItem : public QGraphicsEllipseItem {
public:
    qreal rightBound;
    qreal bottomBound;
    qreal oldRightBound;
    qreal oldBottomBound;

    explicit GraphicsEllipseItem(const QRectF &rect, QGraphicsItem *parent = nullptr,
                                 qreal rightBound = 0, qreal bottomBound = 0);

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    void renderCroppedImage();
};

#endif //UNTITLED13_GRAPHICSITEM_H
