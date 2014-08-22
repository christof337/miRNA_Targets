#include "windowdownloadd.h"
#include "ui_windowdownloadd.h"

using namespace std;

windowDownloadD::windowDownloadD(std::vector<std::string> _id, QString const& _fileName, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::windowDownloadD),
    id(_id),
    fileName(_fileName)
{
    ui->setupUi(this);
    ui->settingsButton->setIcon(QIcon("./images/gear.png"));
    //complete the list of id
    for (unsigned int i = 0; i < id.size(); i++)
    {
        ui->listWidgetId->addItem(QString::fromStdString(id[i]));
    }
    qobject_cast<QMainWindow *> (this->parentWidget())->statusBar()->showMessage(tr("I.3.Download"));

    this->ui->progressBar->setMinimum(0);
    this->ui->progressBar->setMaximum(0);
    this->ui->progressBar->hide();

    connect(&this->FutureWatcher, SIGNAL(finished()), this, SLOT(slot_finished()));
}

windowDownloadD::~windowDownloadD()
{
    delete ui;
}

void windowDownloadD::slot_finished()
{
    ui->NextDownloadD->setEnabled(true);
    vector<vector<string> > result;
    string res1("The following ensembles could not have been downloaded because out of memory.\n");
    string res2("The following ensembles could not have been downloaded because not on the server.\n");

    try
    {
        result=m_fFuture.result();
        if(!result.empty())
        {
            if(!result[0].empty())
            {
                for(unsigned int i=0;i<result[0].size();i++)
                {
                    res1+=result[0][i]+"\n";
                }
                QMessageBox::warning(this,tr("OUT OF MEMORY"),QString::fromStdString(res1));
            }
            if(!result[1].empty())
            {
                for(unsigned int i=0;i<result[1].size();i++)
                {
                    res2+=result[1][i]+"\n";
                }
                QMessageBox::warning(this,tr("DOWNLOAD FAILURE"),QString::fromStdString(res2));
            }
        }
        this->ui->progressBar->setMaximum(100);
        this->ui->progressBar->setValue(100);
        Sleep(1000);
        windowEndDownloadd *centralWindow = new windowEndDownloadd(QString::fromStdString(PathHandler::getDownloadedFileName(fileName.toStdString())),this->parentWidget());
        /*cast parent of current window in QMainWindow and change the central Area*/
        qobject_cast<QMainWindow *> (this->parentWidget())->setCentralWidget(centralWindow);
    }catch(MyException const& e)
    {
        QMessageBox::warning(this,tr("Exception"),e.what());
    }
}

void windowDownloadD::on_NextDownloadD_clicked()
{
    //warning the user that the treatment can be long
    QMessageBox::warning(this,tr("Long treatment"),"Please wait during the processing. \n(Note : the whole downloading can be about 10 minutes maximum");

    ui->NextDownloadD->setEnabled(false);
    ui->progressBar->show();

    m_fFuture=QtConcurrent::run(&QtExceptionWrapper::download_downloadEnsembles,id,getFileName().toStdString());
    this->FutureWatcher.setFuture(m_fFuture);
               //((mainwindow)this->parentWidget())->changeCentralArea(centralWindow,"I.4.End Download");
}

QString windowDownloadD::getFileName() const
{
    return fileName;
}

void windowDownloadD::on_settingsButton_clicked()
{
    WindowSettings *centralWindow = new WindowSettings(this,3,this->parentWidget());
    centralWindow->show();
}
