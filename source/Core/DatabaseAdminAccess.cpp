#include <QMessageBox>
#include "DatabaseChooserDialog.h"
#include "ui_databasechooserdialog.h"
#include "ui_deletedatabasemessagebox.h"
#include "SqlInterface.h"

bool DatabaseChooserDialog::isDatabaseNameCorrect(const QString& databaseName) {
    QString errorMessage("Название базы данных может состоять только из латинских букв, цифр и нижнего подчёркивания");
    if (databaseName.isEmpty()) {
        QMessageBox::critical(this, "Ошибка", QString(errorMessage));
        return false;
    }
    for (auto letter: databaseName.toLatin1()) {
        if (!isalnum(letter) && letter != '_') {
            QMessageBox::critical(this, "Ошибка", QString(errorMessage));
            return false;
        }
    }
    return true;
}

void DatabaseChooserDialog::add_database() {
    QString databaseName(ui->database_creation_edit->text());
    if (!isDatabaseNameCorrect(databaseName))
        return;

    // Check if current name is not in list
    for (int i = 0; i < ui->database_creation_list->count(); ++i) {
        if (ui->database_creation_list->item(i)->data(Qt::DisplayRole) == databaseName) {
            qWarning() << QString("Database name %1 is already in list").arg(databaseName);
            QMessageBox::warning(this, "Ошибка", QString("База данных с названием '%1' уже существует").arg(databaseName));
            return;
        }
    }

    // Throw message box
    QString content("Вы уверены, что хотите создать базу данных с названием '%1'?");
    content = content.arg(databaseName);
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Создание базы данных", content,
                                                              QMessageBox::Yes | QMessageBox::No);
    // If we confirmed deleting
    if (reply == QMessageBox::Yes) {
        qDebug() << QString("Added database with name: %1").arg(databaseName);
        ui->database_creation_list->addItem(databaseName);

        // TODO database create database
        sqlCreateDatabase(databaseName);
    }
}


void DatabaseChooserDialog::delete_database() {
    if (ui->database_creation_list->selectedItems().isEmpty())
        return;

    // TODO database delete database
    QString databaseName = ui->database_creation_list->selectedItems().back()->text();

    deleteMessageBox->databaseName = databaseName;
    deleteMessageBox->databaseNameWithDelete = QString("delete %1").arg(databaseName);
    deleteMessageBox->ui->messagebox_delete_database_edit->clear();
    deleteMessageBox->ui->messagebox_delete_database_confirm->setEnabled(false);
    deleteMessageBox->ui->messagebox_delete_database_label->setText(QString("Введите delete %1").arg(databaseName));
    deleteMessageBox->show();
}

void DatabaseChooserDialog::admin_auth() {
    QString password(ui->admin_auth_button->text());
    if (password.isEmpty())
        return;

    // TODO database check admin password request
    if (sqlAdminAuth(password)) {
        ui->admin_database_creation_stacked_widget->setCurrentIndex(1);
        ui->database_creation_list->clear();
        QList<QString> databases = sqlLoadDatabaseList();
        for (const auto &db : databases) {
            ui->database_creation_list->addItem(db);
        }
    } else {
        qWarning() << QString("Incorrect password: %1").arg(password);
    }
}

void DatabaseChooserDialog::choose_database() {
    if (ui->database_choose_list->selectedItems().isEmpty())
        return;

    // TODO database delete database
    QString databaseName = ui->database_choose_list->selectedItems().back()->text();

    // Throw message box
    QString content("Вы уверены, что хотите выбрать базу данных с названием '%1'?");
    content = content.arg(databaseName);
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Выбор базы данных", content,
                                                              QMessageBox::Yes | QMessageBox::No);

    // If we confirmed choosing
    if (reply == QMessageBox::Yes) {
        qDebug() << QString("Chose database with name: %1").arg(databaseName);

        // TODO database choose database
        sqlChooseDatabase(databaseName);
        close();
    }
}