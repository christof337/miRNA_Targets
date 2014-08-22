#ifndef WINDOWLOAD_H
#define WINDOWLOAD_H

#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QMainWindow>
#include <QStatusBar>

#include "windowextractdata.h"
#include "windowloadmirnaf.h"
#include "utils.h"
#include "windowsettings.h"

namespace Ui {
class WindowLoad;
}

class WindowLoad : public QWidget
{
    Q_OBJECT

public:
    explicit WindowLoad(QWidget *parent = 0);
    ~WindowLoad();
    QString getFileSelectedName() const;
    void setFileSelectedName(QString const& nameFile);
    QString getFileDownloadedName() const;
    void setFileDownloadedName(QString const& nameFile);

private slots:
    void on_buttonChoose_clicked();

    void on_next_clicked();

    void on_buttonGoToAnalyzeRes_clicked();

    void on_buttonLoadDirectlyFile_clicked();

    void on_settingsButton_clicked();

private:
    QString fileSelectedName;
    QString fileDownloadedName;
    Ui::WindowLoad *ui;
};

#endif // WINDOWLOAD_H
