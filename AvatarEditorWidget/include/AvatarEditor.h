#ifndef UNTITLED13_AVATAREDITOR_H
#define UNTITLED13_AVATAREDITOR_H

#include <QDialog>
#include <QGraphicsScene>
#include "GraphicsItem.h"
#include "Defines.h"

QT_BEGIN_NAMESPACE
namespace Ui { class AvatarEditor; }
QT_END_NAMESPACE

class AvatarEditor : public QDialog {
Q_OBJECT

public:
    explicit AvatarEditor(QWidget *parent = nullptr);

    ~AvatarEditor() override;

    qreal scaleFactor;

    QGraphicsScene *scene;

    GraphicsItem *imageItem;

    QImage *image;

    GraphicsEllipseItem *ellipseItem;

    Ui::AvatarEditor *ui;

public:
    void putImage() const;

    void loadImageIntoScene(const QString &pathToImage);

private slots:

    void saveImage() const;
};

#endif //UNTITLED13_AVATAREDITOR_H
