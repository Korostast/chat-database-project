#ifndef UNTITLED13_GRAPHICSVIEW_H
#define UNTITLED13_GRAPHICSVIEW_H

#include <QGraphicsView>

class GraphicsView : public QGraphicsView {
public:
    explicit GraphicsView(QWidget *parent = nullptr);

    void wheelEvent(QWheelEvent *event) override;

    static qreal zoomFactor;
};

#endif //UNTITLED13_GRAPHICSVIEW_H
