#ifndef CHATDATABASEPROJECT_CUSTOMPLAINTEXTEDIT_H
#define CHATDATABASEPROJECT_CUSTOMPLAINTEXTEDIT_H

#include <QPlainTextEdit>
#include <QWidget>

class CustomPlainTextEdit : public QPlainTextEdit {
Q_OBJECT

public:
    explicit CustomPlainTextEdit(QWidget *parent); // TODO refactor

private:
    void keyPressEvent(QKeyEvent *event) override;

signals:
    void returnKeyPressedEvent();

};

#endif //CHATDATABASEPROJECT_CUSTOMPLAINTEXTEDIT_H
