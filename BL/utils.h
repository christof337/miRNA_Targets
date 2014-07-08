#ifndef UTILS_H
#define UTILS_H

#include <string>

class utils
{
public:
    static std::string getExtension(std::string const& fileName);
    static std::string appendTextBeforeExtension(std::string const& inputStr, std::string const& strToAppend);
};

#endif // UTILS_H
