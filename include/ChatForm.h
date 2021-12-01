#ifndef CHATDATABASEPROJECT_CHATFORM_H
#define CHATDATABASEPROJECT_CHATFORM_H

#define CHAT_IMAGE_SIZE 50

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
    class ChatForm;
}
QT_END_NAMESPACE

class ChatForm : public QWidget {
Q_OBJECT

private:
    int id;

    QString avatar;

    QString name;

    bool group;

public:
    int getId() const;

    void setId(int id);

    bool isGroup() const;

    void setGroup(bool group);

    void setName(const QString &name);

    void setAvatar(const QString &pathToImage);

public:
    Ui::ChatForm *ui;

    explicit ChatForm(QWidget *parent = nullptr);

    ~ChatForm() override;

    void mouseReleaseEvent(QMouseEvent *event) override;

};

#endif //CHATDATABASEPROJECT_CHATFORM_H
