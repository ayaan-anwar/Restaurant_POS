#include "mainwindow.h"
#include <iostream> // +
#include <string> // +
#include <QApplication>

int main(int argc, char *argv[])
{
    std::string pass;
    do
    {
    std::cout << "Enter password: ";
    std::cin >> pass;
    } while (pass != "1234");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
