#ifndef QTEXCEPTIONWRAPPER_H
#define QTEXCEPTIONWRAPPER_H

#include <QtConcurrent/qtconcurrentexception.h>
#include <vector>
#include <string>

#include "targetFinder.h"
#include "download.h"
#include "load.h"

class MyException : public QtConcurrent::Exception
 {
public:
    MyException(std::string const& p_sStr="") throw() : m_sStr(p_sStr) {};
    void raise() const  { throw *this; };
    QtConcurrent::Exception *clone() const  { return new MyException(*this); };
    virtual const char* what() const throw() { return m_sStr.c_str(); };
private:
    std::string m_sStr;
};

class QtExceptionWrapper
{
public:
    static std::vector<std::vector<std::string>> download_downloadEnsembles(std::vector<std::string> const& ensList, std::string const& _fileName);
    static bool targetFinder_getMiRNAs(TargetFinder * tf, std::string const& inputFileName);
    static std::vector<std::string> load_loadAFile(std::string const& path);
};

#endif // QTEXCEPTIONWRAPPER_H
