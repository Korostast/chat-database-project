#include "ui_systemmessageform.h"
#include "UserMessageWidget.h"
#include "MainWindow.h"
#include "SystemMessageWidget.h"

SystemMessageWidget::SystemMessageWidget(QWidget *parent) : QWidget(parent), ui(new Ui::SystemMessageWidget),
                                                            messageID(-1), userID(-1) {
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

int SystemMessageWidget::getMessageID() const {
    return messageID;
}

void SystemMessageWidget::setMessageID(int id) {
    SystemMessageWidget::messageID = id;
}

int SystemMessageWidget::getUserID() const {
    return userID;
}

void SystemMessageWidget::setUserID(int userID) {
    SystemMessageWidget::userID = userID;
}