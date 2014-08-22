#ifndef SETTINGSUPDATER_H
#define SETTINGSUPDATER_H

#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QString>

#include "settings.h"

class SettingsUpdater : public Settings
{
public:
    SettingsUpdater();
    bool loadFromFile();
    void saveToFile();

    //EXTRACT
    void setExtract_supportedExtension(std::string);
    void setExtract_splitChar(char);
    void setExtract_splitCharInColumn(char);
    void setExtract_normalEnsemblPrefix(std::string);
    void setExtract_ContainingEnsemblColumnNumber(unsigned int);

    //DOWNLOAD
    void setDownload_DownloadURL1(std::string);
    void setDownload_DownloadURL2(std::string);
    void setDownload_SequenceFilter(std::string);
    void setDownload_DownloadPath(std::string );
    void setDownload_DownloadedFileSuffix(std::string);
    void setDownload_DownloadedFileExtension(std::string);

    //LOAD
    void setLoad_minMiRNALength(unsigned int);
    void setLoad_maxMiRNALength(unsigned int);
    void setLoad_charBeginHeaderLine(char);

    //SHUFFLE
    void setShuffle_FileNameSuffix(std::string);

    //FIND TARsetS
    void setFind_gapPenalty1(int);
    void setFind_gapPenalty2(int);
    void setFind_defaultKValue(double);
    void setFind_defaultSign(char);
    void setFind_maximumScore(double);
    void setFind_maxDistanceBetweenDouble(unsigned int);

    //RESULTS
    void setResults_stepCutoff(double);
private:
    void read(QJsonObject const& json);
    static const std::string fileName;
};

#endif // SETTINGSUPDATER_H
