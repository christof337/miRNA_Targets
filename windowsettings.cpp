#include "windowsettings.h"
#include "ui_windowsettings.h"

WindowSettings::WindowSettings(QWidget * previous, int step, QWidget * parent) :
    QTabWidget(parent),
    previousUI(previous),
    ui(new Ui::WindowSettings)
{
    ui->setupUi(this);

    //showing the good index
    this->setCurrentIndex(step);

    initFields();
}

WindowSettings::~WindowSettings()
{
    delete ui;
}

void WindowSettings::initFields(int tab)
{
    if(tab==0||tab==2)
    {
        //EXTRACT
        ui->ESE_lineEdit->setText(QString::fromStdString(Settings::getSettings()->getExtract_supportedExtension()));
        ui->ESC_lineEdit->setText(QChar::fromLatin1(Settings::getSettings()->getExtract_splitChar()));
        ui->ESCIC_lineEdit->setText(QChar::fromLatin1(Settings::getSettings()->getExtract_splitCharInColumn()));
        ui->ENEP_lineEdit->setText(QString::fromStdString(Settings::getSettings()->getExtract_normalEnsemblPrefix()));
        ui->ENCE_spinBox->setValue(Settings::getSettings()->getExtract_ContainingEnsemblColumnNumber());
    }

    if(tab==0||tab==3)
    {
        //DOWNLOAD
        ui->DDURL1_lineEdit->setText(QString::fromStdString(Settings::getSettings()->getDownload_DownloadURL1()));
        ui->DDURL2_lineEdit->setText(QString::fromStdString(Settings::getSettings()->getDownload_DownloadURL2()));
        ui->DDP_lineEdit->setText(QString::fromStdString(Settings::getSettings()->getDownload_DownloadPath()));
        ui->DDFE_lineEdit->setText(QString::fromStdString(Settings::getSettings()->getDownload_DownloadedFileExtension()));
        ui->DFS_lineEdit->setText(QString::fromStdString(Settings::getSettings()->getDownload_DownloadedFileSuffix()));
    }


    if(tab==0||tab==1)
    {
        //LOAD
        ui->LCBHL_lineEdit->setText(QChar::fromLatin1(Settings::getSettings()->getLoad_charBeginHeaderLine()));
        ui->LMinML_spinBox->setValue(Settings::getSettings()->getLoad_minMiRNALength());
        ui->LMaxML_spinBox->setValue(Settings::getSettings()->getLoad_maxMiRNALength());
    }

    if(tab==0||tab==4)
    {
        //SHUFFLE
        ui->SSFN_lineEdit->setText(QString::fromStdString(Settings::getSettings()->getShuffle_FileNameSuffix()));
    }

    if(tab==0||tab==5)
    {
        //FIND TARGETS
        ui->FGP1_spinBox->setValue(Settings::getSettings()->getFind_gapPenalty1());
        ui->FGP2_spinBox->setValue(Settings::getSettings()->getFind_gapPenalty2());
        ui->FDKV_doubleSpinBox->setValue(Settings::getSettings()->getFind_defaultKValue());
        ui->FDS_comboBox->setCurrentText(QChar::fromLatin1(Settings::getSettings()->getFind_defaultSign()));
        ui->FMS_doubleSpinBox->setValue(Settings::getSettings()->getFind_maximumScore());
        ui->FMDD_spinBox->setValue(Settings::getSettings()->getFind_maxDistanceBetweenDouble());
    }

    if(tab==0||tab==6)
    {
        //RESULTS
        ui->RSC_doubleSpinBox->setValue(Settings::getSettings()->getResults_stepCutoff());
        ui->RMinC_doubleSpinBox->setValue(Settings::getSettings()->getResults_minimumCutoff());
        ui->RMaxC_doubleSpinBox->setValue(Settings::getSettings()->getResults_maximumCutoff());
        ui->RNF_spinBox->setValue(Settings::getSettings()->getResults_nbFirsts());
    }
}

void WindowSettings::validate() const
{
    //EXTRACT
    Settings::getSettings()->setExtract_supportedExtension(ui->ESE_lineEdit->text().toStdString());
    Settings::getSettings()->setExtract_splitChar(ui->ESC_lineEdit->text().toStdString()[0]);
    Settings::getSettings()->setExtract_splitCharInColumn(ui->ESCIC_lineEdit->text().toStdString()[0]);
    Settings::getSettings()->setExtract_normalEnsemblPrefix(ui->ENEP_lineEdit->text().toStdString());
    Settings::getSettings()->setExtract_ContainingEnsemblColumnNumber(ui->ENCE_spinBox->value());

    //DOWNLOAD
    Settings::getSettings()->setDownload_DownloadURL1(ui->DDURL1_lineEdit->text().toStdString());
    Settings::getSettings()->setDownload_DownloadURL2(ui->DDURL2_lineEdit->text().toStdString());
    Settings::getSettings()->setDownload_DownloadPath(ui->DDP_lineEdit->text().toStdString());
    Settings::getSettings()->setDownload_DownloadedFileSuffix(ui->DFS_lineEdit->text().toStdString());
    Settings::getSettings()->setDownload_DownloadedFileExtension(ui->DDFE_lineEdit->text().toStdString());

    //LOAD
    Settings::getSettings()->setLoad_minMiRNALength(ui->LMinML_spinBox->value());
    Settings::getSettings()->setLoad_maxMiRNALength(ui->LMaxML_spinBox->value());
    Settings::getSettings()->setLoad_charBeginHeaderLine(ui->LCBHL_lineEdit->text().toStdString()[0]);

    //SHUFFLE
    Settings::getSettings()->setShuffle_FileNameSuffix(ui->SSFN_lineEdit->text().toStdString());

    //FIND TARGETS
    Settings::getSettings()->setFind_gapPenalty1(ui->FGP1_spinBox->value());
    Settings::getSettings()->setFind_gapPenalty2(ui->FGP2_spinBox->value());
    Settings::getSettings()->setFind_defaultKValue(ui->FDKV_doubleSpinBox->value());
    Settings::getSettings()->setFind_defaultSign(ui->FDS_comboBox->currentText().toStdString()[0]);
    Settings::getSettings()->setFind_maximumScore(ui->FMS_doubleSpinBox->value());
    Settings::getSettings()->setFind_maxDistanceBetweenDouble(ui->FMDD_spinBox->value());

    //RESULTS
    Settings::getSettings()->setResults_stepCutoff(ui->RSC_doubleSpinBox->value());
    Settings::getSettings()->setResults_minimumCutoff(ui->RMinC_doubleSpinBox->value());
    Settings::getSettings()->setResults_maximumCutoff(ui->RMaxC_doubleSpinBox->value());
    Settings::getSettings()->setResults_nbFirsts(ui->RNF_spinBox->value());

    if(Settings::getSettings()->saveToFile())
    {
        QMessageBox::information(this->parentWidget(),tr("Settings saved successfully."),"Settings successfully saved.");
    }else
    {
        QMessageBox::warning(this->parentWidget(),tr("Failed to save settings."),"Failed to save the settings. The settings you chose are viable only for this session.");
    }

}

void WindowSettings::hideIt()
{
    this->hide();
}

void WindowSettings::on_button_Rcancel_2_clicked()
{
    hideIt();
}

void WindowSettings::on_button_Lcancel_clicked()
{
    hideIt();
}

void WindowSettings::on_button_Ecancel_clicked()
{
    hideIt();
}

void WindowSettings::on_button_Dcancel_clicked()
{
    hideIt();
}

void WindowSettings::on_button_Scancel_clicked()
{
    hideIt();
}

void WindowSettings::on_button_Fcancel_clicked()
{
    hideIt();
}

void WindowSettings::on_button_Rcancel_clicked()
{
    hideIt();
}

void WindowSettings::on_button_Rsave_2_clicked()
{
    validate();
    hideIt();
}

void WindowSettings::on_button_Lsave_clicked()
{
    validate();
    hideIt();
}

void WindowSettings::on_button_Esave_clicked()
{
    validate();
    hideIt();
}

void WindowSettings::on_button_Dsave_clicked()
{
    validate();
    hideIt();
}

void WindowSettings::on_button_Ssave_clicked()
{
    validate();
    hideIt();
}

void WindowSettings::on_button_Fsave_clicked()
{
    validate();
    hideIt();
}

void WindowSettings::on_button_Rsave_clicked()
{
    validate();
    hideIt();
}

void WindowSettings::on_button_Ldefault_clicked()
{
    initFields(this->currentIndex());
}

void WindowSettings::on_button_Edefault_clicked()
{
    initFields(this->currentIndex());
}

void WindowSettings::on_button_Ddefault_clicked()
{
    initFields(this->currentIndex());
}

void WindowSettings::on_button_Sdefault_clicked()
{
    initFields(this->currentIndex());
}

void WindowSettings::on_button_Fdefault_clicked()
{
    initFields(this->currentIndex());
}

void WindowSettings::on_button_Rdefault_clicked()
{
    initFields(this->currentIndex());
}
