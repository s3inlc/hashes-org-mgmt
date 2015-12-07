/*
 * Created 2014 by Sein Coray
 *
 * https://github.com/s3inlc/hashes-org-mgmt/tree/master/MDXfind-GUI
 */

#include "start.h"
#include <QApplication>
#include <QFontDatabase>

int main(int argc, char *argv[]){
    QApplication a(argc, argv);

    Start w;
    w.show();

    return a.exec();
}
