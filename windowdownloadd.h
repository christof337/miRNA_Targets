#ifndef WINDOWDOWNLOADD_H
#define WINDOWDOWNLOADD_H

#include <QWidget>
#include <QMessageBox>
#include <QMainWindow>
#include <QStatusBar>
#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrentRun>
#include <vector>
#include <string>

#include "windowsettings.h"
//#include "qtexceptionwrapper.h"
#include "download.h"
#include "windowenddownloadd.h"
#include "pathhandler.h"

namespace Ui {
class windowDownloadD;
}

class windowDownloadD : public QWidget
{
    Q_OBJECT

public slots:
  void slot_finished();
public:
    explicit windowDownloadD(std::vector<std::string> id, QString const& _filename, QWidget *parent = 0);
    ~windowDownloadD();
    QString getFileName() const;
   // static std::vector<std::vector<std::string>> persoLoad(QString _fileName);

private slots:
    void on_NextDownloadD_clicked();
   // std::vector<std::vector<std::string>> toStd(QList<QList<QString>> p_vSource);

    void on_settingsButton_clicked();

private:
    std::vector<std::string> id;
    QString fileName;
    Ui::windowDownloadD *ui;
    QFutureWatcher<std::vector<std::vector<std::string>>> FutureWatcher;
    QFuture<std::vector<std::vector<std::string>>> m_fFuture;
};

#endif // WINDOWDOWNLOADD_H
