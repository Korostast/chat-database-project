#include "MainWindow.h"
#include "ui_mainwindow.h"

void MainWindow::profile_button_released() {
    ui->main_stacked_widget->setCurrentIndex(PROFILE_PAGE);
    currentChat = nullptr;
    currentState = PROFILE;
}
