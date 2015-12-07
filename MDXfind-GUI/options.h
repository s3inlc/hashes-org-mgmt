/*
 * Created 2014 by Sein Coray
 *
 * https://github.com/s3inlc/hashes-org-mgmt/tree/master/MDXfind-GUI
 */

#ifndef OPTIONS_H
#define OPTIONS_H

#include <QObject>
#include <QApplication>
#include <QFile>
#include <QDebug>

struct Opt{
    QString ident;
    QString val;
};

class Options : public QObject
{
    Q_OBJECT
public:
    explicit Options(QObject *parent = 0);
    void setOption(QString ident, QString val);
    QString getOption(QString ident);

signals:

public slots:

private:
    void load();
    void save();
    QString path;
    QList<Opt> options;
};

#endif // OPTIONS_H
