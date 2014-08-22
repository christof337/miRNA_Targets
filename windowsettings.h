#ifndef WINDOWSETTINGS_H
#define WINDOWSETTINGS_H

#include <QTabWidget>
#include <QMainWindow>
#include <QStatusBar>
#include <QMessageBox>

#include "settings.h"

namespace Ui {
class WindowSettings;
}

class WindowSettings : public QTabWidget
{
    Q_OBJECT

public:
    explicit WindowSettings(QWidget * previous, int step, QWidget *parent = 0);
    ~WindowSettings();
private slots:
    void on_button_Rcancel_2_clicked();
    void on_button_Lcancel_clicked();
    void on_button_Ecancel_clicked();
    void on_button_Dcancel_clicked();
    void on_button_Scancel_clicked();
    void on_button_Fcancel_clicked();
    void on_button_Rcancel_clicked();
    void on_button_Rsave_2_clicked();
    void on_button_Lsave_clicked();
    void on_button_Esave_clicked();
    void on_button_Dsave_clicked();
    void on_button_Ssave_clicked();
    void on_button_Fsave_clicked();
    void on_button_Rsave_clicked();
    void on_button_Ldefault_clicked();
    void on_button_Edefault_clicked();
    void on_button_Ddefault_clicked();
    void on_button_Sdefault_clicked();
    void on_button_Fdefault_clicked();
    void on_button_Rdefault_clicked();
private:
    Ui::WindowSettings *ui;
    QWidget * previousUI;
    void initFields(int tab=0);
    void validate() const;
    void hideIt();
};

#endif // WINDOWSETTINGS_H
