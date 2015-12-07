#include "run.h"

Run::Run(QString file){
    running = false;
    checkfile = file;
}

void Run::run(){
    //do threading
    running = true;
    bool ok;
    /*qDebug() << "Sort tested...";
    system("sort -u tested.txt > tested.txt.new");
    qDebug() << "Sort input...";
    system(QString("sort -u " + checkfile + " > " + checkfile + ".new").toStdString().c_str());
    qDebug() << "Merge files...";
    system(QString("comm -13 tested.txt.new " + checkfile + ".new > " + checkfile).toStdString().c_str());
    system(QString("mv " + checkfile + ".new " + checkfile).toStdString().c_str());
    system("mv tested.txt.new tested.txt");*/
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
    file.setFileName("complete.txt");
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
        if(count%1000000 == 0){
            QFile file("output_cpp_12.txt");
            file.open(QIODevice::WriteOnly | QIODevice::Append);
            QTextStream out(&file);   // we will serialize the data into the file
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
    file.setFileName("output_cpp_12_sha1.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream out(&file);   // we will serialize the data into the file
    for(int x=0;x<found.size();x++){
        out << found.at(x) << "\n";
    }
    file.close();
    found.clear();

    qDebug() << "Rebuilding left list and update tested plains...";
    system("./rebuild");
    /*system(QString("cat tested.txt " + checkfile + " > tested.txt.new").toStdString().c_str());
    system("mv tested.txt.new tested.txt");*/
    qDebug() << "All done!";
    running = false;
    QCoreApplication::quit();
}

