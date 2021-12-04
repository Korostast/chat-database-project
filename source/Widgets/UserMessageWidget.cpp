#include "ui_usermessageform.h"
#include "UserMessageWidget.h"
#include "MainWindow.h"

UserMessageWidget::UserMessageWidget(QWidget *parent) : QWidget(parent), ui(new Ui::UserMessageWidget), id(-1) {
    ui->setupUi(this);
}

UserMessageWidget::~UserMessageWidget() {
    delete ui;
}

int UserMessageWidget::getId() const {
    return id;
}

void UserMessageWidget::setId(int id) {
    UserMessageWidget::id = id;
}

const QImage & UserMessageWidget::getAvatar() const {
    return avatar;
}

void UserMessageWidget::setAvatar(const QImage &image) {
    UserMessageWidget::avatar = image;
    ui->message_avatar->setPixmap(AvatarEditor::getCircularPixmap(image, USER_MESSAGE_IMAGE_SIZE));
}

const QString &UserMessageWidget::getName() const {
    return name;
}

void UserMessageWidget::setName(const QString &name) {
    UserMessageWidget::name = name;
    ui->message_username->setText(name);
}

const QString &UserMessageWidget::getTime() const {
    return time;
}

void UserMessageWidget::setTime(const QString &time) {
    UserMessageWidget::time = time;
    ui->message_time->setText(time);
}

const QString &UserMessageWidget::getContent() const {
    return content;
}

void UserMessageWidget::setContent(const QString &content) {
    UserMessageWidget::content = content;
    ui->message_content->setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    ui->message_content->setPlainText(content);
    qDebug() << "Doc size:" << ui->message_content->document()->size().toSize();

    int nRows = countRows(content);
    qDebug() << "Rows:" << nRows;
    /*QMargins margins = ui->message_content->contentsMargins ();
    QTextDocument *pdoc = ui->message_content->document();
    QFontMetrics fm (pdoc->defaultFont ());
    int nHeight = fm.lineSpacing () * nRows +
                  (pdoc->documentMargin () + ui->message_content->frameWidth ()) * 2 +
                  margins.top () + margins.bottom ();*/

    QMargins margins = ui->message_content->contentsMargins ();
    auto widgetMargins = contentsMargins();
    auto documentMargin = ui->message_content->document()->documentMargin();
    auto fontMetrics = QFontMetrics(ui->message_content->document()->defaultFont());
    auto newHeight = margins.top() +
                     documentMargin +
                     (nRows + 1) * fontMetrics.height() +
                     ui->message_content->document()->documentMargin() +
                     widgetMargins.bottom();


    ui->message_content->setFixedHeight((int) newHeight);
}

int UserMessageWidget::countRows(const QString &text) {
    QStringList lines = text.split("\n");
    int answer = (int) lines.size();
    for (const auto& line : lines) {
        QTextDocument document(line);
        document.setDefaultFont(QFont("Segoe UI", 12)); // TODO hardcoded font
        qDebug() << "+ answer:" << (int) document.size().width() / MESSAGE_LINE_WIDTH;
        qDebug() << "+ answer:" << (int) document.size().width();
        qDebug() << "+ answer:" << MESSAGE_LINE_WIDTH;
        answer += (int) document.size().width() / MESSAGE_LINE_WIDTH;
    }
    return answer;
}
