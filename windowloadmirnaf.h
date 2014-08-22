#ifndef WINDOWLOADMIRNAF_H
#define WINDOWLOADMIRNAF_H

#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QMainWindow>
#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrentRun>
#include <QtConcurrent/qtconcurrentexception.h>
#include <exception>

#include "qtexceptionwrapper.h"
#include "windowshuffledata.h"
#include "targetFinder.h"
#include "windowsettings.h"

namespace Ui {
class windowLoadMiRNAF;
}

class windowLoadMiRNAF : public QWidget
{
    Q_OBJECT

public slots:
  void slot_finished();
public:
    explicit windowLoadMiRNAF(QString const& _fileName,QWidget *parent = 0);
    ~windowLoadMiRNAF();
    QString getFileName() const;
    void setFileName(QString const& _fileName);

private slots:
    void on_buttonChooseMirnaF_clicked();

    void on_NextAnalyzeResults_clicked();

    void on_settingsButton_clicked();

private:
    QString fileName;
    QString fileNameMiRNA;
    Ui::windowLoadMiRNAF *ui;
    TargetFinder * m_oTF;
    QFutureWatcher<bool> FutureWatcher;
};

#endif // WINDOWLOADMIRNAF_H
