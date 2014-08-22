#include "settingsupdater.h"

using namespace std;

const string SettingsUpdater::fileName = "settings.conf";

SettingsUpdater::SettingsUpdater()
{
    //must load the config file
    if(!loadFromFile())
    {
        //must load default parameters
        Extract_supportedExtension=".csv";
    }
}

bool SettingsUpdater::loadFromFile()
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

void SettingsUpdater::read(QJsonObject const& json)
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
    Download_SequenceFilter     = json["DSF"].toString().toStdString();
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
}

void SettingsUpdater::saveToFile()
{
    QJsonObject json;

    //EXTRACT
    json["ESE"]     = QString::fromStdString(Extract_supportedExtension);
    json["ESC"]     = Extract_splitChar;
    json["ESCIC"]   = Extract_splitCharInColumn;
    json["ENEP"]    = QString::fromStdString(Extract_normalEnsemblPrefix);
    json["ECEC"]    = Extract_ContainingEnsemblColumnNumber;

    //DOWNLOAD
    json["DDURL1"]  = QString::fromStdString(Download_DownloadURL1);
    json["DDURL2"]  = QString::fromStdString(Download_DownloadURL2);
    json["DSF"]     = QString::fromStdString(Download_SequenceFilter);
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
}
