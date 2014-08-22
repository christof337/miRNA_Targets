#ifndef LOAD_H
#define LOAD_H

/*!
 * \file load.h
 * \brief Load and treat the input excel file (containing the ensemble list).
 * \author Christophe Pont
 * \version 0.1
 */
#include <fstream>
#include <string>
#include <vector>

#include "settings.h"
#include "utils.h"

/*! \class Load
  * \brief Class handling the loading of the csv ensemble file.
  *
  *  This class handle the loading (mainly with static methods) of the csv ensemble file.
  *  That means that the input excel file is loaded in a vector of string, containing each ensemble needed.
  */
class Load
{
public:
    /*!
     *  \brief Load the list of ensembles from an excel file.
     *
     *  This static method read from the path of the input excel file,
     * and extract all the ensembles of the specified column.
	 * Throw an exception if the file doesn't exists or if its format isn't SUPPORTED_EXTENSION (with appropriate error messages).
	 * It also throw an exception if the input file doesn't have enough columns or if no ensembles were found.
     *
     *  \param path : The full path of the input excel file (can be relative).
     *  \param column : The number of the column in which the data shall be found.
     *  \param shouldWriteInAFile : true if the user prefer to write the corresponding data in a file (instead of using the RAM),
     * false if not.
     *  \return if shouldWriteInAFile == true : A vector containing one string : the path of the resulting output file.
     * if shouldWriteInAFile == false : A string vector containing each one of the ensemble found.
     */
    static std::vector<std::string> loadAFile(std::string const& path, bool shouldWriteInAFile=Settings::getSettings()->getOOO_DEFAULT_PARAM_SHOULD_USE_ROM(), unsigned int column=Settings::getSettings()->getExtract_ContainingEnsemblColumnNumber());
private:

	//ERROR MESSAGES
    static const std::string ERROR_MESSAGE_FILE_DO_NOT_EXISTS ;		///error message of the absence of the input file
    static const std::string ERROR_MESSAGE_WRONG_FORMAT_1 ;			///error message of bad input format 1
    static const std::string ERROR_MESSAGE_WRONG_FORMAT_2 ;			///error message of bad input format 2
    static const std::string ERROR_MESSAGE_NO_ENSEMBLE_FOUND_1 ;	///error message of no ensemble found in the given file and column 1
    static const std::string ERROR_MESSAGE_NO_ENSEMBLE_FOUND_2 ;	///error message of no ensemble found in the given file and column 2
    static const std::string ERROR_MESSAGE_NOT_ENOUGH_COLUMNS_1;	///error message of the lack of columns 1
    static const std::string ERROR_MESSAGE_NOT_ENOUGH_COLUMNS_2 ;	///error message of the lack of columns 2
    static const std::string ERROR_MESSAGE_NOT_ENOUGH_COLUMNS_3;	///error message of the lack of columns 3
};

#endif // LOAD_H
