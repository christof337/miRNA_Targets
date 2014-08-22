#ifndef PATHHANDLER_H
#define PATHHANDLER_H

#include <string>

#include "utils.h"
#include "settings.h"

class PathHandler
{
public:
    static std::string getShuffledFileName(std::string const& p_sFileName);
    static std::string getDownloadedFileName(std::string const& p_sFileName, bool p_bWithPath=true);
private:
};

#endif // PATHHANDLER_H
