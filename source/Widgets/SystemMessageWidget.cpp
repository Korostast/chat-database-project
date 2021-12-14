#include "ui_systemmessageform.h"
#include "UserMessageWidget.h"
#include "MainWindow.h"
#include "SystemMessageWidget.h"

SystemMessageWidget::SystemMessageWidget(QWidget *parent) : QWidget(parent), ui(new Ui::SystemMessageWidget),
                                                            messageId(-1), userId(-1) {
    ui->setupUi(this);
}

SystemMessageWidget::~SystemMessageWidget() {
    delete ui;
}

const QString &SystemMessageWidget::getContent() const {
    return content;
}

void SystemMessageWidget::setContent(const QString &content) {
    SystemMessageWidget::content = content;
    ui->system_message_content->setText(content);
}

int SystemMessageWidget::getMessageId() const {
    return messageId;
}

void SystemMessageWidget::setMessageId(int messageId) {
    SystemMessageWidget::messageId = messageId;
}

int SystemMessageWidget::getUserId() const {
    return userId;
}

void SystemMessageWidget::setUserId(int userId) {
    SystemMessageWidget::userId = userId;
}
