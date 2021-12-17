#ifndef CHATDATABASEPROJECT_CLICKABLELABEL_H
#define CHATDATABASEPROJECT_CLICKABLELABEL_H

#include <QLabel>
#include <QWidget>

class ClickableLabel : public QLabel {
Q_OBJECT

public:
    explicit ClickableLabel(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());

    int getPositiveIntData() const;

    void setPositiveIntData(int intData);

signals:

    void released();

protected:
    void mouseReleaseEvent(QMouseEvent *event) override;

    void enterEvent(QEnterEvent *event) override;

    void leaveEvent(QEvent *event) override;

private:
    int intPositiveData;
};

#endif //CHATDATABASEPROJECT_CLICKABLELABEL_H