#ifndef CHATDATABASEPROJECT_DELETEDATABASEMESSAGEBOX_H
#define CHATDATABASEPROJECT_DELETEDATABASEMESSAGEBOX_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
    class DeleteDatabaseMessagebox;
}
QT_END_NAMESPACE

class DeleteDatabaseMessagebox : public QDialog {
Q_OBJECT

public:
    QString databaseName;

    QString databaseNameWithDelete;

    explicit DeleteDatabaseMessagebox(QWidget *parent = nullptr);

    ~DeleteDatabaseMessagebox() override;

    Ui::DeleteDatabaseMessagebox *ui;

private slots:

    void delete_database_edit_text_changed(const QString &) const;

    void confirm_delete_database();

};

#endif //CHATDATABASEPROJECT_DELETEDATABASEMESSAGEBOX_H