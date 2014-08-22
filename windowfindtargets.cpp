#include "windowfindtargets.h"
#include "ui_windowfindtargets.h"

using namespace std;

windowFindTargets::windowFindTargets(QString const& p_sFileName, TargetFinder * p_oTF, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::windowFindTargets),
    m_sFileName(p_sFileName),
    m_oTF(p_oTF),
    m_oTFShuffled(0),
    m_bShuffling(false),
    m_bFlagProcessOver(false),
    m_iNbSequences(m_oTF->getNbSequences(p_sFileName.toStdString())),
    mutex()
{
    //this->ProgressDialog = new QProgressDialog();
    ui->setupUi(this);
    ui->settingsButton->setIcon(QIcon("./images/gear.png"));
    qobject_cast<QMainWindow *> (this->parentWidget())->statusBar()->showMessage(tr("II.3.Find Targets"));

    ui->label_origin->hide();
    ui->label_shuffle->hide();

    this->ui->button_next->setEnabled(false);

    this->ui->progressBar->setMinimum(0);
    this->ui->progressBar->setMaximum(m_iNbSequences);
    this->ui->progressBar->hide();

    this->ui->progressBar_shuffle->setMinimum(0);
    this->ui->progressBar_shuffle->setMaximum(m_iNbSequences);
    this->ui->progressBar_shuffle->hide();

    this->ui->label_advancing->setText("0/"+QString::fromStdString(utils::convertToString<int>(m_iNbSequences)));
    this->ui->label_advancing->hide();
    this->ui->label_advancing_shuffle->setText("0/"+QString::fromStdString(utils::convertToString<int>(m_iNbSequences)));
    this->ui->label_advancing_shuffle->hide();

    m_oTFShuffled = new TargetFinder(*m_oTF);
    connect(&this->FutureWatcher, SIGNAL(finished()), this, SLOT(slot_finished()));
    connect(&this->FutureWatcher2, SIGNAL(finished()), this, SLOT(slot_finished()));
    //connect(&this->FutureWatcher,SIGNAL(finished()),this->ProgressDialog,SLOT(cancel()));
    //connecting the advancing long function to the progress bar
    connect(m_oTF, SIGNAL(signalProgress(int)), this, SLOT(changeProgressValue(int)));
    connect(m_oTFShuffled, SIGNAL(signalProgress(int)), this, SLOT(changeProgressValueShuffle(int)));
}

void windowFindTargets::changeProgressValue(int progress)
{
    ui->label_advancing->setText(QString::fromStdString(utils::convertToString<int>(progress))+"/"+QString::fromStdString(utils::convertToString<int>(m_iNbSequences)));
    ui->progressBar->setValue(progress);
}

void windowFindTargets::changeProgressValueShuffle(int progress)
{
    ui->label_advancing_shuffle->setText(QString::fromStdString(utils::convertToString<int>(progress))+"/"+QString::fromStdString(utils::convertToString<int>(m_iNbSequences)));
    ui->progressBar_shuffle->setValue(progress);
}

void windowFindTargets::slot_finished()
{
    mutex.lock();
    //target finding over
    if(m_bFlagProcessOver||!m_bShuffling)
    {
        //if it's the second or if we don't shuffle, we enable next button
        ui->button_next->setEnabled(true);
    }else
    {
        //if it's the first, we do nothing (waiting for the second)
    }
    m_bFlagProcessOver=true;
    mutex.unlock();
}

windowFindTargets::~windowFindTargets()
{
    delete ui;
    if(FutureWatcher.isRunning())
        FutureWatcher.future().cancel();
    if(FutureWatcher2.isRunning())
        FutureWatcher2.future().cancel();
}

void windowFindTargets::on_button_start_finding_clicked()
{
    ui->button_start_finding->setEnabled(false);
    m_bFlagProcessOver=false;
    ui->button_next->setEnabled(false);
    ui->progressBar->show();
    ui->label_advancing->show();
    //running the target finding in the original file
    QFuture<void> future1 = QtConcurrent::run(this->m_oTF, &TargetFinder::loadSequenceFile, m_sFileName.toStdString());
    this->FutureWatcher.setFuture(future1);

    //running the target finding in the shuffled file if exists
    if(utils::isFileExisting(PathHandler::getShuffledFileName(m_sFileName.toStdString())))
    {
        ui->label_origin->show();
        ui->label_shuffle->show();
        ui->progressBar_shuffle->show();
        ui->label_advancing_shuffle->show();
        QFuture<void> future2 = QtConcurrent::run(this->m_oTFShuffled, &TargetFinder::loadSequenceFile,PathHandler::getShuffledFileName(m_sFileName.toStdString()));
        this->FutureWatcher2.setFuture(future2);
        m_bShuffling=true;
    }else
    {
        m_bShuffling=false;
    }
}

void windowFindTargets::on_button_next_clicked()
{
    //going to the next page?
    //when over, go to the result window
    WindowResults *centralWindow;
    if(m_bShuffling)
    {
        centralWindow = new WindowResults(*m_oTF,m_sFileName,this->parentWidget(),*m_oTFShuffled);
    }else
    {
        centralWindow = new WindowResults(*m_oTF,m_sFileName,this->parentWidget());
    }
    /*cast parent of current window in QMainWindow and change the central Area*/
    qobject_cast<QMainWindow *> (this->parentWidget())->setCentralWidget(centralWindow);
}

void windowFindTargets::on_settingsButton_clicked()
{
    WindowSettings *centralWindow = new WindowSettings(this,5,this->parentWidget());
    centralWindow->show();
}
