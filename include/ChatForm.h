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

public:
    Ui::ChatForm *ui;

    explicit ChatForm(QWidget *parent = nullptr);

    ~ChatForm() override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void setChatName(const QString& name) const;

    void setChatAvatar(const QString& pathToImage, int size) const;

    static QPixmap getCircularPixmap(const QString &pathToImage, int size) ;
};

#endif //CHATDATABASEPROJECT_CHATFORM_H
