#include "windowshuffledata.h"
#include "ui_windowshuffledata.h"

windowShuffleData::windowShuffleData(QString const& _fileName,TargetFinder * p_oTF, QWidget *parent) :
    QWidget(parent),
    fileName(_fileName),
    m_oTF(p_oTF),
    ui(new Ui::windowShuffleData),
    m_bHasShuffled(false),
    m_bIsShuffling(false)
{
    ui->setupUi(this);
    ui->settingsButton->setIcon(QIcon("./images/gear.png"));
    //ProgressBar
    this->ui->progressBar->setMinimum(0);
    this->ui->progressBar->setMaximum(0);
    this->ui->progressBar->hide();
    connect(&this->FutureWatcher, SIGNAL(finished()), this, SLOT(slot_finished()));
    qobject_cast<QMainWindow *> (this->parentWidget())->statusBar()->showMessage(tr("II.2.Shuffle data"));
}

QString windowShuffleData::getFileName(){
    return fileName;
}

void windowShuffleData::setFileName(QString _fileName){
    fileName=_fileName;
}

windowShuffleData::~windowShuffleData()
{
    delete ui;
}

void windowShuffleData::on_ButtonShuffling_clicked()
{
    m_bIsShuffling=true;
    m_bHasShuffled=false;
    ui->ButtonShuffling->setEnabled(false);
    //ProgressBar
    ui->progressBar->show();
   QFuture<void> future = QtConcurrent::run(Shuffle::mainShuffling,this->getFileName().toStdString());

   this->FutureWatcher.setFuture(future);
}

void windowShuffleData::on_nextShuffleData_clicked()
{
    if(!m_bHasShuffled&&!utils::isFileExisting(PathHandler::getShuffledFileName(fileName.toStdString()))||m_bIsShuffling)
    {
        if(QMessageBox::Yes == QMessageBox::question(this,tr("Hasn't shuffled yet."),"The shuffling isn't over. If you go to the next page now, you will be unable to get informations concierning the Correlation Coefficient (only the targets). Do you still want to leave this window?"))
        {
            //displaying the next window
            windowFindTargets *centralWindow = new windowFindTargets(this->fileName,m_oTF,this->parentWidget());
            /*cast parent of current window in QMainWindow and change the central Area*/
            qobject_cast<QMainWindow *> (this->parentWidget())->setCentralWidget(centralWindow);
        }
    }else
    {
        //displaying the next window
        windowFindTargets *centralWindow = new windowFindTargets(this->fileName,m_oTF,this->parentWidget());
        /*cast parent of current window in QMainWindow and change the central Area*/
        qobject_cast<QMainWindow *> (this->parentWidget())->setCentralWidget(centralWindow);
    }
}

void windowShuffleData::slot_finished()
{
    m_bIsShuffling=false;
    this->ui->progressBar->setMaximum(100);
    this->ui->progressBar->setValue(100);
   // this->ui->progressBar->hide();
    //going to the next page?
    ui->ButtonShuffling->setEnabled(true);
}

void windowShuffleData::on_settingsButton_clicked()
{
    WindowSettings *centralWindow = new WindowSettings(this,4,this->parentWidget());
    centralWindow->show();
}
