/*
 * Created 2014 by Sein Coray
 *
 * https://github.com/s3inlc/hashes-org-mgmt/tree/master/MDXfind-GUI
 */

#include "options.h"

Options::Options(QObject *parent) : QObject(parent){
    path = QApplication::applicationDirPath() + "/mdxfind-gui.ini";
    load();
}

void Options::load(){
    options.clear();
    QFile file(path);
    if(!file.exists()){
        return;
    }
    file.open(QIODevice::ReadOnly);
    QString content(file.readAll());
    content = content.replace("\r\n", "\n");
    QStringList cols = content.split("\n");
    int count = 0;
    for(int x=0;x<cols.size();x++){
        if(cols.at(x).length() == 0){
            continue;
        }
        QStringList op = cols.at(x).split("=>");
        if(op.size() != 2){
            qDebug() << "Invalid options '" << cols.at(x) << "'!";
            continue;
        }
        setOption(op.at(0), op.at(1));
        count++;
    }
    qDebug() << count << "valid options read, " << options.size() << "options loaded!";
}

void Options::save(){
    QString content = "";
    for(int x=0;x<options.size();x++){
        content += options.at(x).ident + "=>" + options.at(x).val + "\n";
    }
    QFile file(path);
    file.open(QIODevice::WriteOnly);
    file.write(content.toUtf8());
    file.close();
}

void Options::setOption(QString ident, QString val){
    for(int x=0;x<options.size();x++){
        if(options.at(x).ident.compare(ident) == 0){
            options.removeAt(x);
            break;
        }
    }
    Opt n;
    n.ident = ident;
    n.val = val;
    options.append(n);
    save();
}

QString Options::getOption(QString ident){
    for(int x=0;x<options.size();x++){
        if(options.at(x).ident.compare(ident) == 0){
            return options.at(x).val;
        }
    }
    return "";
}
