#ifndef WINDOWSHUFFLEDATA_H
#define WINDOWSHUFFLEDATA_H

#include <QWidget>
#include <QStatusBar>
#include <QMainWindow>
#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrentRun>

#include "shuffle.h"
#include "windowfindtargets.h"
#include "targetFinder.h"
#include "windowsettings.h"

namespace Ui {
class windowShuffleData;
}

class windowShuffleData : public QWidget
{
    Q_OBJECT
public slots:
    void slot_finished();
public:
    explicit windowShuffleData(QString const& _fileName,TargetFinder * p_oTF,QWidget *parent = 0);
    ~windowShuffleData();
    QString getFileName();
    void setFileName(QString _fileName);

private slots:
    void on_ButtonShuffling_clicked();

    void on_nextShuffleData_clicked();

    void on_settingsButton_clicked();

private:
    QString fileName;
    TargetFinder * m_oTF;
    QFutureWatcher<void> FutureWatcher;
    Ui::windowShuffleData *ui;
    bool m_bHasShuffled;
    bool m_bIsShuffling;
};

#endif // WINDOWSHUFFLEDATA_H
