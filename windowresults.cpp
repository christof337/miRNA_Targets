#include "windowresults.h"

using namespace std;

const string WindowResults::CONST_EXPORT_SUFFIX = "_export";
const string WindowResults::CONST_EXPORT_EXTENSION = "csv";
const string WindowResults::CONST_DEFAULT_NO_CC_STRING = "-";

bool myListItem ::operator< ( const QTableWidgetItem &other ) const
 {
     if( this->text().toDouble() < other.text().toDouble() )
         return true;
     else
         return false;
 }

/*myListItem * myListItem::myClone()
{
    myListItem * temp = new myListItem(*this);
    return temp;
}*/

int widgetItemCheckBox::getIndiceVector() const{
    return this->indiceVector;
}

void widgetItemCheckBox::setIndiceVector(int _indice){
    this->indiceVector=_indice;
}

WindowResults::WindowResults(QWidget *parent) :
QWidget(parent),
ui(new Ui::WindowResults)
{}

WindowResults::WindowResults(TargetFinder & p_vTargets, QString const& p_sFileName,QWidget * parent, TargetFinder const& p_vTargetsShuffle):
    QWidget(parent),
    ui(new Ui::WindowResults),
    m_vTargets(p_vTargets.getTargets()),
    m_vTargetsShuffle(p_vTargetsShuffle.getTargets()),
    m_iNbSequences(p_vTargets.getNbSequences(p_sFileName.toStdString())),
    m_sFileName(p_sFileName),
    m_bIsCC(p_vTargetsShuffle!=TargetFinder())
{
    ui->setupUi(this);

    ui->settingsButton->setIcon(QIcon("./images/gear.png"));
    ui->Chb_shouldDisplayResult->setToolTip("YES may slow the display");

    /*second tab*/
    //to fit the size of the window
    ui->tableWidgetResults->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidgetResults->setColumnCount(6);
    for(int i=0;i<ui->tableWidgetResults->columnCount();i++){
         ui->tableWidgetResults->setColumnWidth(i,100);
    }
    //labels of the second tab
    QStringList tableResutlsLab;
    tableResutlsLab<<"Sequence name"<<"miRNA name"<<"Position Start"<<"Position End"<<"Matches in first "+ QString::fromStdString(utils::convertToString<unsigned long>(Settings::getSettings()->getResults_nbFirsts()))<<"Score";
    ui->tableWidgetResults->setHorizontalHeaderLabels(tableResutlsLab);
    ui->tableWidgetResults->setItem(ui->tableWidgetResults->rowCount()-1,0,new QTableWidgetItem("f"));
    //QSlider
    ui->horizontalSlider->setRange(Settings::getSettings()->getResults_minimumCutoff()*100,Settings::getSettings()->getResults_maximumCutoff()*100);
    ui->horizontalSlider->setValue(Settings::getSettings()->getResults_minimumCutoff()*100);

    ui->horizontalSlider->setValue(Settings::getSettings()->getResults_maximumCutoff()*100); //--------------------------
    ui->horizontalSlider->setPageStep(Settings::getSettings()->getOOO_CONST_LARG_STEP_CUTOFF()*100);
    ui->horizontalSlider->setSingleStep(Settings::getSettings()->getResults_stepCutoff()*100);

    ui->tableWidgetResults->setSortingEnabled(true);
    ui->tableWidgetResults->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidgetTotalResults->setSelectionMode(QAbstractItemView::NoSelection);

    //diplay the value's slider on the screen
    ui->lcdNumber->setSegmentStyle(QLCDNumber::Filled);
    //change the font of the lcdNumer
    ui->lcdNumber->setPalette(Qt::darkGray);
    d(ui->horizontalSlider->value());
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)),this,SLOT(d(int)));
    qobject_cast<QMainWindow *> (this->parentWidget())->statusBar()->showMessage(tr("II.4.Results"));
   // for(unsigned int i=0;i<10;i++)
   // {
   //     WindowResults::insertRow(i);
   // }

    //QFutureWatcher
    this->ProgressDialog = new QProgressDialog();

    connect(&this->FutureWatcher, SIGNAL(finished()), this, SLOT(export_finished()));
    connect(&this->FutureWatcher, SIGNAL(finished()), this->ProgressDialog , SLOT(cancel()));


    if(m_bIsCC)
    {
        ui->tableWidgetTotalResults->horizontalHeader()->setStretchLastSection(true);
        //a shuffle has been made
        /*first tab*/
        //to fit the size of the window
        ui->tableWidgetTotalResults->setColumnCount(7);
        for(int i=0;i<ui->tableWidgetTotalResults->columnCount();i++){
             ui->tableWidgetTotalResults->setColumnWidth(i,150);

        }
        for(int i=2;i<6;i++){
             ui->tableWidgetTotalResults->setColumnWidth(i,50);
        }
        ui->tableWidgetTotalResults->setColumnWidth(6,50);
        //ui->tableWidgetResults->setColu
        //labels of the first tab
        QStringList tableTotalResutlsLab;
        tableTotalResutlsLab<<"Number of sequences"<<"Number of found targets"<<"TP"<<"FP"<<"FN"<<"TN"<<"CC";
        ui->tableWidgetTotalResults->setHorizontalHeaderLabels(tableTotalResutlsLab);

       // fillFirstResultTab();
        optimizeCC();
    }else
    {
        ui->button_export_CC->hide();
        ui->button_optimize_CC->hide();
        ui->tableWidgetTotalResults->horizontalHeader()->setStretchLastSection(true);
        ui->tableWidgetTotalResults->setColumnCount(1);
        ui->tableWidgetTotalResults->insertRow(0);
        QTableWidgetItem * item=new QTableWidgetItem("No shuffle has been made : no result to display.");
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

        ui->tableWidgetTotalResults->setItem(0,0,item);
    }
}

////Function called to update the state of the view
/// display the targets depending on the giving cutoff value (under 100)
void WindowResults::d(int v){
    double temp = v/100.0;
    ui->lcdNumber->display(temp);
    if(ui->Chb_shouldDisplayResult->isChecked())
    {
        //delete lines of the table
        ui->tableWidgetResults->setRowCount(0);
        for(unsigned int i=0;i<m_vTargets.size();i++)
        {
            if(m_vTargets[i].getScoreUnderOne()>=temp){
                WindowResults::insertRow(i);
            }
        }
    }
    fillFirstResultTab(temp);
}

void WindowResults::insertRow(int i){
   // double t=m_vTargets[i].getScoreUnderOne();
   // double v=ui->horizontalSlider->value()/100.0;
   // if(m_vTargets[i].getScoreUnderOne()<ui->horizontalSlider->value()/100.0)
    //{
       myListItem model;
        model.setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->tableWidgetResults->insertRow( ui->tableWidgetResults->rowCount());
       //for int col
       myListItem *item = new myListItem;
       item->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
       // à rajouter un getter dans target
       //Col 0
       ui->tableWidgetResults->setItem( ui->tableWidgetResults->rowCount()-1,0,new QTableWidgetItem(QString::fromStdString(m_vTargets[i].getGene().m_sGroupNumber)));
       //Col 1
        ui->tableWidgetResults->setItem( ui->tableWidgetResults->rowCount()-1,1,new QTableWidgetItem(QString::fromStdString(m_vTargets[i].getMiRNA()->name)));
       //Col 2
       item = new myListItem;
       item->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
       item->setText(QString::fromStdString(utils::convertToString<long>(m_vTargets[i].getPositionStart())));
        ui->tableWidgetResults->setItem( ui->tableWidgetResults->rowCount()-1,2,item);
       //Col 3
       item =new myListItem;
       item->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
       item->setText(QString::fromStdString(utils::convertToString<long>(m_vTargets[i].getPositionEnd())));
        ui->tableWidgetResults->setItem( ui->tableWidgetResults->rowCount()-1,3,item);
       //Col 4
       item = new myListItem;
       item->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
       item->setText(QString::fromStdString(utils::convertToString<long>(m_vTargets[i].getNbMatchInFirsts())));
        ui->tableWidgetResults->setItem( ui->tableWidgetResults->rowCount()-1,4,item);
       //Col 5 (score)
       item = new myListItem;
       item->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
       item->setText(QString::fromStdString(utils::convertToString<double>(m_vTargets[i].getScoreUnderOne())));
        ui->tableWidgetResults->setItem( ui->tableWidgetResults->rowCount()-1,5,item);

       //tab->item(tab->rowCount()-1,0)->checkState();
   // }
}

WindowResults::~WindowResults()
{
    delete ui;
}

QWidget * WindowResults::getCenteredWidget()
{
    widgetItemCheckBox *pWidget = new widgetItemCheckBox();
    QCheckBox *pCheckBox = new QCheckBox();
    QHBoxLayout *pLayout = new QHBoxLayout(pWidget);
    pLayout->addWidget(pCheckBox);
    pLayout->setAlignment(Qt::AlignCenter);
    pLayout->setContentsMargins(0,0,0,0);
    pWidget->setLayout(pLayout);
    return pWidget;
}

double WindowResults::getCC(double p_dCutoff) const
{
    return CorelCoef::getCC(p_dCutoff,m_vTargets,m_vTargetsShuffle,m_iNbSequences);
}

void WindowResults::fillFirstResultTab(double p_dCutoff)
{
    vector<Target> vectorCutoff=Target::getTargets(m_vTargets,p_dCutoff);
    vector<Target> vectorShuffleCutoff=Target::getTargets(m_vTargetsShuffle,p_dCutoff);
    QTableWidget * tab = ui->tableWidgetTotalResults;
    double CC;
    //delete lines of the table
     tab->setRowCount(0);

    unsigned int TP, FP, FN, TN;
    tab->insertRow(tab->rowCount());
    //for int col
    myListItem *item = new myListItem;
    //Col 0 : Total number of sequences
    item->setText(QString::fromStdString(utils::convertToString<unsigned int>(m_iNbSequences)));
    tab->setItem(tab->rowCount()-1,0,item);
    //Col 1 : Total number of found targets
    item=new myListItem;
    item->setText(QString::fromStdString(utils::convertToString<size_t>(vectorCutoff.size())));
    tab->setItem(tab->rowCount()-1,1,item);
    //Col 2 : TP
    TP = CorelCoef::getNumberOfSequencesWithTargets(vectorCutoff);
    item = new myListItem;
    item->setText(QString::fromStdString(utils::convertToString<unsigned int>(TP)));
    tab->setItem(tab->rowCount()-1,2,item);
    //Col 3 : FP
    FP = CorelCoef::getNumberOfSequencesWithTargets(vectorShuffleCutoff);
    item = new myListItem;
    item->setText(QString::fromStdString(utils::convertToString<unsigned int>(FP)));
    tab->setItem(tab->rowCount()-1,3,item);
    //Col 4 : FN
    FN = m_iNbSequences - TP;
    item = new myListItem;
    item->setText(QString::fromStdString(utils::convertToString<unsigned int>(FN)));
    tab->setItem(tab->rowCount()-1,4,item);
    //Col 5 : TN
    TN = m_iNbSequences - FP;
    item = new myListItem;
    item->setText(QString::fromStdString(utils::convertToString<unsigned int>(TN)));
    tab->setItem(tab->rowCount()-1,5,item);
    //Col 6 : CC
    CC=CorelCoef::getCorelCoef(TP,TN,FP,FN);
    item = new myListItem;
    item->setText(QString::fromStdString(CC==CorelCoef::CONST_DEFAULT_WRONG_CC_VALUE?CONST_DEFAULT_NO_CC_STRING:utils::convertToString<double>(CC)));
    tab->setItem(tab->rowCount()-1,6,item);
}

void WindowResults::on_Chb_shouldDisplayResult_stateChanged(int state)
{
    if(state==Qt::Unchecked)
    {
        //unchecked
        ui->tableWidgetResults->setRowCount(0);
    }else if(state == Qt::Checked)
    {
        //checked
        d(ui->horizontalSlider->value());
        if(ui->tableWidgetResults->rowCount()==0)
        {
            setMaximumCutoffWithResults();
        }
    }
}

//we change the current cutoff in order to have at least one target
void WindowResults::setMaximumCutoffWithResults()
{
    double max_score;
    double i(getSelectedCutoff());
    int sliderValue(Settings::getSettings()->getResults_minimumCutoff()*100);
    if(m_vTargets.size()>0)
    {
        max_score=m_vTargets[0].getScoreUnderOne();
        for(unsigned int j=1;j<m_vTargets.size();j++)
        {
            if(max_score<m_vTargets[j].getScoreUnderOne())
            {
                max_score=m_vTargets[j].getScoreUnderOne();
            }
        }
        i=max_score;
    }else
    {
        i=Settings::getSettings()->getResults_minimumCutoff();
    }
    //bool p_bHasBeenFound(false);
    //while(!p_bHasBeenFound)
    //{
    //    p_bHasBeenFound=!(i>CONST_MINIMUM_CUTOFF) || Target::getTargets(m_vTargets,i).size()>0;
    //    i-=CONST_STEP_CUTOFF;
    //}
    sliderValue=(int)(i*100.0);
    ui->horizontalSlider->setValue(sliderValue);    //call d
}

void WindowResults::optimizeCC()
{
    double cutoff(Settings::getSettings()->getResults_minimumCutoff());
    double max_CC=getCC(cutoff);
    double CC(max_CC);
    double p_dBestCutoff(cutoff);
    vector<Target> targets=Target::getTargets(m_vTargets,cutoff);
    vector<Target> targetsShuffle = Target::getTargets(m_vTargetsShuffle, cutoff);
    for(cutoff=Settings::getSettings()->getResults_minimumCutoff();cutoff<Settings::getSettings()->getResults_maximumCutoff();cutoff+=Settings::getSettings()->getResults_stepCutoff())
    {
        targets=Target::getTargets(targets,cutoff);
        targetsShuffle=Target::getTargets(targetsShuffle,cutoff);
        CC=CorelCoef::getCC(cutoff,targets,targetsShuffle, m_iNbSequences);
        if(max_CC<CC)
        {
            max_CC=CC;
            p_dBestCutoff=cutoff;
        }
    }
    bestCutoff=p_dBestCutoff;
    ui->horizontalSlider->setValue(bestCutoff*100); //call d
}

double WindowResults::getSelectedCutoff() const
{
    return ui->horizontalSlider->value()/100.0;
}

void WindowResults::on_exportButton_clicked()
{
    string fileName(utils::appendTextBeforeExtension(m_sFileName.toStdString(),CONST_EXPORT_SUFFIX));
    //fileName = "<nameFile><CONST_EXPORT_SUFFIX>.<EXTENSION>" : we should remove the extension
    fileName = fileName.substr(0,fileName.find(utils::getExtension(fileName))) + CONST_EXPORT_EXTENSION;
    m_oFutureExport = QtConcurrent::run(this, &WindowResults::myExport,fileName);
    this->FutureWatcher.setFuture(m_oFutureExport);

    this->ProgressDialog->setMinimum(0);
    this->ProgressDialog->setMaximum(0);
    this->ProgressDialog->setWindowModality(Qt::WindowModal);
    this->ProgressDialog->exec();
}

bool WindowResults::myExport(string const& p_sFileName)
{
    return Export::exportTargets(p_sFileName, m_vTargets, m_vTargetsShuffle, m_iNbSequences, getSelectedCutoff(),m_bIsCC);
}

void WindowResults::export_finished()
{
    if(m_oFutureExport.result())
    {
        QMessageBox::information(this,tr("Export succeed."),"Successfully export the targets with a score higher than "+QString::fromStdString(utils::convertToString<double>(getSelectedCutoff()))+".");
    }else
    {
        QMessageBox::warning(this,tr("Export failed."),"Failed to export the targets.");
    }
}

void WindowResults::on_button_optimize_CC_clicked()
{
    ui->horizontalSlider->setValue(bestCutoff*100); //call d
}

void WindowResults::on_button_export_CC_clicked()
{
    if(Export::exportCCData(m_sFileName.toStdString(), m_vTargets, m_vTargetsShuffle, m_iNbSequences))
    {
        QMessageBox::information(this,tr("Export succeed."),"Successfully exported CC data.");
    }else
    {
        QMessageBox::warning(this,tr("Export failed."),"Failed to export CC data.");
    }
}

void WindowResults::on_settingsButton_clicked()
{
    WindowSettings *centralWindow = new WindowSettings(this,6,this->parentWidget());
    centralWindow->show();
}
