#include "windowenddownloadd.h"
#include "ui_windowenddownloadd.h"

windowEndDownloadd::windowEndDownloadd(QString p_sFileName, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::windowEndDownloadd),
    m_sFileName(p_sFileName)
{
    ui->setupUi(this);
    ui->settingsButton->setIcon(QIcon("./images/gear.png"));
    qobject_cast<QMainWindow *> (this->parentWidget())->statusBar()->showMessage(tr("I.4.End Download"));
}

windowEndDownloadd::~windowEndDownloadd()
{
    delete ui;
}

void windowEndDownloadd::on_pushButton_clicked()
{
    windowLoadMiRNAF *centralWindow = new windowLoadMiRNAF(m_sFileName,this->parentWidget());
    /*cast parent of current window in QMainWindow and change the central Area*/
    qobject_cast<QMainWindow *> (this->parentWidget())->setCentralWidget(centralWindow);
}

void windowEndDownloadd::on_settingsButton_clicked()
{
    WindowSettings *centralWindow = new WindowSettings(this,3,this->parentWidget());
    centralWindow->show();
}
