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

private:

    friend class GraphicsView;

    friend class GraphicsEllipseItem;

    Ui::AvatarEditor *ui;

    qreal scaleFactor;

    QImage *image;

    GraphicsEllipseItem *ellipseItem;

    void loadImageIntoScene(const QString &pathToImage);

    void updatePreview() const;

    void closeEvent(QCloseEvent *e) override;

public:

    explicit AvatarEditor(QWidget *parent = nullptr);

    ~AvatarEditor() override;

    static QPixmap getCircularPixmap(const QImage &targetImage, int size);

    void setChooseFilePage() const;

private slots:

    void editorFileChooserOpen();

    void saveImage();
};

#endif //UNTITLED13_AVATAREDITOR_H