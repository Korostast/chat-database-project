#include "ChatDialog.h"
#include "ui_chatdialog.h"

ChatDialog::ChatDialog(QWidget *parent) : QDialog(parent), ui(new Ui::ChatDialog) {
    ui->setupUi(this);
    connect(ui->chat_dialog_name, SIGNAL(returnPressed()), this, SLOT(changeChatName()));
    connect(ui->chat_dialog_avatar, SIGNAL(released()), this, SLOT(openFileChooser()));
    connect(ui->chat_dialog_leave_button, SIGNAL(released()), this, SLOT(leaveChat()));
}

ChatDialog::~ChatDialog() {
    delete ui;
}