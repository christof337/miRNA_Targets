#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>
#include <QJsonObject>
#include <QString>
#include <QFile>
#include <QJsonDocument>

#include "parameters.h"

class Settings
{
public:
    Settings();
    static Settings * getSettings();

    //EXTRACT
     std::string getExtract_supportedExtension() const;
     char getExtract_splitChar() const;
     char getExtract_splitCharInColumn() const;
     std::string getExtract_normalEnsemblPrefix() const;
      int getExtract_ContainingEnsemblColumnNumber() const;

    //DOWNLOAD
     std::string getDownload_DownloadURL1() const;
     std::string getDownload_DownloadURL2() const;
     std::string getDownload_DownloadPath() const;
     std::string getDownload_DownloadedFileSuffix() const;
     std::string getDownload_DownloadedFileExtension() const;

    //LOAD
      int getLoad_minMiRNALength() const;
      int getLoad_maxMiRNALength() const;
     char getLoad_charBeginHeaderLine() const;

    //SHUFFLE
     std::string getShuffle_FileNameSuffix() const;

    //FIND TARGETS
     int getFind_gapPenalty1() const;
     int getFind_gapPenalty2() const;
     double getFind_defaultKValue() const;
     char getFind_defaultSign() const;
     double getFind_maximumScore() const;
      int getFind_maxDistanceBetweenDouble() const;

    //RESULTS
     double getResults_stepCutoff() const;
     double getResults_minimumCutoff() const;
     double getResults_maximumCutoff() const;
      int getResults_nbFirsts() const;

     //OUT OF ORDER
     double getOOO_CONST_LARG_STEP_CUTOFF() const;
     bool getOOO_DEFAULT_PARAM_SHOULD_USE_ROM() const;
     std::string getOOO_outputSuffix() const;
     std::string getOOO_outputExtension() const;
private:
    static Settings * itself;
protected:
    //EXTRACT
    std::string Extract_supportedExtension;
    char Extract_splitChar;
    char Extract_splitCharInColumn;
    std::string Extract_normalEnsemblPrefix;
     int Extract_ContainingEnsemblColumnNumber;

    //DOWNLOAD
    std::string Download_DownloadURL1;
    std::string Download_DownloadURL2;
    std::string Download_DownloadPath;
    std::string Download_DownloadedFileSuffix;
    std::string Download_DownloadedFileExtension;

    //LOAD
     int Load_minMiRNALength;
     int Load_maxMiRNALength;
    char Load_charBeginHeaderLine;

    //SHUFFLE
    std::string Shuffle_FileNameSuffix;

    //FIND TARGETS
    int Find_gapPenalty1;
    int Find_gapPenalty2;
    double Find_defaultKValue;
    char Find_defaultSign;
    double Find_maximumScore;
     int Find_maxDistanceBetweenDouble;

    //RESULTS
    double Results_stepCutoff;
    double Results_minimumCutoff;
    double Results_maximumCutoff;
    int Results_nbFirsts;

    //OUT OF ORDER
    double OOO_CONST_LARG_STEP_CUTOFF;
    bool OOO_DEFAULT_PARAM_SHOULD_USE_ROM;
    std::string OOO_outputSuffix;
    std::string OOO_outputExtension;
    //-----------------------------------
public:
    bool loadFromFile();
    bool saveToFile() const;

    //EXTRACT
    void setExtract_supportedExtension(std::string);
    void setExtract_splitChar(char);
    void setExtract_splitCharInColumn(char);
    void setExtract_normalEnsemblPrefix(std::string);
    void setExtract_ContainingEnsemblColumnNumber(unsigned int);

    //DOWNLOAD
    void setDownload_DownloadURL1(std::string);
    void setDownload_DownloadURL2(std::string);
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
    void setResults_minimumCutoff(double);
    void setResults_maximumCutoff(double);
    void setResults_nbFirsts(int);
private:
    void read(QJsonObject const& json);
    void write(QJsonObject & json) const;
    static const std::string fileName;
    void loadDefaultParam();
};

#endif // SETTINGS_H
