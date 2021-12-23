#include <QMessageBox>
#include "DeleteDatabaseMessagebox.h"
#include "ui_deletedatabasemessagebox.h"
#include "ui_databasechooserdialog.h"
#include "SqlInterface.h"

DeleteDatabaseMessagebox::DeleteDatabaseMessagebox(QWidget *parent) : QDialog(parent), ui(new Ui::DeleteDatabaseMessagebox) {
    ui->setupUi(this);

    connect(ui->messagebox_delete_database_edit, SIGNAL(textChanged(QString)), this, SLOT(delete_database_edit_text_changed(QString)));
    connect(ui->messagebox_delete_database_confirm, SIGNAL(released()), this, SLOT(confirm_delete_database()));
}

DeleteDatabaseMessagebox::~DeleteDatabaseMessagebox() {
    delete ui;
}

void DeleteDatabaseMessagebox::delete_database_edit_text_changed(const QString &) const {
    if (databaseNameWithDelete == ui->messagebox_delete_database_edit->text())
        ui->messagebox_delete_database_confirm->setEnabled(true);
    else
        ui->messagebox_delete_database_confirm->setEnabled(false);
}

void DeleteDatabaseMessagebox::confirm_delete_database() {
    qDebug() << QString("Database with name \"%1\" has been deleted").arg(databaseName);
    try {
        sqlDeleteDatabase(databaseName);
        auto *databaseChooser = qobject_cast<DatabaseChooserDialog *>(parent());
        QList<QListWidgetItem*> items = databaseChooser->ui->database_creation_list->selectedItems();
        foreach(QListWidgetItem * item, items) {
            delete databaseChooser->ui->database_creation_list->takeItem(databaseChooser->ui->database_creation_list->row(item));
        }
    } catch (QSqlException &error) {
        QMessageBox::critical(this, "Ошибка", error.what());
    }
    close();
}