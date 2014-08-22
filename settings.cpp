#include "settings.h"

Settings * Settings::itself=0;

const std::string Settings::fileName="setting.conf";

Settings::Settings()
{
    //must load the config file
    if(!loadFromFile())
    {
        //must load default parameters
        loadDefaultParam();

        //then save
        saveToFile();
    }
}

Settings * Settings::getSettings()
{
    if(itself==0)
    {
        itself = new Settings();
    }
    return itself;
}

bool Settings::loadFromFile()
{
    QFile loadFile(fileName.c_str());
    if (!loadFile.open(QIODevice::ReadOnly)) {
           return false;
    }

    QByteArray saveSettings = loadFile.readAll();

    QJsonDocument loadDoc(QJsonDocument::fromJson(saveSettings));

    read(loadDoc.object());
    return true;
}

bool Settings::saveToFile() const
{
    QFile saveFile(fileName.c_str());

    if (!saveFile.open(QIODevice::WriteOnly)) {
        return false;
    }

    QJsonObject settingsObject;
    write(settingsObject);
    QJsonDocument saveDoc(settingsObject);
    saveFile.write(saveDoc.toJson());

    return true;
}

void Settings::read(QJsonObject const& json)
{
    //EXTRACT
    Extract_supportedExtension  = json["ESE"].toString().toStdString();
    Extract_splitChar           = json["ESC"].toInt();
    Extract_splitCharInColumn   = json["ESCIC"].toInt();
    Extract_normalEnsemblPrefix = json["ENEP"].toString().toStdString();
    Extract_ContainingEnsemblColumnNumber   = json["ECEC"].toInt();

    //DOWNLOAD
    Download_DownloadURL1       = json["DDURL1"].toString().toStdString();
    Download_DownloadURL2       = json["DDURL2"].toString().toStdString();
    Download_DownloadPath       = json["DDP"].toString().toStdString();
    Download_DownloadedFileSuffix   = json["DDFS"].toString().toStdString();
    Download_DownloadedFileExtension    = json["DDFE"].toString().toStdString();

    //LOAD
    Load_minMiRNALength = json["LMinML"].toInt();
    Load_maxMiRNALength = json["LMaxML"].toInt();
    Load_charBeginHeaderLine    = json["LCBHL"].toInt();

    //SHUFFLE
    Shuffle_FileNameSuffix  = json["SFNS"].toString().toStdString();

    //FIND TARGETS
    Find_gapPenalty1    = json["FGP1"].toInt();
    Find_gapPenalty2    = json["FCP2"].toInt();
    Find_defaultKValue  = json["FDKV"].toDouble();
    Find_defaultSign    = json["FDS"].toInt();
    Find_maximumScore   = json["FMS"].toDouble();
    Find_maxDistanceBetweenDouble   = json["FMDBD"].toInt();

    //RESULTS
    Results_stepCutoff  = json["RSC"].toDouble();
    Results_minimumCutoff = json["RMinC"].toDouble();
    Results_maximumCutoff = json["RMaxC"].toDouble();
    Results_nbFirsts = json["RNF"].toInt();

    //OUT OF ORDER
    OOO_CONST_LARG_STEP_CUTOFF = Parameters::CONST_LARG_STEP_CUTOFF;
    OOO_DEFAULT_PARAM_SHOULD_USE_ROM=Parameters::DEFAULT_PARAM_SHOULD_USE_ROM;
    OOO_outputSuffix=Parameters::OUTPUT_SUFFIX;
    OOO_outputExtension=Parameters::OUTPUT_EXTENSION;
}


void Settings::write(QJsonObject & json) const
{
    //EXTRACT
    json["ESE"]     = QString::fromStdString(Extract_supportedExtension);
    json["ESC"]     = Extract_splitChar;
    json["ESCIC"]   = Extract_splitCharInColumn;
    json["ENEP"]    = QString::fromStdString(Extract_normalEnsemblPrefix);
    json["ECEC"]    = Extract_ContainingEnsemblColumnNumber;

    //DOWNLOAD
    json["DDURL1"]  = QString::fromStdString(Download_DownloadURL1);
    json["DDURL2"]  = QString::fromStdString(Download_DownloadURL2);
    json["DDP"]     = QString::fromStdString(Download_DownloadPath);
    json["DDFS"]    = QString::fromStdString(Download_DownloadedFileSuffix);
    json["DDFE"]    = QString::fromStdString(Download_DownloadedFileExtension);

    //LOAD
    json["LMinML"]   = Load_minMiRNALength;
    json["LMaxML"]   = Load_maxMiRNALength;
    json["LCBHL"]    = Load_charBeginHeaderLine;

    //SHUFFLE
    json["SFNS"]     = QString::fromStdString(Shuffle_FileNameSuffix);

    //FIND TARGETS
    json["FGP1"]     = Find_gapPenalty1;
    json["FCP2"]     = Find_gapPenalty2;
    json["FDKV"]     = Find_defaultKValue;
    json["FDS"]      = Find_defaultSign;
    json["FMS"]      = Find_maximumScore;
    json["FMDBD"]    = Find_maxDistanceBetweenDouble;

    //RESULTS
    json["RSC"]      = Results_stepCutoff;
    json["RMinC"]    = Results_minimumCutoff;
    json["RMaxC"]    = Results_maximumCutoff;
    json["RNF"]      = Results_nbFirsts;
}

void Settings::loadDefaultParam()
{
    //EXTRACT
    Extract_supportedExtension  = Parameters::SUPPORTED_EXTENSION;
    Extract_splitChar           = Parameters::SPLIT_CHAR;
    Extract_splitCharInColumn   = Parameters::SPLIT_CHAR_IN_COLUMN;
    Extract_normalEnsemblPrefix = Parameters::NORMAL_PREFIX;
    Extract_ContainingEnsemblColumnNumber = Parameters::DEFAULT_PARAM_LOAD_COLUMN;

    //DOWNLOAD
    Download_DownloadURL1       = Parameters::DOWNLOAD_URL_1;
    Download_DownloadURL2       = Parameters::DOWNLOAD_URL_2;
    Download_DownloadPath       = Parameters::DEFAULT_DOWNLOAD_PATH;
    Download_DownloadedFileSuffix   = Parameters::DEFAULT_DOWNLOADED_SUFFIX;
    Download_DownloadedFileExtension    = Parameters::DEFAULT_DOWNLOADED_EXTENSION;

    //LOAD
    Load_minMiRNALength = Parameters::DEFAULT_MINIMUM_MIRNA_LENGTH;
    Load_maxMiRNALength = Parameters::DEFAULT_MAXIMUM_MIRNA_LENGTH;
    Load_charBeginHeaderLine = Parameters::CHAR_BEGIN_HEADER_LINE;

    //SHUFFLE
    Shuffle_FileNameSuffix  = Parameters::CONST_SHUFFLE_SUFFIX;

    //FIND TARGETS
    Find_gapPenalty1    = Parameters::const_igapPenalty1;
    Find_gapPenalty2    = Parameters::const_igapPenalty2;
    Find_defaultKValue  = Parameters::DEFAULT_K_VALUE;
    Find_defaultSign    = Parameters::CONST_DEFAULT_SIGN;
    Find_maximumScore   = Parameters::const_dIdealScore2;
    Find_maxDistanceBetweenDouble   = Parameters::const_lMaxDistance;

    //RESULTS
    Results_stepCutoff  = Parameters::CONST_STEP_CUTOFF;
    Results_minimumCutoff = Parameters::CONST_MINIMUM_CUTOFF;
    Results_maximumCutoff = Parameters::CONST_MAXIMUM_CUTOFF;
    Results_nbFirsts = Parameters::DEFAULT_NB_FIRST;

    //OUT OF ORDER
    OOO_CONST_LARG_STEP_CUTOFF = Parameters::CONST_LARG_STEP_CUTOFF;
    OOO_DEFAULT_PARAM_SHOULD_USE_ROM=Parameters::DEFAULT_PARAM_SHOULD_USE_ROM;
    OOO_outputSuffix=Parameters::OUTPUT_SUFFIX;
    OOO_outputExtension=Parameters::OUTPUT_EXTENSION;
}

//EXTRACT
 std::string Settings::getExtract_supportedExtension() const{ return Extract_supportedExtension ;}
 char Settings::getExtract_splitChar() const{ return Extract_splitChar ;}
 char Settings::getExtract_splitCharInColumn() const{ return Extract_splitCharInColumn ;}
 std::string Settings::getExtract_normalEnsemblPrefix() const{ return Extract_normalEnsemblPrefix ;}
  int Settings::getExtract_ContainingEnsemblColumnNumber() const{ return Extract_ContainingEnsemblColumnNumber ;}

//DOWNLOAD
 std::string Settings::getDownload_DownloadURL1() const{ return Download_DownloadURL1 ;}
 std::string Settings::getDownload_DownloadURL2() const{ return Download_DownloadURL2 ;}
 std::string Settings::getDownload_DownloadPath() const{ return Download_DownloadPath ;}
 std::string Settings::getDownload_DownloadedFileSuffix() const{ return Download_DownloadedFileSuffix ;}
 std::string Settings::getDownload_DownloadedFileExtension() const{ return Download_DownloadedFileExtension ;}

//LOAD
  int Settings::getLoad_minMiRNALength() const{ return Load_minMiRNALength ;}
  int Settings::getLoad_maxMiRNALength() const{ return Load_maxMiRNALength ;}
 char Settings::getLoad_charBeginHeaderLine() const{ return Load_charBeginHeaderLine ;}

//SHUFFLE
 std::string Settings::getShuffle_FileNameSuffix() const{ return Shuffle_FileNameSuffix ;}

//FIND TARGETS
 int Settings::getFind_gapPenalty1() const{ return Find_gapPenalty1 ;}
 int Settings::getFind_gapPenalty2() const{ return Find_gapPenalty2 ;}
 double Settings::getFind_defaultKValue() const{ return Find_defaultKValue ;}
 char Settings::getFind_defaultSign() const{ return Find_defaultSign ;}
 double Settings::getFind_maximumScore() const{ return Find_maximumScore ;}
  int Settings::getFind_maxDistanceBetweenDouble() const{ return Find_maxDistanceBetweenDouble ;}

//RESULTS
 double Settings::getResults_stepCutoff() const{ return Results_stepCutoff ;}
 double Settings::getResults_minimumCutoff() const { return Results_minimumCutoff; }
 double Settings::getResults_maximumCutoff() const { return Results_maximumCutoff; }
 int Settings::getResults_nbFirsts() const { return Results_nbFirsts; }

 //OUT OF ORDER
 double Settings::getOOO_CONST_LARG_STEP_CUTOFF() const { return OOO_CONST_LARG_STEP_CUTOFF ; }
 bool Settings::getOOO_DEFAULT_PARAM_SHOULD_USE_ROM() const { return OOO_DEFAULT_PARAM_SHOULD_USE_ROM; }
std::string Settings::getOOO_outputSuffix() const { return OOO_outputSuffix;}
std::string Settings::getOOO_outputExtension() const { return OOO_outputExtension;}


//---------------------- SET
//EXTRACT
void Settings::setExtract_supportedExtension(std::string e){Extract_supportedExtension=e;}
void Settings::setExtract_splitChar(char e) { Extract_splitChar =e; }
void Settings::setExtract_splitCharInColumn(char e) { Extract_splitCharInColumn =e; }
void Settings::setExtract_normalEnsemblPrefix(std::string e) {  Extract_normalEnsemblPrefix=e; }
void Settings::setExtract_ContainingEnsemblColumnNumber(unsigned int e) {  Extract_ContainingEnsemblColumnNumber=e; }

//DOWNLOAD
void Settings::setDownload_DownloadURL1(std::string e) {  Download_DownloadURL1=e; }
void Settings::setDownload_DownloadURL2(std::string e) {  Download_DownloadURL2=e; }
void Settings::setDownload_DownloadPath(std::string e ) { Download_DownloadPath=e;}
void Settings::setDownload_DownloadedFileSuffix(std::string e) { Download_DownloadedFileSuffix =e; }
void Settings::setDownload_DownloadedFileExtension(std::string e) { Download_DownloadedFileExtension =e; }

//LOAD
void Settings::setLoad_minMiRNALength(unsigned int e) { Load_minMiRNALength =e; }
void Settings::setLoad_maxMiRNALength(unsigned int e) {  Load_maxMiRNALength=e; }
void Settings::setLoad_charBeginHeaderLine(char e) {  Load_charBeginHeaderLine=e; }

//SHUFFLE
void Settings::setShuffle_FileNameSuffix(std::string e) {  Shuffle_FileNameSuffix=e; }

//FIND TARsetS
void Settings::setFind_gapPenalty1(int e) {  Find_gapPenalty1=e; }
void Settings::setFind_gapPenalty2(int e) {  Find_gapPenalty2=e; }
void Settings::setFind_defaultKValue(double e) {  Find_defaultKValue=e; }
void Settings::setFind_defaultSign(char e) {  Find_defaultSign=e; }
void Settings::setFind_maximumScore(double e) { Find_maximumScore =e; }
void Settings::setFind_maxDistanceBetweenDouble(unsigned int e) { Find_maxDistanceBetweenDouble =e; }

//RESULTS
void Settings::setResults_stepCutoff(double e) {  Results_stepCutoff=e; }
void Settings::setResults_minimumCutoff(double e) { Results_minimumCutoff =e; }
void Settings::setResults_maximumCutoff(double e) {  Results_maximumCutoff=e; }
void Settings::setResults_nbFirsts(int e) {  Results_nbFirsts=e; }
