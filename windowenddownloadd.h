#ifndef WINDOWENDDOWNLOADD_H
#define WINDOWENDDOWNLOADD_H

#include <QWidget>
#include "windowsettings.h"
#include "windowloadmirnaf.h"
#include <QMainWindow>
#include <QStatusBar>

namespace Ui {
class windowEndDownloadd;
}

class windowEndDownloadd : public QWidget
{
    Q_OBJECT

public:
    explicit windowEndDownloadd(QString p_sFileName, QWidget *parent = 0);
    ~windowEndDownloadd();

private slots:
    void on_pushButton_clicked();

    void on_settingsButton_clicked();

private:
    Ui::windowEndDownloadd *ui;
    QString m_sFileName;
};

#endif // WINDOWENDDOWNLOADD_H
