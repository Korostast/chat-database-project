#include "MainWindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // Test
    auto *button = new QPushButton("qerwerw");
    auto *button1 = new QPushButton("qerwerw");
    ui->chats_layout->addWidget(button);
    ui->chats_layout->addWidget(button1);
}

MainWindow::~MainWindow() {
    delete ui;
}

