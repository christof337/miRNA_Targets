#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <sstream>

class utils
{
public:
    static std::string getExtension(std::string const& fileName);
    static std::string appendTextBeforeExtension(std::string const& inputStr, std::string const& strToAppend);
	static std::string convertInt(int const number);
};

#endif // UTILS_H
