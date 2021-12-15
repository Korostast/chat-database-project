#include "DatabaseChooserDialog.h"
#include "ui_databasechooserdialog.h"

DatabaseChooserDialog::DatabaseChooserDialog(QWidget *parent) : QDialog(parent), ui(new Ui::DatabaseChooserDialog) {
    ui->setupUi(this);
    deleteMessageBox = new DeleteDatabaseMessagebox(this);

    connect(ui->database_creation_add_button, SIGNAL(released()), this, SLOT(add_database()));
    connect(ui->database_creation_remove_button, SIGNAL(released()), this, SLOT(delete_database()));
    connect(ui->admin_auth_button, SIGNAL(released()), this, SLOT(admin_auth()));
}

DatabaseChooserDialog::~DatabaseChooserDialog() {
    delete ui;
}
