#include "MainWindow.h"
#include "ui_mainwindow.h"
#include <QFile>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QFile file(":QSS");
    file.open(QFile::ReadOnly);
    a.setStyleSheet(QLatin1String(file.readAll()));

    qInstallMessageHandler(myMessageOutput);

    QFontDatabase::addApplicationFont(":Segoe UI");
    QFontDatabase::addApplicationFont(":Segoe UI Bold");
    QFontDatabase::addApplicationFont(":Roboto");

    MainWindow window;
    window.show();
    return QApplication::exec();
}
