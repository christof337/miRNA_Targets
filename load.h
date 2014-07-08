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

#define SUPPORTED_EXTENSION ".csv"
#define SPLIT_CHAR ","
#define SPLIT_CHAR_IN_COLUMN ";"
#define NORMAL_PREFIX "ENSG"

/*! \class Load
  * \brief Class representing the loading.
  *
  *  This class handle the loading (mainly with static methods).
  */
class Load
{
public:
    /*!
     *  \brief Load the list of ensembles from an excel file
     *
     *  This static method read from the path of the input excel file,
     * and extract all the ensembles of the specified column.
     *
     *  \param path : The full path of the input excel file (can be relative).
     *  \param column : The number of the column in which the data shall be found.
     *  \param shouldWriteInAFile : true if the user prefer to write the corresponding data in a file (instead of using the RAM),
     * false if not.
     *  \return if shouldWriteInAFile == true : A vector containing one string : the path of the resulting output file.
     * if shouldWriteInAFile == false : A string vector containing each one of the ensemble found.
     */
    static std::vector<std::string> loadAFile(std::string const& path, int column=DEFAULT_PARAM_LOAD_COLUMN, bool shouldWriteInAFile=DEFAULT_PARAM_SHOULD_USE_ROM);
};

#endif // LOAD_H
