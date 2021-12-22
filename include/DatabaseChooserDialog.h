#ifndef CHATDATABASEPROJECT_DATABASECHOOSERDIALOG_H
#define CHATDATABASEPROJECT_DATABASECHOOSERDIALOG_H

#include <QDialog>
#include "DeleteDatabaseMessagebox.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class DatabaseChooserDialog;
}
QT_END_NAMESPACE

class DatabaseChooserDialog : public QDialog {
Q_OBJECT

public:
    explicit DatabaseChooserDialog(QWidget *parent = nullptr);

    ~DatabaseChooserDialog() override;

    Ui::DatabaseChooserDialog *ui;

    DeleteDatabaseMessagebox *deleteMessageBox;

private:
    bool isDatabaseNameCorrect(const QString& databaseName);

    void closeEvent(QCloseEvent *event) override;

private slots:

    void add_database();

    void delete_database();

    void admin_auth();

    void choose_database();

};


#endif //CHATDATABASEPROJECT_DATABASECHOOSERDIALOG_H