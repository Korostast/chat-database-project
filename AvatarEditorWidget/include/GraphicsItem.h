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
    qreal originalRightBound;
    qreal originalBottomBound;

    explicit GraphicsEllipseItem(const QRectF &rect = QRectF(0, 0, 0, 0), QGraphicsItem *parent = nullptr,
                                 qreal rightBound = 0, qreal bottomBound = 0);

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    void renderCroppedImage();

private:
    void updateEllipsePosition();

};

#endif //UNTITLED13_GRAPHICSITEM_H
