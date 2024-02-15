#include "MainWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w{QStringLiteral("system.db")};
    w.show();
    return a.exec();
}
