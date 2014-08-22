#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    /*Change Title*/
    setWindowTitle("MIRNA TARGET");
    if (this->centralWidget())
        this->centralWidget()->setParent(0); // Preserve current central widget
    /*Set QWidget as the central layout of the main window*/
    WindowLoad *centralWindow = new WindowLoad(this);
    this->changeCentralArea(centralWindow);

    statusBar()->showMessage(tr("I.1.Load"));
   /* QTreeWidgetItem qtwi(this->ui->arianeString);
    qtwi.setText(0,"Get data");
    QTreeWidgetItem qtwi2(qtwi);
    qtwi2.setText(0,"Load");
    qtwi2.setText(1,"Extract");
    qtwi2.setText(2,"Download");
    qtwi.setText(1,"Analyze data");
    QTreeWidgetItem qtwi3(qtwi);
    qtwi3.setText(0,"Load miRNA");
    qtwi3.setText(1,"Shuffle");
    qtwi3.setText(2,"Find targets");
    qtwi3.setText(3,"Results");

    this->ui->arianeString->insertTopLevelItem(0,&qtwi);*/
}

void MainWindow::changeCentralArea(QWidget* centralWindow, QString const& p_sMessage){
    this->setCentralWidget(centralWindow);
    statusBar()->showMessage(p_sMessage);
}

MainWindow::~MainWindow()
{
    delete ui;
}
