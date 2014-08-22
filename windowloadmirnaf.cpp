#include "windowloadmirnaf.h"
#include "ui_windowloadmirnaf.h"

windowLoadMiRNAF::windowLoadMiRNAF(QString const& _fileName,QWidget *parent) :
    QWidget(parent),
    fileName(_fileName),
    ui(new Ui::windowLoadMiRNAF),
    m_oTF(0)
{
    ui->setupUi(this);
    ui->settingsButton->setIcon(QIcon("./images/gear.png"));
    //not visible  labelFileSelectedMirna
    ui->labelFileSelectedMirna->setVisible(false);
    //change the color of label
    ui->labelAnalyzeResults->setStyleSheet(("* { background-color:gray }"));
    //not visible button NextAnalyzeResults
    ui->NextAnalyzeResults->setEnabled(false);
    qobject_cast<QMainWindow *> (this->parentWidget())->statusBar()->showMessage(tr("II.1.Load miRNA"));
    m_oTF=new TargetFinder();


    this->ui->progressBar->setMinimum(0);
    this->ui->progressBar->setMaximum(0);
    this->ui->progressBar->hide();

    connect(&this->FutureWatcher, SIGNAL(finished()), this, SLOT(slot_finished()));
}

windowLoadMiRNAF::~windowLoadMiRNAF()
{
    delete ui;
}


void windowLoadMiRNAF::slot_finished()
{
    //Sleep(1000);
}

QString windowLoadMiRNAF::getFileName() const{
    return fileName;
}

void windowLoadMiRNAF::setFileName(QString const& _fileName){
    fileName=_fileName;

}


void windowLoadMiRNAF::on_buttonChooseMirnaF_clicked()
{
    fileNameMiRNA=QFileDialog::getOpenFileName(
                this,
                tr("Choose File"),
                "",
                "FASTA files (*.fasta);;Text files (*.txt);;All files (*)"
                );
    if(!fileNameMiRNA.isEmpty()){
        ui->progressBar->show();
        try
        {
            QFuture<bool> future = QtConcurrent::run(&QtExceptionWrapper::targetFinder_getMiRNAs,m_oTF,fileNameMiRNA.toStdString());
            //m_oTF->getMiRNAs(fileNameMiRNA.toStdString());
            this->FutureWatcher.setFuture(future);
            future.result();
            //make visible  labelFileSelectedMirna
            ui->labelFileSelectedMirna->setVisible(true);
            //change the value of labelFileSelectedMirna
            ui->labelFileSelectedMirna->setText(tr("Mirna File selected : ")+fileNameMiRNA);
            //make the button next accessible
            ui->NextAnalyzeResults->setEnabled(true);

            this->ui->progressBar->setMaximum(100);
            this->ui->progressBar->setValue(100);
        }catch(MyException const& e)
        {
            QMessageBox::warning(this,tr("Loading error"),e.what());
            //hiding labelFileSelectedMirna
            ui->labelFileSelectedMirna->setVisible(false);
            //change the value of labelFileSelectedMirna
            ui->labelFileSelectedMirna->setText(tr(""));
            //make the button next disabled
            ui->NextAnalyzeResults->setEnabled(false);
            this->ui->progressBar->hide();
        }catch(std::string const& e2)
        {
            QMessageBox::warning(this,tr("Loading error"),QString::fromStdString(e2));
            //hiding labelFileSelectedMirna
            ui->labelFileSelectedMirna->setVisible(false);
            //change the value of labelFileSelectedMirna
            ui->labelFileSelectedMirna->setText(tr(""));
            //make the button next disabled
            ui->NextAnalyzeResults->setEnabled(false);
            this->ui->progressBar->hide();
        }
    }
    else{
        QMessageBox::warning(this,tr("File selected"),tr("No file selected"));
    }

}

void windowLoadMiRNAF::on_NextAnalyzeResults_clicked()
{
    if(m_oTF->getMiRNA().size()>0 && !fileName.isEmpty() && !fileNameMiRNA.isEmpty()){
            windowShuffleData *centralWindow = new windowShuffleData(this->fileName,m_oTF,this->parentWidget());
            /*cast parent of current window in QMainWindow and change the central Area*/
            qobject_cast<QMainWindow *> (this->parentWidget())->setCentralWidget(centralWindow);
     }
}

void windowLoadMiRNAF::on_settingsButton_clicked()
{
    WindowSettings *centralWindow = new WindowSettings(this,1,this->parentWidget());
    centralWindow->show();
}
