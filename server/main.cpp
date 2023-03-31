#include <QCoreApplication>
#include "server.h"
int main(int argc, char *argv[]){
    QCoreApplication a(argc, argv);
    SERVER server;

    return a.exec();
}
