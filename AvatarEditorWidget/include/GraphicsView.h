#ifndef UNTITLED13_GRAPHICSVIEW_H
#define UNTITLED13_GRAPHICSVIEW_H

#include <QGraphicsView>

class GraphicsView : public QGraphicsView {
private:
    static qreal zoomFactor;

public:
    explicit GraphicsView(QWidget *parent = nullptr);

    void wheelEvent(QWheelEvent *event) override;
};

#endif //UNTITLED13_GRAPHICSVIEW_H
