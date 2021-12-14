#ifndef CHATDATABASEPROJECT_CLICKABLELABEL_H
#define CHATDATABASEPROJECT_CLICKABLELABEL_H

#include <QLabel>
#include <QWidget>

class ClickableLabel : public QLabel {
Q_OBJECT

public:
    explicit ClickableLabel(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());

    int getPersonalChatUserId() const;

    void setPersonalChatUserId(int personalChatUserId);

signals:

    void released();

protected:
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    int personalChatUserId;
};

#endif //CHATDATABASEPROJECT_CLICKABLELABEL_H
