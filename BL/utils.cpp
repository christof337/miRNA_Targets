#include "utils.h"

using namespace std;

string utils::getExtension(string const& fileName)
{
    int found=fileName.find_last_of(".");
    if(found==string::npos)
    {
        return "";
    }else
    {
        return fileName.substr(found+1);
    }
}

string utils::appendTextBeforeExtension(string const& inputStr, string const& strToAppend)
{
    return inputStr.substr(0, inputStr.length() - getExtension(inputStr).length()) + strToAppend + getExtension(inputStr);
}
