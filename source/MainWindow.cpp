#include "MainWindow.h"
#include "./ui_mainwindow.h"
#include "ChatForm.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // Test
    for (int i = 0; i < 5; ++i) {
        addChat("Hello world!");
    }
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::addChat(const QString& name) {
    auto *item = new QListWidgetItem;
    auto *widget = new ChatForm(this);
    widget->setChatName(name);
    widget->setChatAvatar(":images/testImage.png", CHAT_IMAGE_SIZE);

    item->setSizeHint(widget->sizeHint());

    ui->listWidget->addItem(item);
    ui->listWidget->setItemWidget(item, widget);
}

