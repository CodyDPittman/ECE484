#include "mainwindow.h"
#include "myudp.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //MyUDP client;

    w.show();
    w.initSocket();

    //client.sendNumber();

    return a.exec();
}
