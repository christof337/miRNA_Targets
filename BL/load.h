#ifndef LOAD_H
#define LOAD_H

/*!
 * \file load.h
 * \brief Load and treat the input excel file (containing the ensemble list).
 * \author Christophe Pont
 * \version 0.1
 */
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "parameters.h"
#include "utils.h"

/*! \def SUPPORTED_EXTENSION
\brief The extension that can be analyze with the static method Load"()".
*/
#define SUPPORTED_EXTENSION "csv" /* The supported extension to be loaded in the program.*/
#define SPLIT_CHAR "," /* The character delimiting the columns in the SUPPORTED_EXTENSION extension. */
#define SPLIT_CHAR_IN_COLUMN ";" /* The character delimiting the different data in the right column.*/
#define NORMAL_PREFIX "ENSG" /* The usual prefix of the ensembles. */
#define OUTPUT_SUFFIX "_extract" /* The default suffix of the output file. */
#define OUTPUT_EXTENSION ".txt" /* The default extension of the output file. */

//ERROR MESSAGES
#define ERROR_MESSAGE_FILE_DO_NOT_EXISTS "Error : input file doesn't exists."
#define ERROR_MESSAGE_WRONG_FORMAT_1 "Error : thank you to convert the file into '."
#define ERROR_MESSAGE_WRONG_FORMAT_2 "'."
#define ERROR_MESSAGE_NO_ENSEMBLE_FOUND_1 "Error : No ensembles found in the given column ("
#define ERROR_MESSAGE_NO_ENSEMBLE_FOUND_2 ") of the given file.\n"
#define ERROR_MESSAGE_NOT_ENOUGH_COLUMNS_1 "Error : Not enough columns found in the given file ("
#define ERROR_MESSAGE_NOT_ENOUGH_COLUMNS_2 ") to reach the specified column ("
#define ERROR_MESSAGE_NOT_ENOUGH_COLUMNS_3 ")."

/*! \class Load
  * \brief Class representing the loading.
  *
  *  This class handle the loading (mainly with static methods).
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
    static std::vector<std::string> loadAFile(std::string const& path, bool const shouldWriteInAFile=DEFAULT_PARAM_SHOULD_USE_ROM, int const column=DEFAULT_PARAM_LOAD_COLUMN);

private:
	/*!
	*  \brief Check if the caracter at the given position is or is not between quotes.
	*
	*  This static method look for the different quotes in the given string and determine if the character at the given position is beetween two quotes.
	* Meaning : is there an odd number of quotes before this position.
	*
	*  \param position : The position of the character to be tested in the string.
	*  \param str : The string containing the position. Its length should be upper than "position".
	*  \return true if the character at the position "position" is between two quotes in "str", false if not.
	*/
	static bool isBetweenQuotes(int const position, std::string const& str);
};

#endif // LOAD_H
