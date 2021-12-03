#ifndef CHATDATABASEPROJECT_CHATFORM_H
#define CHATDATABASEPROJECT_CHATFORM_H

#define CHAT_IMAGE_SIZE 50

#include <QWidget>

enum ROLE {
    ADMIN,
    PARTICIPANT,
    VIEWER
};

QT_BEGIN_NAMESPACE
namespace Ui {
    class ChatForm;
}
QT_END_NAMESPACE

class ChatForm : public QWidget {
Q_OBJECT

private:
    int id;

    QImage avatar;

    QString name;

    bool group;

    int countMembers;

    ROLE role;

public:

    int getId() const;

    const QImage &getAvatar() const;

    const QString &getName() const;

    int getCountMembers() const;

    void setCountMembers(int countMembers);

    ROLE getRole() const;

    void setRole(ROLE role);

    void setId(int id);

    bool isGroup() const;

    void setGroup(bool group);

    void setName(const QString &name);

public:
    Ui::ChatForm *ui;

    explicit ChatForm(QWidget *parent = nullptr);

    ~ChatForm() override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void setAvatar(const QImage &image);
};

#endif //CHATDATABASEPROJECT_CHATFORM_H
