#ifndef SHUFFLE_H
#define SHUFFLE_H

#include <fstream>
#include <ctime>
#include <string>

#include "pathhandler.h"

class Shuffle
{
public:
   static int mainShuffling(std::string const& _fileName);
private:
   static std::string shuffleString(std::string & mot);
   static const unsigned int CONST_NUMBER_SHUFFLE;
};

#endif // SHUFFLE_H
