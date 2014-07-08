#include "utils.h"

using namespace std;

string utils::getExtension(string const& fileName)
{
	string res;
    int found=fileName.find_last_of(".");
    if(found==string::npos)
    {
        return res;
    }else
    {
        return fileName.substr(found+1);
    }
}

string utils::appendTextBeforeExtension(string const& inputStr, string const& strToAppend)
{
	return inputStr.substr(0, inputStr.length() - getExtension(inputStr).length()-1) + strToAppend + "."+getExtension(inputStr);
}

string utils::convertInt(int number)
{
	stringstream ss;//create a stringstream
	ss << number;//add number to the stream
	return ss.str();//return a string with the contents of the stream
}