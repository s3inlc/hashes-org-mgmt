/*
 * Created 2015 by Sein Coray
 *
 * https://github.com/s3inlc/hashes-org-mgmt/tree/master/Cleaner
 */

#include "run.h"

Run::Run(QString file, QString out, QString left){
    running = false;
    checkfile = file;
    outfile = out;
    leftfile = left;
}

void Run::run(){
    //do threading
    running = true;
    bool ok;
    qDebug() << "Start reading file...";
    QFile file(checkfile);
    file.open(QIODevice::ReadOnly);
    QString s;
    QTextStream s1(&file);
    s.append(s1.readAll());
    qDebug() << "All input read!";
    QStringList input = s.replace("\r\n", "\n").split("\n");
    int count = 0;
    qDebug() << "Creating indizes...";
    QList<QHash<long long int, QString>* > dataarray;
    for(int x=0;x<256*16;x++){
        QHash<long long int, QString> *arr = new QHash<long long int, QString>();
        dataarray.append(arr);
    }
    qDebug() << "Process lines...";
    for(int x=0;x<input.size();x++){
        count++;
        if(count%10000 == 0){
            qDebug() << input.at(x);
        }
        QString hash = QString(QCryptographicHash::hash((input.at(x).toUtf8()),QCryptographicHash::Md5).toHex());
        long long int convert = hash.mid(0, 12).toLongLong(&ok, 16);
        dataarray.at(hash.mid(0, 3).toInt(&ok, 16))->insert(convert, input.at(x));
        convert = hash.mid(8, 12).toLongLong(&ok, 16);
        dataarray.at(hash.mid(8, 3).toInt(&ok, 16))->insert(convert, input.at(x));
        convert = hash.mid(16, 12).toLongLong(&ok, 16);
        dataarray.at(hash.mid(16, 3).toInt(&ok, 16))->insert(convert, input.at(x));
        convert = hash.mid(22).toLongLong(&ok, 16);
        dataarray.at(hash.mid(22, 3).toInt(&ok, 16))->insert(convert, input.at(x));
    }

    qDebug() << "Plains loaded!" << count;

    count = 0;
    file.close();
    file.setFileName(leftfile);
    file.open(QIODevice::ReadOnly);
    QString line = file.readLine();
    QStringList found;
    int foundcount = 0;
    while(!file.atEnd()){
        line = line.trimmed();
        count++;
        if(count%10000 == 0){
            qDebug() << line << found.size() << foundcount;
        }
        if(count%1000000 == 0){ //flush after some progress
            QFile file(outfile);
            file.open(QIODevice::WriteOnly | QIODevice::Append);
            QTextStream out(&file);
            for(int x=0;x<found.size();x++){
                out << found.at(x) << "\n";
            }
            file.close();
            found.clear();
        }
        if(line.length() == 0){
            line = file.readLine();
            continue;
        }
        else if(line.contains("0000000000000000000000")){
            line = file.readLine();
            continue;
        }
        for(int x=0;x<line.length() - 12;x++){
            QString part = line.mid(x, 12);
            if(dataarray.at(part.mid(0, 3).toInt(&ok, 16))->contains(part.toLongLong(&ok, 16))){
                QString plain = dataarray.at(part.mid(0, 3).toInt(&ok, 16))->value(part.toLongLong(&ok, 16));
                QString hash = QString(QCryptographicHash::hash((plain.toUtf8()),QCryptographicHash::Md5).toHex());
                found.append(line + ":" + hash + ":" + plain);
                foundcount++;
                break;
            }
        }
        line = file.readLine();
    }
    file.close();
    file.setFileName(outfile);
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream out(&file);
    for(int x=0;x<found.size();x++){
        out << found.at(x) << "\n";
    }
    file.close();
    found.clear();
    qDebug() << "All done!";
    running = false;
    QCoreApplication::quit();
}

