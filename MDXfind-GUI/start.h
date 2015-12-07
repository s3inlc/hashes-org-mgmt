/*
 * Created 2014 by Sein Coray
 *
 * https://github.com/s3inlc/hashes-org-mgmt/tree/master/MDXfind-GUI
 */

#ifndef START_H
#define START_H

#include <QMainWindow>
#include <QFileInfo>
#include <QFileDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <QMessageBox>
#include <QListWidget>
#include <QStatusBar>
#include <QTimer>
#include <QObject>
#include <QProcess>
#include <QPushButton>
#include <QTextEdit>

#include <cstdio>
#include <cstdlib>
#include <unistd.h>

#include "options.h"

namespace Ui {
class Start;
}

class Start : public QMainWindow
{
    Q_OBJECT

public:
    explicit Start(QWidget *parent = 0);
    ~Start();

public slots:
    void on_hashFile_textChanged();
    void on_selectHashFile_clicked();
    void on_hashTypes_textChanged();
    void on_preHashTypes_currentIndexChanged(int i);
    void on_setSaltFile_toggled();
    void on_saltFile_textChanged();
    void on_selectSaltFile_clicked();
    void on_setUserIDFile_toggled();
    void on_userIDFile_textChanged();
    void on_selectUserIDFile_clicked();
    void on_setRotation_toggled();
    void on_rotation_textChanged();
    void on_setAppendNum_toggled();
    void on_appendNum_textChanged();
    void on_setIterations_toggled();
    void on_iterations_textChanged();
    void on_setReplaceXML_toggled();
    void on_setAddCLRF_toggled();
    void on_setUseUnicode_toggled();
    void on_setNumThreads_toggled();
    void on_numThreads_textChanged();
    void on_setUseRules_toggled();
    void on_rules_textChanged();
    void on_selectRules_clicked();
    void on_setPermutationFile_toggled();
    void on_permutationFile_textChanged();
    void on_selectPermutationFile_clicked();
    void on_setSaveFounds_toggled();
    void on_saveFounds_textChanged();
    void on_selectSaveFounds_clicked();
    void on_mdxfindLocation_textChanged();
    void on_selectMdxfindLocation_clicked();
    void on_setClean_toggled();
    void on_selectClean_clicked();
    void on_setCleanPath_textChanged();

    void on_startProcess_clicked();
    void prgUpdate();
    void prgFinished(int code);
    void on_wordlistsAdd_clicked();
    void on_wordlistsRemove_clicked();
    void on_wordlistsLeft_clicked();
    void on_wordlistsRight_clicked();

private:
    Ui::Start *ui;
    Options opts;
    bool block;

    void testPath(QString, QLineEdit*);
    bool validPath(QString path);
    void errMsg(QString title, QString msg);
    void updateWordlists();

    QProcess *prg;
    QTimer prgTimer;
    bool running;
    bool inprg;
};

#endif // START_H
