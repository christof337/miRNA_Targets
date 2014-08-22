#include "pathhandler.h"

using namespace std;

string PathHandler::getShuffledFileName(string const& p_sFileName)
{
    return utils::appendTextBeforeExtension(p_sFileName,Settings::getSettings()->getShuffle_FileNameSuffix());
}

string PathHandler::getDownloadedFileName(string const& p_sFileName, bool p_bWithPath)
{
    return (p_bWithPath?Settings::getSettings()->getDownload_DownloadPath():"") + utils::getName(p_sFileName) + Settings::getSettings()->getDownload_DownloadedFileSuffix() + Settings::getSettings()->getDownload_DownloadedFileExtension();
}
