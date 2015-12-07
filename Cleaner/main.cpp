/*
 * Created 2015 by Sein Coray
 *
 * https://github.com/s3inlc/hashes-org-mgmt/tree/master/Cleaner
 */

#include <QCoreApplication>

#include "run.h"

int main(int argc, char *argv[]){
    QCoreApplication a(argc, argv);

    //command line: ./Cleaner [checkfile] [outfile] [leftlist]
    Run r(argv[1], argv[2], argv[3]);
    r.start();

    return a.exec();
}

