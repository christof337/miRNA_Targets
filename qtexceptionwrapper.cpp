#include "qtexceptionwrapper.h"

using namespace std;

vector<vector<string>> QtExceptionWrapper::download_downloadEnsembles(std::vector<std::string> const& ensList, std::string const& fileName)
{
    vector<vector<string>> res;
    try
    {
        res=Download::downloadEnsembles(ensList,fileName);
        return res;
    }catch(string const& e)
    {
        MyException ex(e);
        ex.raise();
    }
    return res;
}

bool QtExceptionWrapper::targetFinder_getMiRNAs(TargetFinder * tf, string const& inputFileName)
{
    bool res(false);
    try
    {
        res=tf->getMiRNAs(inputFileName);
        return res;
    }catch(string const& e)
    {
        MyException ex(e);
        ex.raise();
    }
    return res;
}

vector<string> QtExceptionWrapper::load_loadAFile(string const& path)
{
    vector<string> res;
    try
    {
        res=Load::loadAFile(path);
        return res;
    }catch(string const& e)
    {
        MyException ex(e);
        ex.raise();
    }
    return res;
}
