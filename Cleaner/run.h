/*
 * Created 2015 by Sein Coray
 *
 * https://github.com/s3inlc/hashes-org-mgmt/tree/master/Cleaner
 */

#ifndef RUN_H
#define RUN_H

#include <QThread>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QCryptographicHash>
#include <QHash>
#include <QDebug>
#include <QCoreApplication>

class Run: public QThread
{
public:
    Run(QString file, QString out, QString left);

signals:

public slots:

private:
    void run();
    bool running;
    QString checkfile, outfile, leftfile;
};

#endif // RUN_H
