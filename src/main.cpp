#include "mainwindow.h"

#include <QApplication>

#ifdef Q_OS_WIN
    #include <windows.h>
#endif



int main(int argc, char *argv[])
{
#ifdef Q_OS_WIN
    SetConsoleOutputCP(CP_UTF8);
#endif

    QApplication a(argc, argv);
    a.setWindowIcon(QIcon("icons/icon.ico"));

    MainWindow w;
    w.setWindowTitle("DocPattern");
    w.show();
    return a.exec();
}
