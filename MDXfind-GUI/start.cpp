/*
 * Created 2014 by Sein Coray
 *
 * https://github.com/s3inlc/hashes-org-mgmt/tree/master/MDXfind-GUI
 */

#include "start.h"
#include "ui_start.h"

Start::Start(QWidget *parent) : QMainWindow(parent), ui(new Ui::Start){
    ui->setupUi(this);

    //load all options
    ui->hashFile->setText(opts.getOption("hashfile"));
    ui->preHashTypes->setCurrentIndex(opts.getOption("prehash").toInt());
    ui->hashTypes->setText(opts.getOption("hash"));
    if(opts.getOption("salt").compare("1") == 0){
        ui->setSaltFile->setChecked(true);
    }
    ui->saltFile->setText(opts.getOption("saltfile"));
    if(opts.getOption("userid").compare("1") == 0){
        ui->setUserIDFile->setChecked(true);
    }
    ui->userIDFile->setText(opts.getOption("useridfile"));
    if(opts.getOption("rot").compare("1") == 0){
        ui->setRotation->setChecked(true);
    }
    ui->rotation->setText(opts.getOption("rotation"));
    if(opts.getOption("append").compare("1") == 0){
        ui->setAppendNum->setChecked(true);
    }
    ui->appendNum->setText(opts.getOption("appendnum"));
    if(opts.getOption("setiter").compare("1") == 0){
        ui->setIterations->setChecked(true);
    }
    ui->iterations->setText(opts.getOption("iterations"));
    if(opts.getOption("replacexml").compare("1") == 0){
        ui->setReplaceXML->setChecked(true);
    }
    if(opts.getOption("addclrf").compare("1") == 0){
        ui->setAddCLRF->setChecked(true);
    }
    if(opts.getOption("useunicode").compare("1") == 0){
        ui->setUseUnicode->setChecked(true);
    }
    if(opts.getOption("threads").compare("1") == 0){
        ui->setNumThreads->setChecked(true);
    }
    ui->numThreads->setText(opts.getOption("numthreads"));
    if(opts.getOption("userules").compare("1") == 0){
        ui->setUseRules->setChecked(true);
    }
    ui->rules->setText(opts.getOption("rules"));
    if(opts.getOption("setperm").compare("1") == 0){
        ui->setPermutationFile->setChecked(true);
    }
    ui->permutationFile->setText(opts.getOption("permutation"));
    if(opts.getOption("savefound").compare("1") == 0){
        ui->setSaveFounds->setChecked(true);
    }
    ui->saveFounds->setText(opts.getOption("savepath"));
    ui->mdxfindLocation->setText(opts.getOption("mdxfind"));
    if(opts.getOption("clean").compare("1") == 0){
        ui->setClean->setChecked(true);
    }
    ui->setCleanPath->setText(opts.getOption("cleanpath"));

    updateWordlists();

    ui->wordlistsNo->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->wordlistsYes->setSelectionMode(QAbstractItemView::ExtendedSelection);

    block = false;
    running = false;
    prg = NULL;
}

Start::~Start(){
    delete ui;
}

void Start::on_wordlistsLeft_clicked(){
    QList<QListWidgetItem*> items = ui->wordlistsYes->selectedItems();
    QStringList wordlists = opts.getOption("wordlistsyes").split(";");
    for(int x=0;x<items.size();x++){
        QListWidgetItem *item = items.at(x);
        for(int y=0;y<wordlists.size();y++){
            if(wordlists.at(y).compare(item->text()) == 0){
                wordlists.removeAt(y);
                opts.setOption("wordlistsno", opts.getOption("wordlistsno") + item->text() + ";");
                break;
            }
        }
    }
    opts.setOption("wordlistsyes", wordlists.join(";"));
    updateWordlists();
}

void Start::on_wordlistsRight_clicked(){
    QList<QListWidgetItem*> items = ui->wordlistsNo->selectedItems();
    QStringList wordlists = opts.getOption("wordlistsno").split(";");
    for(int x=0;x<items.size();x++){
        QListWidgetItem *item = items.at(x);
        for(int y=0;y<wordlists.size();y++){
            if(wordlists.at(y).compare(item->text()) == 0){
                wordlists.removeAt(y);
                opts.setOption("wordlistsyes", opts.getOption("wordlistsyes") + item->text() + ";");
                break;
            }
        }
    }
    opts.setOption("wordlistsno", wordlists.join(";"));
    updateWordlists();
}

void Start::on_wordlistsRemove_clicked(){
    QList<QListWidgetItem*> items = ui->wordlistsNo->selectedItems();
    QStringList wordlists = opts.getOption("wordlistsno").split(";");
    for(int x=0;x<items.size();x++){
        QListWidgetItem *item = items.at(x);
        for(int y=0;y<wordlists.size();y++){
            if(wordlists.at(y).compare(item->text()) == 0){
                wordlists.removeAt(y);
                break;
            }
        }
    }
    opts.setOption("wordlistsno", wordlists.join(";"));

    items = ui->wordlistsYes->selectedItems();
    wordlists = opts.getOption("wordlistsyes").split(";");
    for(int x=0;x<items.size();x++){
        QListWidgetItem *item = items.at(x);
        for(int y=0;y<wordlists.size();y++){
            if(wordlists.at(y).compare(item->text()) == 0){
                wordlists.removeAt(y);
                break;
            }
        }
    }
    opts.setOption("wordlistsyes", wordlists.join(";"));

    updateWordlists();
}

void Start::updateWordlists(){
    ui->wordlistsNo->clear();
    QString lno = opts.getOption("wordlistsno");
    QStringList nolists = lno.split(";");
    for(int x=0;x<nolists.size();x++){
        if(nolists.at(x).length() == 0){
            continue;
        }
        ui->wordlistsNo->addItem(nolists.at(x));
    }

    ui->wordlistsYes->clear();
    QString lyes = opts.getOption("wordlistsyes");
    QStringList yeslists = lyes.split(";");
    for(int x=0;x<yeslists.size();x++){
        if(yeslists.at(x).length() == 0){
            continue;
        }
        ui->wordlistsYes->addItem(yeslists.at(x));
    }
}

void Start::on_wordlistsAdd_clicked(){
    QStringList paths = QFileDialog::getOpenFileNames(this, "Select wordlist(s)");
    if(paths.size() == 0){
        return;
    }
    for(int x=0;x<paths.size();x++){
        opts.setOption("wordlistsno", opts.getOption("wordlistsno") + paths.at(x) + ";");
    }
    updateWordlists();
}

void Start::on_startProcess_clicked(){
    if(running){
        running = false;
        ui->info->append("MDXfind aborted!!\n");
        prg->kill();
        return;
    }

    QString errors = "<b>There were some errors in your configuration!</b><br><br>";
    bool error = false;
    if(ui->hashFile->text().length() == 0){
        errors += "- Select a hash file<br>";
        error = true;
    }
    else if(!validPath(ui->hashFile->text())){
        errors += "- Hash file not found<br>";
        error = true;
    }
    if(ui->hashTypes->text().length() == 0){
        errors += "- Enter a hashtype regex<br>";
        error = true;
    }
    if(ui->setSaltFile->isChecked() && !validPath(ui->saltFile->text())){
        errors += "- Provide a valid salt file<br>";
        error = true;
    }
    if(ui->setUserIDFile->isChecked() && !validPath(ui->userIDFile->text())){
        errors += "- Provide a valid userID file<br>";
        error = true;
    }
    if(ui->rotation->text().toInt() < 0 && ui->setRotation->isChecked()){
        errors += "- Enter a valid rotation number<br>";
        error = true;
    }
    //test append num
    if(ui->iterations->text().toInt() < 1 && ui->setIterations->isChecked()){
        errors += "- Enter a valid number of iterations<br>";
        error = true;
    }
    if(ui->setNumThreads->isChecked() && ui->numThreads->text().toInt() < 1){
        errors += "- Enter a valid number of threads<br>";
        error = true;
    }
    if(ui->setUseRules->isChecked() && !validPath(ui->rules->text())){
        errors += "- Provide a valid rules file<br>";
        error = true;
    }
    if(ui->setPermutationFile->isChecked() && !validPath(ui->permutationFile->text())){
        errors += "- Provide a valid permutation file<br>";
        error = true;
    }
    if(ui->setSaveFounds->isChecked() && ui->saveFounds->text().length() == 0){
        errors += "- Enter a path to save the founds to<br>";
        error = true;
    }
    if(!validPath(ui->mdxfindLocation->text())){
        errors = "- Provide a valid path for the MDXfind binary<br>";
        error = true;
    }
    if(ui->wordlistsYes->count() < 1){
        errors += "- Select at least one wordlist to use<br>";
        error = true;
    }
    if(running){
        errors += "- There is already a MDXfind process running<br>";
        error = true;
    }

    if(error){
        errMsg("Configuration Error!", errors);
        return;
    }

    QString command = ui->mdxfindLocation->text();
    QStringList args;
    args.append("-h");
    args.append(ui->hashTypes->text());
    args.append("-f");
    args.append(ui->hashFile->text());
    if(ui->setSaltFile->isChecked()){
        args.append("-s");
        args.append(ui->saltFile->text());
    }
    if(ui->setUserIDFile->isChecked()){
        args.append("-u");
        args.append(ui->userIDFile->text());
    }
    if(ui->setRotation->isChecked()){
        args.append("-g");
        args.append(ui->rotation->text());
    }
    if(ui->setAppendNum->isChecked()){
        args.append("-n");
        args.append(ui->appendNum->text());
    }
    if(ui->setIterations->isChecked()){
        args.append("-i");
        args.append(ui->iterations->text());
    }
    if(ui->setReplaceXML->isChecked()){
        args.append("-c");
    }
    if(ui->setAddCLRF->isChecked()){
        args.append("-l");
    }
    if(ui->setUseUnicode->isChecked()){
        args.append("-b");
    }
    if(ui->setNumThreads->isChecked()){
        args.append("-t");
        args.append(ui->numThreads->text());
    }
    if(ui->setUseRules->isChecked()){
        args.append("-r");
        args.append(ui->rules->text());
    }
    if(ui->setPermutationFile->isChecked()){
        args.append("-k");
        args.append(ui->permutationFile->text());
    }

    for(int x=0;x<ui->wordlistsYes->count();x++){
        args.append(ui->wordlistsYes->item(x)->text());
    }

    delete prg;
    prg = new QProcess(this);
    running = true;
    connect(prg, SIGNAL(finished(int)), this, SLOT(prgFinished(int)));
    connect(prg, SIGNAL(readyReadStandardError()), this, SLOT(prgUpdate()));
    if(ui->setSaveFounds->isChecked()){
        prg->setStandardOutputFile(ui->saveFounds->text(), QIODevice::Append);
    }
    prg->start(command, args);
    qDebug() << "Start MDXfind:" << command << args.join(" ");
    ui->statusBar->showMessage("MDXfind is running...");
    ui->startProcess->setText("Abort");
    inprg = false;
    ui->info->clear();
}

void Start::errMsg(QString title, QString msg){
    QMessageBox::question(this, title, msg, QMessageBox::Ok);
}

void Start::prgUpdate(){
    if(!running){
        return;
    }
    QString msg = prg->readAllStandardError();
    qDebug() << msg;
    if(msg.contains("Working on") && !msg.contains("Working on hash types")){
        msg.chop(1);
        if(inprg){
            QTextCursor cursor = ui->info->textCursor();
            cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
            cursor.movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
            cursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
            cursor.removeSelectedText();
            ui->info->setTextCursor(cursor);
            msg = msg.replace("\n", "");
        }
        else{
            inprg = true;
        }
    }
    ui->info->insertPlainText(msg);
    ui->info->textCursor().setPosition(QTextCursor::End);
}

void Start::prgFinished(int code){
    if(ui->setClean->isChecked()){
        qDebug() << "Start left/found cleaning...";
        QString base = ui->setCleanPath->text();
        QString found = base + "_f.txt";
        QString left = base + "_l.txt";
        QString app = QApplication::applicationDirPath();
        system(QString("fromdos " + ui->hashFile->text()).toStdString().c_str());
        system(QString("fromdos " + ui->saveFounds->text()).toStdString().c_str());
        system(QString("sort -u '" + ui->hashFile->text() + "' > '" + app + "/tmp234_l_s.txt'").toStdString().c_str());
        system(QString("sort -u '" + ui->saveFounds->text() + "' > '" + app + "/tmp234_f_s.txt'").toStdString().c_str());
        system(QString("awk '{ print $2 }' '" + app + "/tmp234_f_s.txt' > " + found).toStdString().c_str());
        system(QString("awk -F: '{ print $1 }' '" + found + "' > '" + app + "/tmp234_f_h.txt'").toStdString().c_str());
        system(QString("comm -23 '" + app + "/tmp234_l_s.txt' '" + app + "/tmp234_f_h.txt' > " + left).toStdString().c_str());
        system(QString("rm '" + app + "/tmp234_l_s.txt' '" + app + "/tmp234_f_h.txt' '" + app + "/tmp234_f_s.txt'").toStdString().c_str());
        qDebug() << "Left/found cleaning done!";
    }


    ui->startProcess->setText("Start");
    running = false;
    ui->statusBar->showMessage("MDXfind finished! - Code: " + QString::number(code));
}

bool Start::validPath(QString path){
    QFileInfo file(path);
    if(file.exists() && !file.isDir()){
        return true;
    }
    return false;
}

void Start::testPath(QString path, QLineEdit *line){
    QFileInfo file(path);
    QPalette palette;
    if(file.exists() && !file.isDir()){
        palette.setColor(QPalette::Text, Qt::black);
    }
    else{
        palette.setColor(QPalette::Text, Qt::red);
    }
    line->setPalette(palette);
}

void Start::on_hashFile_textChanged(){
    QString path = ui->hashFile->text();
    testPath(path, ui->hashFile);
    opts.setOption("hashfile", path);
}

void Start::on_selectHashFile_clicked(){
    QString path = QFileDialog::getOpenFileName(this, "Select hash file");
    if(path.length() == 0){
        return;
    }
    testPath(path, ui->hashFile);
    ui->hashFile->setText(path);
}

void Start::on_hashTypes_textChanged(){
    QString hash = ui->hashTypes->text();
    opts.setOption("hash", hash);
    opts.setOption("prehash", "0");
    if(!block){
        ui->preHashTypes->setCurrentIndex(0);
    }
}

void Start::on_preHashTypes_currentIndexChanged(int i){
    QString sel = ui->preHashTypes->currentText();
    if(sel.length() == 0){
        return;
    }
    block = true;
    if(sel.compare("DYN1024") == 0){
        ui->hashTypes->setText("^DYN1024$");
    }
    else if(sel.compare("HAV128_4") == 0){
        ui->hashTypes->setText("^HAV128_4$");
    }
    else if(sel.compare("HAV128_5") == 0){
        ui->hashTypes->setText("^HAV128_5$");
    }
    else if(sel.compare("HAV") == 0){
        ui->hashTypes->setText("^HAV$");
    }
    else if(sel.compare("LM") == 0){
        ui->hashTypes->setText("^LM$");
    }
    else if(sel.compare("MD2") == 0){
        ui->hashTypes->setText("^MD2$");
    }
    else if(sel.compare("MD4") == 0){
        ui->hashTypes->setText("^MD4$");
    }
    else if(sel.compare("MD5-4xMD5") == 0){
        ui->hashTypes->setText("^MD5-4xMD5$");
    }
    else if(sel.compare("MD5BASE64") == 0){
        ui->hashTypes->setText("^MD5BASE64$");
    }
    else if(sel.compare("MD5HEXSALT") == 0){
        ui->hashTypes->setText("^MD5HEXSALT$");
    }
    else if(sel.compare("MD5MD5SALT") == 0){
        ui->hashTypes->setText("^MD5MD5SALT$");
    }
    else if(sel.compare("MD5PASSMD5") == 0){
        ui->hashTypes->setText("^MD5PASSMD5$");
    }
    else if(sel.compare("MD5rev") == 0){
        ui->hashTypes->setText("^MD5rev$");
    }
    else if(sel.compare("MD5SALT") == 0){
        ui->hashTypes->setText("^MD5SALT$");
    }
    else if(sel.compare("MD5SALTPASS") == 0){
        ui->hashTypes->setText("^MD5SALTPASS$");
    }
    else if(sel.compare("MD5SHA1SHA1") == 0){
        ui->hashTypes->setText("^MD5SHA1SHA1$");
    }
    else if(sel.compare("MD5SHA1") == 0){
        ui->hashTypes->setText("^MD5SHA1$");
    }
    else if(sel.compare("MD5SQL5-32") == 0){
        ui->hashTypes->setText("^MD5SQL5-32$");
    }
    else if(sel.compare("MD5USERIDMD5") == 0){
        ui->hashTypes->setText("^MD5USERIDMD5$");
    }
    else if(sel.compare("MD5USERIDMD5MD5") == 0){
        ui->hashTypes->setText("^MD5USERIDMD5MD5$");
    }
    else if(sel.compare("MD5") == 0){
        ui->hashTypes->setText("^MD5$");
    }
    else if(sel.compare("NTLM") == 0){
        ui->hashTypes->setText("^NTLM$");
    }
    else if(sel.compare("RMD128") == 0){
        ui->hashTypes->setText("^RMD128$");
    }
    else if(sel.compare("SHA1BASE64") == 0){
        ui->hashTypes->setText("^SHA1BASE64$");
    }
    else if(sel.compare("SHA1MD5") == 0){
        ui->hashTypes->setText("^SHA1MD5$");
    }
    else if(sel.compare("SHA1RAW") == 0){
        ui->hashTypes->setText("^SHA1RAW$");
    }
    else if(sel.compare("SHA1") == 0){
        ui->hashTypes->setText("^SHA1$");
    }
    else if(sel.compare("SHA256") == 0){
        ui->hashTypes->setText("^SHA256$");
    }
    else if(sel.compare("SHA512") == 0){
        ui->hashTypes->setText("^SHA512$");
    }
    else if(sel.compare("SQL5") == 0){
        ui->hashTypes->setText("^SQL5$");
    }
    else if(sel.compare("TIGER") == 0){
        ui->hashTypes->setText("^TIGER$");
    }
    else if(sel.compare("WRL") == 0){
        ui->hashTypes->setText("^WRL$");
    }


    else if(sel.compare("salted MD5") == 0){
        ui->hashTypes->setText("^MD5PASSSALT$,^MD5SALTPASS$,^MD5SALT$,^MD5HEXSALT$");
    }
    else if(sel.compare("salted SHA1") == 0){
        ui->hashTypes->setText("^SHA1HEXSALT$,^SHA1SALTPASS$,^SHA1PASSSALT$,^SHA1SALTPASSSALT$");
    }
    else if(sel.compare("all quick MD5") == 0){
        ui->hashTypes->setText("^MD5,!SALT,!USER,!MD5x");
    }
    else if(sel.compare("all quick SHA1") == 0){
        ui->hashTypes->setText("^SHA1,!SALT,!USER");
    }
    else if(sel.compare("all quick") == 0){
        ui->hashTypes->setText("^,!SALT,!USER,!MD5x");
    }
    else if(sel.compare("all WRL") == 0){
        ui->hashTypes->setText("^WRL");
    }
    else if(sel.compare("all") == 0){
        ui->hashTypes->setText("^");
    }
    else{
        ui->hashTypes->setText("ERR");
        block = false;
        return;
    }
    opts.setOption("hash", ui->hashTypes->text());
    opts.setOption("prehash", QString::number(i));
    block = false;
}

void Start::on_setSaltFile_toggled(){
    if(ui->setSaltFile->isChecked()){
        opts.setOption("salt", "1");
    }
    else{
        opts.setOption("salt", "0");
    }
}

void Start::on_saltFile_textChanged(){
    QString path = ui->saltFile->text();
    testPath(path, ui->saltFile);
    opts.setOption("saltfile", path);
}

void Start::on_selectSaltFile_clicked(){
    QString path = QFileDialog::getOpenFileName(this, "Select salt file");
    if(path.length() == 0){
        return;
    }
    testPath(path, ui->saltFile);
    ui->saltFile->setText(path);
}

void Start::on_setUserIDFile_toggled(){
    if(ui->setUserIDFile->isChecked()){
        opts.setOption("userid", "1");
    }
    else{
        opts.setOption("userid", "0");
    }
}

void Start::on_userIDFile_textChanged(){
    QString path = ui->userIDFile->text();
    testPath(path, ui->userIDFile);
    opts.setOption("useridfile", path);
}

void Start::on_selectUserIDFile_clicked(){
    QString path = QFileDialog::getOpenFileName(this, "Select userID file");
    if(path.length() == 0){
        return;
    }
    testPath(path, ui->userIDFile);
    ui->setUserIDFile->setChecked(true);
    ui->userIDFile->setText(path);
}

void Start::on_setRotation_toggled(){
    if(ui->setRotation->isChecked()){
        opts.setOption("rot", "1");
    }
    else{
        opts.setOption("rot", "0");
    }
}

void Start::on_setClean_toggled(){
    if(ui->setClean->isChecked()){
        opts.setOption("clean", "1");
    }
    else{
        opts.setOption("clean", "0");
    }
}

void Start::on_selectClean_clicked(){
    QString path = QFileDialog::getSaveFileName(this, "Select base path to save new left list and found");
    if(path.length() == 0){
        return;
    }
    ui->setClean->setChecked(true);
    ui->setCleanPath->setText(path);
}

void Start::on_setCleanPath_textChanged(){
    opts.setOption("cleanpath", ui->setCleanPath->text());
}

void Start::on_rotation_textChanged(){
    int rotation = ui->rotation->text().toInt();
    QString rot = QString::number(rotation);
    opts.setOption("rotation", rot);
    ui->rotation->setText(rot);
}

void Start::on_setAppendNum_toggled(){
    if(ui->setAppendNum->isChecked()){
        opts.setOption("append", "1");
    }
    else{
        opts.setOption("append", "0");
    }
}

void Start::on_appendNum_textChanged(){
    QString num = ui->appendNum->text();
    opts.setOption("appendnum", num);
}

void Start::on_setIterations_toggled(){
    if(ui->setIterations->isChecked()){
        opts.setOption("setiter", "1");
    }
    else{
        opts.setOption("setiter", "0");
    }
}

void Start::on_iterations_textChanged(){
    int iterations = ui->iterations->text().toInt();
    QString iter = QString::number(iterations);
    opts.setOption("iterations", iter);
    ui->iterations->setText(iter);
}

void Start::on_setReplaceXML_toggled(){
    if(ui->setReplaceXML->isChecked()){
        opts.setOption("replacexml", "1");
    }
    else{
        opts.setOption("replacexml", "0");
    }
}

void Start::on_setAddCLRF_toggled(){
    if(ui->setAddCLRF->isChecked()){
        opts.setOption("addclrf", "1");
    }
    else{
        opts.setOption("addclrf", "0");
    }
}

void Start::on_setUseUnicode_toggled(){
    if(ui->setUseUnicode->isChecked()){
        opts.setOption("useunicode", "1");
    }
    else{
        opts.setOption("useunicode", "0");
    }
}

void Start::on_setNumThreads_toggled(){
    if(ui->setNumThreads->isChecked()){
        opts.setOption("threads", "1");
    }
    else{
        opts.setOption("threads", "0");
    }
}

void Start::on_numThreads_textChanged(){
    int threads = ui->numThreads->text().toInt();
    QString thread = QString::number(threads);
    opts.setOption("numthreads", thread);
    ui->numThreads->setText(thread);
}

void Start::on_setUseRules_toggled(){
    if(ui->setUseRules->isChecked()){
        opts.setOption("userules", "1");
    }
    else{
        opts.setOption("userules", "0");
    }
}

void Start::on_rules_textChanged(){
    QString path = ui->rules->text();
    testPath(path, ui->rules);
    opts.setOption("rules", path);
}

void Start::on_selectRules_clicked(){
    QString path = QFileDialog::getOpenFileName(this, "Select rules file");
    if(path.length() == 0){
        return;
    }
    testPath(path, ui->rules);
    ui->rules->setText(path);
}

void Start::on_setPermutationFile_toggled(){
    if(ui->setPermutationFile->isChecked()){
        opts.setOption("setperm", "1");
    }
    else{
        opts.setOption("setperm", "0");
    }
}

void Start::on_permutationFile_textChanged(){
    QString path = ui->permutationFile->text();
    testPath(path, ui->permutationFile);
    opts.setOption("permutation", path);
}

void Start::on_selectPermutationFile_clicked(){
    QString path = QFileDialog::getOpenFileName(this, "Select permutation file");
    if(path.length() == 0){
        return;
    }
    testPath(path, ui->permutationFile);
    ui->permutationFile->setText(path);
}

void Start::on_setSaveFounds_toggled(){
    if(ui->setSaveFounds->isChecked()){
        opts.setOption("savefound", "1");
    }
    else{
        opts.setOption("savefound", "0");
    }
}

void Start::on_saveFounds_textChanged(){
    QString path = ui->saveFounds->text();
    opts.setOption("savepath", path);
}

void Start::on_selectSaveFounds_clicked(){
    QString path = QFileDialog::getSaveFileName(this, "Select where the founds should be saved");
    if(path.length() == 0){
        return;
    }
    ui->saveFounds->setText(path);
}

void Start::on_mdxfindLocation_textChanged(){
    QString path = ui->mdxfindLocation->text();
    testPath(path, ui->mdxfindLocation);
    opts.setOption("mdxfind", path);
}

void Start::on_selectMdxfindLocation_clicked(){
    QString path = QFileDialog::getOpenFileName(this, "Select the mdxfind binary");
    if(path.length() == 0){
        return;
    }
    testPath(path, ui->mdxfindLocation);
    ui->mdxfindLocation->setText(path);
}












