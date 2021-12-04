#include "MainWindow.h"
#include "ui_mainwindow.h"
#include <QScrollBar>

STATE MainWindow::currentState = AUTHORIZATION;

ChatWidget *MainWindow::currentChat = nullptr;

UserInfo *MainWindow::currentUser = nullptr;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    //TODO
    avatarEditor = new AvatarEditor(this);
    //avatarEditor->loadImageIntoScene(":chatDefaultImage.png");
    //avatarEditor->show();

    currentState = AUTHORIZATION;
    currentUser = new UserInfo();
    (currentChat = new ChatWidget(this))->hide(); // TODO parent?
    //currentChat->hide();
    chatDialog = new ChatDialog(this);
    chatDialog->installEventFilter(this);

    // Setting up interface and show default page (authorization)
    ui->setupUi(this);
    ui->app_stacked_widget->setCurrentIndex(APP_PAGE);
    ui->authentification_stacked_widget->setCurrentIndex(AUTHORIZATION_PAGE);
    ui->main_stacked_widget->setCurrentIndex(CHAT_LIST_PAGE);

    int defaultRowNumber = 1;
    ui->message_text_edit->setFixedHeight(getNewEditTextHeight(ui->message_text_edit->document()->size(),
                                                               ui->message_text_edit, defaultRowNumber));
    ui->messageList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->messageList->verticalScrollBar()->setSingleStep(10);

    QSizePolicy sp_retain = ui->listWidget_4->sizePolicy(); // TODO refactor
    sp_retain.setRetainSizeWhenHidden(true);
    ui->listWidget_4->setSizePolicy(sp_retain);
    ui->listWidget_4->hide(); // TODO delete widget?

    QSizePolicy retain = ui->auth_register_error_label->sizePolicy();
    retain.setRetainSizeWhenHidden(true);
    ui->auth_register_error_label->setSizePolicy(retain);
    ui->auth_register_error_label->hide();


    // Test
    for (int i = 0; i < 5; ++i) {
        addChat(i, QString::fromStdString(std::to_string(i)), QImage(":chatDefaultImage"), true, 0, PARTICIPANT);
    }
    addChat(5, "5", QImage(":chatDefaultImage"), true, 0, VIEWER);
    addChat(6, "6", QImage(":chatDefaultImage"), true, 0, ADMIN);
    //updateChat(2, nullptr, QImage(":chatDefaultImage"), PARTICIPANT);


    connect(ui->sign_in_button, SIGNAL(released()), this, SLOT(sign_in_button_released()));
    connect(ui->register_button, SIGNAL(released()), this, SLOT(register_button_released()));
    connect(ui->switch_auth_button, SIGNAL(released()), this, SLOT(switch_auth_button_released()));
    connect(ui->switch_register_button, SIGNAL(released()), this, SLOT(switch_register_button_released()));
    connect(ui->chat_name_label, SIGNAL(released()), this, SLOT(chat_name_label_released()));
    connect(ui->chats_button, SIGNAL(released()), this, SLOT(chats_button_released()));
    connect(ui->chat_back_button, SIGNAL(released()), this, SLOT(chats_button_released()));
    connect(ui->message_text_edit, SIGNAL(returnKeyPressedEvent()), this, SLOT(sendMessage()));
    connect(ui->send_message_button, SIGNAL(released()), this, SLOT(sendMessage()));
    connect(ui->auth_password_edit, SIGNAL(returnPressed()), this, SLOT(sign_in_button_released()));
    connect(ui->auth_email_edit, SIGNAL(returnPressed()), this, SLOT(sign_in_button_released()));
    connect(ui->register_username_edit, SIGNAL(returnPressed()), this, SLOT(sign_in_button_released()));
    connect(ui->register_password_edit, SIGNAL(returnPressed()), this, SLOT(sign_in_button_released()));
    connect(ui->register_email_edit, SIGNAL(returnPressed()), this, SLOT(sign_in_button_released()));
    connect(ui->message_text_edit->document()->documentLayout(), SIGNAL(documentSizeChanged(QSizeF)),
            this, SLOT(messageTextChanged(QSizeF)));


    // Test
    addMessage(0, "Korostast", "2021-03-31 22:10", QImage(":chatDefaultImage"), "Hello world!", USER_MESSAGE);
    addMessage(1, "Korostast", "2021-03-31 22:11", QImage(":chatDefaultImage"), "Hello world!", USER_MESSAGE);
    QString test("Он белый\n"
                 "Пушистый\n"
                 "Мягкий\n"
                 "Падает красиво\n"
                 "Особенно когда в темноте, медленно-медленно и хлопьями, в ресницах застревающими\n"
                 "Он безумно красивый\n"
                 "И из него можно сделать снежок и запустить в какого-нибудь очень хорошего человека");
    addMessage(2, "Korostast", "2021-03-31 23:59", QImage(":chatDefaultImage"), test,
               USER_MESSAGE);

    QString test2("Он белый"
                  "Пушистый"
                  "Мягкий"
                  "Падает красиво"
                  "Особенно когда в темноте, медленно-медленно и хлопьями, в ресницах застревающими"
                  "Он безумно красивый"
                  "И из него можно сделать снежок и запустить в какого-нибудь очень хорошего человека");
    addMessage(3, "Korostast", "2021-03-31 23:59", QImage(":chatDefaultImage"), test2,
               USER_MESSAGE);

    QString test3(
            "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
            "aaaaaaaaaaaaaaaaaaaaaaaa"
            "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
            "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
            "aaaaaaaaaaaaaaaaaaaaaaa");
    addMessage(3, "Korostast", "2021-03-31 23:59", QImage(":chatDefaultImage"), test3,
               USER_MESSAGE);
}

MainWindow::~MainWindow() {
    delete ui;
}

bool MainWindow::eventFilter(QObject *object, QEvent *event) {
    if (event->type() == QEvent::WindowDeactivate && isActiveWindow() && chatDialog == object) {
        qDebug() << "Focus lost!";
        chatDialog->hide();
        //dialog->setFocus(); TODO why this is necessary?

        return true;
    }
    return false;
}
