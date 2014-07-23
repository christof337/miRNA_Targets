#include "utils.h"

using namespace std;

string utils::getExtension(string const& fileName)
{
	string res;
    size_t found=fileName.find_last_of(".");
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

vector<string> utils::splitString(string const& str, char const splitChar, bool const shouldIgnoreQuotes)
{
	vector<string> res;
	unsigned int lastSplitPosition(0);
	for (unsigned int i = 0; i < str.length(); i++)
	{
		if (str[i] == splitChar && !(shouldIgnoreQuotes && isBetweenQuotes(i,str)))
		{
			res.push_back(str.substr(lastSplitPosition+1, i - lastSplitPosition - 1));
			lastSplitPosition = i;
		}
	}
	res.push_back(str.substr(lastSplitPosition, str.length() - lastSplitPosition));
	return res;
}

bool utils::isBetweenQuotes(int const position, string const& str)
{
	int found(-1), found2(-1);
	unsigned int i(0);
	bool isBeetween(false); /* flag raised when the character at the position "position" is between two quotes.
						   Try this algorithm to verify :
						   ------------------------
						   string str1 = "012\"456\"89"; //the string is : 012"456"89
						   //456 should return true, the other, false
						   cout << str1 << endl;
						   for (int i = 0; i < 10; i++)
						   {
						   if (Load::isBetweenQuotes(i, str1))
						   {
						   cout << i << " : " << "true" << endl;
						   }
						   else
						   {
						   cout << i << " : " << "false" << endl;
						   }
						   }
						   ------------------------
						   */
	while (i < str.length() && !isBeetween)
	{
		if (str[i] == '"')
		{
			found2 = found;
			found = i;
		}
		isBeetween = (position < found && position > found2 && found >= 0 && found2 >= 0);
		i++;
	}
	return isBeetween;
}

string utils::toLower(string str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	return str;
}

string utils::toUpper(string str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::toupper);
	return str;
}