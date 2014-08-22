#ifndef WINDOWEXTRACTDATA_H
#define WINDOWEXTRACTDATA_H

#include <QWidget>
#include <QMessageBox>
#include <QStatusBar>
#include <QMainWindow>
#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrentRun>
#include <QList>
#include <QString>

#include "load.h"
#include "windowdownloadd.h"
#include "qtexceptionwrapper.h"
#include "windowsettings.h"
#include "windowload.h"

namespace Ui {
class WindowExtractData;
}

class WindowExtractData : public QWidget
{
    Q_OBJECT
public slots:
  void slot_finished();
public:
    explicit WindowExtractData(QString _fileName,QWidget *parent = 0);
    ~WindowExtractData();
    QString getFileName();
    void setFileName(QString _fileName);

private slots:
    void on_NextExtract_clicked();
    void on_buttonExtract_clicked();

    void on_settingsButton_clicked();

private:
    QString fileName;
    std::vector<std::string> id;
    Ui::WindowExtractData *ui;
    QFutureWatcher<std::vector<std::string>> FutureWatcher;

};

#endif // WINDOWEXTRACTDATA_H
