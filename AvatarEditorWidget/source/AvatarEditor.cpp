#include "../include/AvatarEditor.h"
#include "ui_avatareditor.h"

AvatarEditor::AvatarEditor(QWidget *parent)
        : QDialog(parent), ui(new Ui::AvatarEditor) {
    ui->setupUi(this);
    //loadImageIntoScene(":chatDefaultImage.png");

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(saveImage()));
}

void AvatarEditor::loadImageIntoScene(const QString &pathToImage) {// Set graphics view scene
    {
        scene = new QGraphicsScene(this);
        ui->graphicsView->setScene(scene);
        image = new QImage(pathToImage);    // TODO path in constructor
    }

    // Calculate scale factor
    {
        qreal viewWidth = ui->graphicsView->width();
        qreal viewHeight = ui->graphicsView->height();
        qreal imageWidth = image->width();
        if (viewWidth < imageWidth) {
            scaleFactor = viewWidth / imageWidth;
        }
        qreal newHeight = image->height() * scaleFactor;
        if (viewHeight < newHeight) {
            scaleFactor *= viewHeight / newHeight;
        }
        qDebug() << "Scale factor =" << scaleFactor;
    }

    // Image item in the scene
    {
        imageItem = new GraphicsItem(QPixmap::fromImage(*image), nullptr);
        imageItem->setScale(scaleFactor);
        // Accurate smooth scaling, high quality
        imageItem->setTransformationMode(Qt::SmoothTransformation);
        scene->addItem(imageItem);
    }

    // Ellipse item in the scene
    {
        ellipseItem = new GraphicsEllipseItem(QRect(0, 0, RESULT_IMAGE_SIZE, RESULT_IMAGE_SIZE), nullptr,
                                              image->width() * scaleFactor - RESULT_IMAGE_SIZE,
                                              image->height() * scaleFactor - RESULT_IMAGE_SIZE);
        ellipseItem->setAcceptHoverEvents(true);

        // Construct a black semi-transparent circle
        QPen blackPen(Qt::black);
        blackPen.setWidth(6);
        ellipseItem->setFlag(QGraphicsItem::ItemIsMovable);
        ellipseItem->setPen(blackPen);
        ellipseItem->setOpacity(0.5);
        scene->addItem(ellipseItem);
        ellipseItem->renderCroppedImage();
    }
}

AvatarEditor::~AvatarEditor() {
    delete ui;
}

// Place image into preview label
void AvatarEditor::putImage() const {
    qreal reversedScaleFactor = 1 / scaleFactor;
    QRect rect((int) (ellipseItem->x() * reversedScaleFactor), (int) (ellipseItem->y() * reversedScaleFactor),
               (int) (ellipseItem->rect().width() * reversedScaleFactor * ellipseItem->scale()),
               (int) (ellipseItem->rect().height() * reversedScaleFactor * ellipseItem->scale()));

    QImage result = image->copy(rect);
    QPixmap resultPixmap = QPixmap::fromImage(result).scaled(RESULT_IMAGE_SIZE, RESULT_IMAGE_SIZE,
                                                             Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // Draw circular image
    QSize size(RESULT_IMAGE_SIZE, RESULT_IMAGE_SIZE);
    QBitmap mask(size);
    QPainter painter(&mask);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.fillRect(0, 0, size.width(), size.height(), Qt::white);
    painter.setBrush(QColor(0, 0, 0));
    painter.drawRoundedRect(0, 0, size.width(), size.height(), size.width() / 2.0, size.height() / 2.0);
    resultPixmap.setMask(mask);

    ui->label->setPixmap(resultPixmap);
}

void AvatarEditor::saveImage() const {
    qreal reversedScaleFactor = 1 / scaleFactor;
    qDebug() << reversedScaleFactor << ellipseItem->scale();
    QRect rect((int) (ellipseItem->x() * reversedScaleFactor), (int) (ellipseItem->y() * reversedScaleFactor),
               (int) (ellipseItem->rect().width() * reversedScaleFactor * ellipseItem->scale()),
               (int) (ellipseItem->rect().height() * reversedScaleFactor * ellipseItem->scale()));

    qDebug() << rect;

    QImage result = image->copy(rect).scaled(RESULT_IMAGE_SIZE, RESULT_IMAGE_SIZE,
                                             Qt::KeepAspectRatio, Qt::SmoothTransformation);
    if (!result.save("../resources/images/avatar.png", "png")) {    //TODO path to image
        qDebug() << "error";
    }
}