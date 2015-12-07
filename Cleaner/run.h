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
    Run(QString file);

signals:

public slots:

private:
    void run();
    bool running;
    QString checkfile;
};

#endif // RUN_H
