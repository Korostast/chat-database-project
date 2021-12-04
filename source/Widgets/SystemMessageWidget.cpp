#include "ui_systemmessageform.h"
#include "UserMessageWidget.h"
#include "MainWindow.h"
#include "SystemMessageWidget.h"

SystemMessageWidget::SystemMessageWidget(QWidget *parent) : QWidget(parent), ui(new Ui::SystemMessageWidget), id(-1) {
    ui->setupUi(this);
}

SystemMessageWidget::~SystemMessageWidget() {
    delete ui;
}

int SystemMessageWidget::getId() const {
    return id;
}

void SystemMessageWidget::setId(int id) {
    SystemMessageWidget::id = id;
}

const QString &SystemMessageWidget::getContent() const {
    return content;
}

void SystemMessageWidget::setContent(const QString &content) {
    SystemMessageWidget::content = content;
    ui->system_message_content->setText(content);
}
