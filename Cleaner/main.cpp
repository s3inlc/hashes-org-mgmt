#include <QCoreApplication>

#include "run.h"

int main(int argc, char *argv[]){
    QCoreApplication a(argc, argv);

    Run r(argv[1]);
    r.start();

    return a.exec();
}

