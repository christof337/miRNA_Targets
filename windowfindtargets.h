#ifndef WINDOWFINDTARGETS_H
#define WINDOWFINDTARGETS_H

#include <QWidget>
#include <QMainWindow>
#include <QProgressBar>
//#include <QProgressDialog>
#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrentRun>
#include <Windows.h>
#include <fstream>

#include "windowresults.h"
#include "targetFinder.h"
#include "pathhandler.h"
#include "windowsettings.h"

namespace Ui {
class windowFindTargets;
}

class windowFindTargets : public QWidget
{
    Q_OBJECT
public slots:
    void slot_finished();
    void changeProgressValue(int progress);
    void changeProgressValueShuffle(int progress);
public:
    explicit windowFindTargets(QString const& p_sFileName, TargetFinder * p_oTF, QWidget *parent = 0);
    ~windowFindTargets();

private slots:
    void on_button_start_finding_clicked();

    void on_button_next_clicked();

    void on_settingsButton_clicked();

private:
    Ui::windowFindTargets *ui;
    QString m_sFileName;
    TargetFinder * m_oTF;
    TargetFinder * m_oTFShuffled;
    QFutureWatcher<void> FutureWatcher;
    QFutureWatcher<void> FutureWatcher2;
    int m_iNbSequences;
    bool m_bShuffling;
    bool m_bFlagProcessOver;
    QMutex mutex;
   // QProgressDialog* ProgressDialog;
};

#endif // WINDOWFINDTARGETS_H
