
#include "windowload.h"
#include "ui_windowload.h"

using namespace std;

//functions

WindowLoad::WindowLoad(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WindowLoad)
{
    ui->setupUi(this);

    ui->settingsButton->setIcon(QIcon("./images/gear.png"));

    //not visible  labelFileSelected
    ui->labelFileSelected->setVisible(false);
    //not accessible button next
    ui->next->setEnabled(false);
    //change the color of label
    ui->labelFindData->setStyleSheet(("* { background-color:black }"));
    //not visible button buttonGoToAnalyzeRes
    ui->buttonGoToAnalyzeRes->setVisible(false);
    //change the color of button
   // ui->buttonGoToAnalyzeRes->setStyleSheet(("* { background-color:gray }"));
    qobject_cast<QMainWindow *> (this->parentWidget())->statusBar()->showMessage(tr("I.1.Load"));
}

WindowLoad::~WindowLoad()
{
    delete ui;
}

QString WindowLoad::getFileSelectedName() const{
    return fileSelectedName;
}

void WindowLoad::setFileSelectedName(QString const& nameFile){
    fileSelectedName=nameFile;

}

QString WindowLoad::getFileDownloadedName() const{
    return fileDownloadedName;
}

void WindowLoad::setFileDownloadedName(QString const& nameFile){
    fileDownloadedName=nameFile;
}

void WindowLoad::on_buttonChoose_clicked()
{
    string name;
    string path;
    QString fileName=QFileDialog::getOpenFileName(
                this,
                tr("Choose File"),
                "",
                "CSV (*.csv)"
                );
    //not visible button buttonGoToAnalyzeRes
    ui->buttonGoToAnalyzeRes->setVisible(false);
    if(!fileName.isEmpty()){
        ui->next->setEnabled(true);
        setFileSelectedName(fileName);
        name=PathHandler::getDownloadedFileName(utils::getName(fileName.toStdString()));
       // name=name+(DEFAULT_DOWNLOADED_SUFFIX);
        path=name;
        if (utils::isFileExisting(path)){
            // à implementer
            QMessageBox::information(this,tr("File selected"),"This file has been downloaded before !");
            //the path of the file downloaded
            setFileDownloadedName(QString::fromStdString(path));
            //make visible the button buttonGoToAnalyzeRes
            ui->buttonGoToAnalyzeRes->setVisible(true);
            ui->next->setText("Download again");
        }
        else{
            ui->next->setText("Download");
        }
        //make visible  labelFileSelected
        ui->labelFileSelected->setVisible(true);
        //change the value of labelFileSelected
        ui->labelFileSelected->setText("File selected : "+this->getFileSelectedName());
        //make the button next accessible
        ui->next->setEnabled(true);
     }
    else{
        QMessageBox::warning(this,tr("File selected"),"No file selected");
    }
}

void WindowLoad::on_next_clicked()
{
    WindowExtractData *centralWindow = new WindowExtractData(this->getFileSelectedName(),this->parentWidget());
   /*cast parent of current window in QMainWindow and change the central Area*/
    qobject_cast<QMainWindow *> (this->parentWidget())->setCentralWidget(centralWindow);
}

void WindowLoad::on_buttonGoToAnalyzeRes_clicked()
{
    windowLoadMiRNAF *centralWindow = new windowLoadMiRNAF(this->getFileDownloadedName(),this->parentWidget());
    /*cast parent of current window in QMainWindow and change the central Area*/
     qobject_cast<QMainWindow *> (this->parentWidget())->setCentralWidget(centralWindow);
}

void WindowLoad::on_buttonLoadDirectlyFile_clicked()
{

    QString fileName=QFileDialog::getOpenFileName(
                this,
                tr("Choose File"),
                "",
                "FASTA files (*.fasta);;Text files (*.txt);;All files (*)"
                );
        if(!fileName.isEmpty()){
            setFileDownloadedName(fileName);
                //make visible  labelFileSelected
                ui->labelFileSelected->setVisible(true);
                //change the value of labelFileSelected
                ui->labelFileSelected->setText("File selected : "+this->getFileDownloadedName());
                //make visible the button buttonGoToAnalyzeRes
                ui->buttonGoToAnalyzeRes->setVisible(true);
                ui->next->setEnabled(false);
                ui->next->setText("Download");
         }
        else{
            QMessageBox::warning(this,tr("File selected"),"No file selected");
        }

}

void WindowLoad::on_settingsButton_clicked()
{
    WindowSettings *centralWindow = new WindowSettings(this,1,this->parentWidget());
    centralWindow->show();
}
