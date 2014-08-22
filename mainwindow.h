#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

#include "WindowLoad.h"


/*! \class MainWindow
  * \brief Class of the UI handling the positionning of each "real" window.
  *
  * This class enable the user to see different displays in the same window.
  *  Note : not very visible since there isn't any bar menu.
  */
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void changeCentralArea(QWidget* centralWindow, QString const& p_sMessage="");

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
