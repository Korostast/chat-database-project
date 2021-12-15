#include "ChatDialog.h"
#include "ui_chatdialog.h"

ChatDialog::ChatDialog(QWidget *parent) : QDialog(parent), ui(new Ui::ChatDialog) {
    ui->setupUi(this);
    connect(ui->chat_dialog_name, SIGNAL(returnPressed()), this, SLOT(changeChatName()));
    connect(ui->chat_dialog_avatar, &ClickableLabel::released, this, [this]() {
        openFileChooser();
    });
    connect(ui->chat_dialog_leave_button, SIGNAL(released()), this, SLOT(leaveChat()));
    connect(ui->chat_dialog_add_member_button, SIGNAL(released()), this, SLOT(loadAddMemberUI()));
    connect(ui->chat_dialog_add_members_button, SIGNAL(released()), this, SLOT(addMembers()));
}

ChatDialog::~ChatDialog() {
    delete ui;
}