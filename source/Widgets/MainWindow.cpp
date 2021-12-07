#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "FriendWidget.h"
#include "IncomingRequestWidget.h"
#include <QScrollBar>

STATE MainWindow::currentState = AUTHORIZATION;

ChatWidget *MainWindow::currentChat = nullptr;

UserInfo *MainWindow::currentUser;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    // Initialize Main Window variables
    avatarEditor = new AvatarEditor(this);
    currentState = AUTHORIZATION;
    currentUser = new UserInfo(10, "KorostastTrue", QImage(":chatDefaultImage"), "Hello world!!",
                               nullptr); // TODO it is a test
    (currentChat = new ChatWidget(this))->hide(); // TODO parent?
    chatDialog = new ChatDialog(this);

    // Event filter for chat info dialog
    chatDialog->installEventFilter(this);

    // Setting up interface and show default page (authorization)
    ui->setupUi(this);
    ui->app_stacked_widget->setCurrentIndex(APP_PAGE);
    ui->authentification_stacked_widget->setCurrentIndex(AUTHORIZATION_PAGE);
    ui->main_stacked_widget->setCurrentIndex(CHAT_LIST_PAGE);

    // Establish default size of text edit in chat and speed of vertical scrolling
    int defaultRowNumber = 1;
    ui->message_text_edit->setFixedHeight(getNewEditTextHeight(ui->message_text_edit->document()->size(),
                                                               ui->message_text_edit, defaultRowNumber));
    ui->messageList->verticalScrollBar()->setSingleStep(10);

    // If we hide the widget, layout won't be resized. Thus, other widgets will keep their positions
    QSizePolicy sizePolicy = ui->additional_info_list->sizePolicy();
    sizePolicy.setRetainSizeWhenHidden(true);
    ui->additional_info_list->setSizePolicy(sizePolicy);
    ui->additional_info_list->hide(); // Don't delete this widget. I love it

    QSizePolicy retain = ui->auth_register_error_label->sizePolicy();
    retain.setRetainSizeWhenHidden(true);
    ui->auth_register_error_label->setSizePolicy(retain);
    ui->auth_register_error_label->hide();

    // Connect all main window widgets to the functions
    // Authentication
    connect(ui->auth_password_edit, SIGNAL(returnPressed()), this, SLOT(sign_in_button_released()));
    connect(ui->auth_email_edit, SIGNAL(returnPressed()), this, SLOT(sign_in_button_released()));
    connect(ui->register_username_edit, SIGNAL(returnPressed()), this, SLOT(sign_in_button_released()));
    connect(ui->register_password_edit, SIGNAL(returnPressed()), this, SLOT(sign_in_button_released()));
    connect(ui->register_email_edit, SIGNAL(returnPressed()), this, SLOT(sign_in_button_released()));
    connect(ui->sign_in_button, SIGNAL(released()), this, SLOT(sign_in_button_released()));
    connect(ui->register_button, SIGNAL(released()), this, SLOT(register_button_released()));
    connect(ui->switch_auth_button, SIGNAL(released()), this, SLOT(switch_auth_button_released()));
    connect(ui->switch_register_button, SIGNAL(released()), this, SLOT(switch_register_button_released()));

    // App
    connect(ui->chats_button, SIGNAL(released()), this, SLOT(chats_button_released()));
    connect(ui->profile_button, SIGNAL(released()), this, SLOT(profile_button_released()));
    connect(ui->friends_button, SIGNAL(released()), this, SLOT(friends_button_released()));


    // Chat interface
    connect(ui->chat_name_label, SIGNAL(released()), this, SLOT(chat_name_label_released()));
    connect(ui->chat_back_button, SIGNAL(released()), this, SLOT(chats_button_released()));
    connect(ui->send_message_button, SIGNAL(released()), this, SLOT(sendMessage()));
    connect(ui->search_people_button, SIGNAL(released()), this, SLOT(search_people()));
    connect(ui->search_people_line, SIGNAL(returnPressed()), this, SLOT(search_people()));
    connect(ui->message_text_edit, SIGNAL(returnKeyPressedEvent()), this, SLOT(sendMessage()));
    connect(ui->message_text_edit->document()->documentLayout(), SIGNAL(documentSizeChanged(QSizeF)),
            this, SLOT(messageTextChanged(QSizeF)));

    // Friends
    connect(ui->switch_actual_friends, &QPushButton::released, this, [this]() {
        switch_friends_page(ACTUAL_FRIENDS_PAGE);
    });
    connect(ui->switch_incoming_requests, &QPushButton::released, this, [this]() {
        switch_friends_page(INCOMING_REQUESTS_PAGE);
    });
    connect(ui->switch_outcoming_requests, &QPushButton::released, this, [this]() {
        switch_friends_page(OUTCOMING_REQUESTS_PAGE);
    });
    connect(ui->switch_search_people, &QPushButton::released, this, [this]() {
        switch_friends_page(SEARCH_PEOPLE_PAGE);
    });

    // TODO tests
    // tests begin
    tests(/*tests*/); // tests
    // tests end
}

MainWindow::~MainWindow() {
    delete ui;
    delete currentUser;
}

// This filter monitor event of focus switching when dealing with chat info dialog. Ask Korostast about this mega stupid feature
bool MainWindow::eventFilter(QObject *object, QEvent *event) {
    if (event->type() == QEvent::WindowDeactivate && isActiveWindow() && chatDialog == object) {
        qDebug() << "Focus lost!";
        chatDialog->hide();

        return true;
    }
    return false;
}

// Debug functions behavior
void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    QByteArray localMsg = msg.toLocal8Bit();
    const char *file = context.file ? context.file : "";
    const char *function = context.function ? context.function : "";
    switch (type) {
        case QtDebugMsg:
            fprintf(stderr, "DEBUG: %s (in func '%s')\n\n", localMsg.constData(), function);
            break;
        case QtInfoMsg:
            fprintf(stderr, "INFO: %s\n\n", localMsg.constData());
            break;
        case QtWarningMsg:
            fprintf(stderr, "WARNING: %s (%s:%u, %s)\n\n", localMsg.constData(), file, context.line, function);
            break;
        case QtCriticalMsg:
            fprintf(stderr, "CRITICAL: %s (%s:%u, %s)\n\n", localMsg.constData(), file, context.line, function);
            break;
        case QtFatalMsg:
            fprintf(stderr, "FATAL: %s (%s:%u, %s)\n\n", localMsg.constData(), file, context.line, function);
            break;
    }
}