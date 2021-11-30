#include <QPainter>
#include <QPainterPath>
#include "ChatForm.h"
#include "ui_chatform.h"

ChatForm::ChatForm(QWidget *parent) : QWidget(parent), ui(new Ui::ChatForm) {
    ui->setupUi(this);
}

ChatForm::~ChatForm() {
    delete ui;
}

void ChatForm::mouseReleaseEvent(QMouseEvent *event) {
    qDebug() << this->ui->name->text();
}

void ChatForm::setChatName(const QString& name) const {
    ui->name->setText(name);
}

QPixmap ChatForm::getCircularPixmap(const QString &pathToImage, const int size) {
    QPixmap currentPixmap(pathToImage);
    currentPixmap = currentPixmap.scaled(size, size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    QPixmap resultPixmap(QSize(size, size));
    resultPixmap.fill(Qt::transparent);

    QPainter painter(&resultPixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    QPainterPath p = QPainterPath();
    p.addRoundedRect(0, 0, size, size, size / 2.0, size / 2.0);
    painter.setClipPath(p);
    painter.drawPixmap(0, 0, currentPixmap);
    return resultPixmap;
}

void ChatForm::setChatAvatar(const QString &pathToImage, const int size) const {
    QPixmap resultPixmap = getCircularPixmap(pathToImage, size);

    ui->avatar->setPixmap(resultPixmap);
}

void ChatForm::mousePressEvent(QMouseEvent *event) {
    qDebug() << "Pressed";
}
