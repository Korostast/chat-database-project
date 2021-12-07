#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "FriendWidget.h"
#include "IncomingRequestWidget.h"
#include <QScrollBar>
#include <QFile>

STATE MainWindow::currentState = AUTHORIZATION;

ChatWidget *MainWindow::currentChat = nullptr;

UserInfo *MainWindow::currentUser = nullptr;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    // Open the file containing connection data
    QFile qFile("db_access.txt");
    qFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream qTextStream(&qFile);

    // Set connection parameters from the file
    db = QSqlDatabase::addDatabase(qTextStream.readLine());
    db.setHostName(qTextStream.readLine());
    db.setDatabaseName(qTextStream.readLine());
    db.setUserName(qTextStream.readLine());
    db.setPassword(qTextStream.readLine());

    // Attempt connection
    if (!db.open()) {
        qCritical() << "Cannot connect to database\n";
        exit(1);
    }
    qDebug() << "Succesfully connected to the database\n";



    //TODO
    avatarEditor = new AvatarEditor(this);

    currentState = AUTHORIZATION;
    currentUser = new UserInfo();
    (currentChat = new ChatWidget(this))->hide(); // TODO parent?
    chatDialog = new ChatDialog(this);
    chatDialog->installEventFilter(this);

    // Setting up interface and show default page (authorization)
    ui->setupUi(this);
    ui->app_stacked_widget->setCurrentIndex(AUTHORIZATION_PAGE);
    ui->authentification_stacked_widget->setCurrentIndex(AUTHORIZATION_PAGE);
    ui->main_stacked_widget->setCurrentIndex(CHAT_LIST_PAGE);

    int defaultRowNumber = 1;
    ui->message_text_edit->setFixedHeight(getNewEditTextHeight(ui->message_text_edit->document()->size(),
                                                               ui->message_text_edit, defaultRowNumber));
    ui->messageList->verticalScrollBar()->setSingleStep(10);

    QSizePolicy sp_retain = ui->listWidget_4->sizePolicy(); // TODO refactor
    sp_retain.setRetainSizeWhenHidden(true);
    ui->listWidget_4->setSizePolicy(sp_retain);
    ui->listWidget_4->hide(); // TODO delete widget?

    QSizePolicy retain = ui->auth_register_error_label->sizePolicy();
    retain.setRetainSizeWhenHidden(true);
    ui->auth_register_error_label->setSizePolicy(retain);
    ui->auth_register_error_label->hide();

    connect(ui->sign_in_button, SIGNAL(released()), this, SLOT(sign_in_button_released()));
    connect(ui->register_button, SIGNAL(released()), this, SLOT(register_button_released()));
    connect(ui->switch_auth_button, SIGNAL(released()), this, SLOT(switch_auth_button_released()));
    connect(ui->switch_register_button, SIGNAL(released()), this, SLOT(switch_register_button_released()));
    connect(ui->chat_name_label, SIGNAL(released()), this, SLOT(chat_name_label_released()));
    connect(ui->chats_button, SIGNAL(released()), this, SLOT(chats_button_released()));
    connect(ui->profile_button, SIGNAL(released()), this, SLOT(profile_button_released()));
    connect(ui->friends_button, SIGNAL(released()), this, SLOT(friends_button_released()));

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

    connect(ui->chat_back_button, SIGNAL(released()), this, SLOT(chats_button_released()));
    connect(ui->message_text_edit, SIGNAL(returnKeyPressedEvent()), this, SLOT(sendMessage()));
    connect(ui->send_message_button, SIGNAL(released()), this, SLOT(sendMessage()));
    connect(ui->auth_password_edit, SIGNAL(returnPressed()), this, SLOT(sign_in_button_released()));
    connect(ui->auth_email_edit, SIGNAL(returnPressed()), this, SLOT(sign_in_button_released()));
    connect(ui->register_username_edit, SIGNAL(returnPressed()), this, SLOT(sign_in_button_released()));
    connect(ui->register_password_edit, SIGNAL(returnPressed()), this, SLOT(sign_in_button_released()));
    connect(ui->register_email_edit, SIGNAL(returnPressed()), this, SLOT(sign_in_button_released()));
    connect(ui->search_people_button, SIGNAL(released()), this, SLOT(search_people()));
    connect(ui->search_people_line, SIGNAL(returnPressed()), this, SLOT(search_people()));
    connect(ui->message_text_edit->document()->documentLayout(), SIGNAL(documentSizeChanged(QSizeF)),
            this, SLOT(messageTextChanged(QSizeF)));

    tests();
}

MainWindow::~MainWindow() {
    db.close();
    qDebug() << "Database connection closed\n";
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