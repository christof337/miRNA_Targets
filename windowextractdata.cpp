#include "windowextractdata.h"
#include "ui_windowextractdata.h"

using namespace std;

WindowExtractData::WindowExtractData(QString _fileName,QWidget *parent) :
    QWidget(parent),
    fileName(_fileName),
    ui(new Ui::WindowExtractData)
{
    ui->setupUi(this);
    ui->settingsButton->setIcon(QIcon("./images/gear.png"));
    //not accessible button next
    ui->NextExtract->setEnabled(false);
    //the user can not change the state of checkBox
    ui->checkBoxSuccesExtract->setDisabled(true);
    ui->checkBoxSuccesExtract->setCheckState(Qt::Unchecked);
    qobject_cast<QMainWindow *> (this->parentWidget())->statusBar()->showMessage(tr("I.2.Extract"));

    this->ui->progressBar->setMinimum(0);
    this->ui->progressBar->setMaximum(0);
    this->ui->progressBar->hide();

    connect(&this->FutureWatcher, SIGNAL(finished()), this, SLOT(slot_finished()));
}


void WindowExtractData::slot_finished()
{
    this->ui->progressBar->setMaximum(100);
    this->ui->progressBar->setValue(100);
    //Sleep(1000);
   // this->ui->progressBar->hide();
}

QString WindowExtractData::getFileName(){
    return fileName;
}

void WindowExtractData::setFileName(QString _fileName){
    fileName=_fileName;

}

WindowExtractData::~WindowExtractData()
{
    delete ui;
}

void WindowExtractData::on_NextExtract_clicked()
{
    if(!id.empty()){
        windowDownloadD *centralWindow = new windowDownloadD(this->id,this->getFileName(),this->parentWidget());
        /*cast parent of current window in QMainWindow and change the central Area*/
        qobject_cast<QMainWindow *> (this->parentWidget())->setCentralWidget(centralWindow);
     }
}



void WindowExtractData::on_buttonExtract_clicked()
{
    ui->buttonExtract->setEnabled(false);
    ui->progressBar->show();
    id.clear();
    /*save the id of ensembles*/
        try
     {
        // Start the computation.
        //extern QList<QString> persoLoad(QString);
       // extern vector<string> Load::loadAFile(const string & path, bool shouldWriteInAFile, unsigned int column);
        QFuture<vector<string>> future = QtConcurrent::run(&QtExceptionWrapper::load_loadAFile,this->getFileName().toStdString());
        this->FutureWatcher.setFuture(future);
        id=future.result();
        //vector<string> temp=future.result();
        //for(int i=0;i<temp.size();i++)
        //{
        //    id.push_back(temp[i].toStdString());
        //}
        //id=future.results().toVector();
        // id=Load::loadAFile(this->getFileName().toStdString(),false);
         //Finish loading
         //ui->progressBar->setValue(100);
         //check the chekBox
         ui->checkBoxSuccesExtract->setCheckState(Qt::Checked);
         //display the list of id
         for (unsigned int i = 0; i < id.size(); i++)
         {
             ui->listWidgetId->addItem(QString::fromStdString(id[i]));
         }
         //make the button next accessible
         ui->NextExtract->setEnabled(true);
       }
        catch(MyException const& e)
        {
             QMessageBox::warning(this,tr("Error"),e.what());
             WindowLoad *centralWindow = new WindowLoad(this->parentWidget());
             qobject_cast<QMainWindow *> (this->parentWidget())->setCentralWidget(centralWindow);
        }
}

void WindowExtractData::on_settingsButton_clicked()
{
    WindowSettings *centralWindow = new WindowSettings(this,2,this->parentWidget());
    centralWindow->show();
}
