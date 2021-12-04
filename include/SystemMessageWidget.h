#ifndef CHATDATABASEPROJECT_SYSTEMMESSAGEWIDGET_H
#define CHATDATABASEPROJECT_SYSTEMMESSAGEWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
    class SystemMessageWidget;
}
QT_END_NAMESPACE

class SystemMessageWidget : public QWidget {
Q_OBJECT

private:
    Ui::SystemMessageWidget *ui;

    int id;

    QString content;

public:
    explicit SystemMessageWidget(QWidget *parent = nullptr);

    ~SystemMessageWidget() override;

    int getId() const;

    void setId(int id);

    const QString &getContent() const;

    void setContent(const QString &content);

};

#endif //CHATDATABASEPROJECT_SYSTEMMESSAGEWIDGET_H
