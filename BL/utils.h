#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <sstream>
#include <vector>

class utils
{
public:
    static std::string getExtension(std::string const& fileName);
    static std::string appendTextBeforeExtension(std::string const& inputStr, std::string const& strToAppend);
	static std::string convertInt(int const number);
	static std::vector<std::string> splitString(std::string const& str, char const splitChar, bool const shouldIgnoreQuotes = false);
private:	
	static bool isBetweenQuotes(int const position, std::string const& str);
};

#endif // UTILS_H
