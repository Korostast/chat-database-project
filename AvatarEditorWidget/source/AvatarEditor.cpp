#include <QFileDialog>
#include "../include/AvatarEditor.h"
#include "ui_avatareditor.h"
#include "MainWindow.h"

AvatarEditor::AvatarEditor(QWidget *parent)
        : QDialog(parent), ui(new Ui::AvatarEditor) {
    ui->setupUi(this);
    //loadImageIntoScene(":chatDefaultImage.png");
    scene = new QGraphicsScene(this);   // TODO refactor this? Replace public variables with local ones
    image = new QImage;
    scaleFactor = 0;

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(saveImage()));
    connect(ui->avatar_editor_choose_file_button, SIGNAL(clicked()), this, SLOT(avatar_editor_choose_file_released()));
}

void AvatarEditor::loadImageIntoScene(const QString &pathToImage) {// Set graphics view scene
    {
        //scene = new QGraphicsScene(this);
        ui->graphicsView->setScene(scene);
        image = new QImage(pathToImage);    // TODO do it in constructor? Memory leak here
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
        imageItem = new GraphicsItem(QPixmap::fromImage(*image), nullptr);  // TODO leak memory
        imageItem->setScale(scaleFactor);
        // Accurate smooth scaling, high quality
        imageItem->setTransformationMode(Qt::SmoothTransformation);
        scene->addItem(imageItem);
    }

    // Ellipse item in the scene
    {
        // TODO leak memory here
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

void AvatarEditor::saveImage() {
    qreal reversedScaleFactor = 1 / scaleFactor;
    qDebug() << reversedScaleFactor << ellipseItem->scale();
    QRect rect((int) (ellipseItem->x() * reversedScaleFactor), (int) (ellipseItem->y() * reversedScaleFactor),
               (int) (ellipseItem->rect().width() * reversedScaleFactor * ellipseItem->scale()),
               (int) (ellipseItem->rect().height() * reversedScaleFactor * ellipseItem->scale()));

    qDebug() << rect;

    QImage result = image->copy(rect).scaled(RESULT_IMAGE_SIZE, RESULT_IMAGE_SIZE,
                                             Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // TODO database work here

    auto *mainWindow = qobject_cast<MainWindow *>(this->parentWidget());

    if (MainWindow::currentState == MESSAGES) {
        if (!result.save(QString("../resources/images/chats/%1.png").arg(MainWindow::currentChat->getId()),"png")) {    //TODO path to image
            qDebug() << "Error: can't save image";
        }
        mainWindow->updateChat(MainWindow::currentChat->getId(), MainWindow::currentChat->getName(),
                               result, MainWindow::currentChat->getRole());
    }
    else {  // if (currentState == USER)
        // TODO user avatar
        if (!result.save(QString("../resources/images/users/%1.png").arg(MainWindow::currentUser->getId()),"png")) {    //TODO path to image
            qDebug() << "Error: can't save image";
        }
    }

    scene->clear();
    hide();
}

void AvatarEditor::setChooseFilePage() const {
    ui->avatar_editor_stacked_widget->setCurrentIndex(0);
}

void AvatarEditor::avatar_editor_choose_file_released() {
    qDebug() << "Open chooser";
    QString fileName = QFileDialog::getOpenFileName(this, "Open a file", "/", "Image Files (*.png)");
    if (fileName != nullptr) { // TODO refactor
        qDebug() << "File chosen: " << fileName;
        loadImageIntoScene(fileName);
        auto *mainWindow = qobject_cast<MainWindow *>(this->parentWidget());
        auto windowCentreX = mainWindow->x() + mainWindow->width() / 2 - EDITOR_PAGE_WIDTH / 2;
        auto windowCentreY = mainWindow->y() + mainWindow->height() / 2 - EDITOR_PAGE_HEIGHT / 2;
        mainWindow->avatarEditor->setGeometry(windowCentreX, windowCentreY,
                                              EDITOR_PAGE_WIDTH, EDITOR_PAGE_HEIGHT);
        mainWindow->avatarEditor->setFixedSize(EDITOR_PAGE_WIDTH, EDITOR_PAGE_HEIGHT);
        ui->avatar_editor_stacked_widget->setCurrentIndex(1); // TODO ?
    } else {
        qDebug() << "Cancel choosing";
    }

}
