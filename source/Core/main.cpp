#include "MainWindow.h"
#include "ui_mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    qInstallMessageHandler(myMessageOutput);
    QFontDatabase::addApplicationFont(":Roboto font");
    MainWindow window;
    window.show();
    return QApplication::exec();
}
