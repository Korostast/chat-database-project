#ifndef CHATDATABASEPROJECT_CUSTOMPLAINTEXTEDIT_H
#define CHATDATABASEPROJECT_CUSTOMPLAINTEXTEDIT_H

#include <QPlainTextEdit>
#include <QWidget>

class CustomPlainTextEdit : public QPlainTextEdit {
Q_OBJECT

public:
    explicit CustomPlainTextEdit(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());

    ~CustomPlainTextEdit();

private:
    void keyPressEvent(QKeyEvent *event);

signals:
    void returnKeyPressedEvent();

};

#endif //CHATDATABASEPROJECT_CUSTOMPLAINTEXTEDIT_H
