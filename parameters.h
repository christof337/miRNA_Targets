#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <string>

/*!
* \file parameters.h
* \brief Contains the default application parameters.
* \author Christophe Pont
* \version 0.1
*/

class Parameters{
public:
    const static bool DEFAULT_PARAM_SHOULD_USE_ROM;        ///true if the user prefer to use ROM instead of RAM ONLY DURING THE LOADING. It is slower but less memory-expensive. Not used in reality
    const static unsigned int DEFAULT_PARAM_LOAD_COLUMN;      ///The column in which the ensembles shall be found.
    const static std::string DEFAULT_SEQUENCE_FILTER;         ///The default filter used to determine weather a header should be added or not.
    const static unsigned int DEFAULT_MINIMUM_MIRNA_LENGTH;   ///The default minimum length of the miRNA
    const static unsigned int DEFAULT_MAXIMUM_MIRNA_LENGTH;   ///The default maximum length of the miRNA (algorithm can work badly if too much modified).
    const static char CHAR_BEGIN_HEADER_LINE;                ///The default char indicating the current fasta format line is a header
    const static double CONST_MINIMUM_CUTOFF;                ///The minimum cutoff that the user can choose. Note : all the finding will consider CONST_MINIMUM_CUTOFF as the current cutoff.
    const static double CONST_MAXIMUM_CUTOFF;                  ///The maximum cutoff that the user can choose (shouldn't be changed).
    //const static double CONST_STEP_CUTOFF = 0.1;                   ///The step

    const static double CONST_STEP_CUTOFF;
    const static double CONST_LARG_STEP_CUTOFF;

    static const std::string SUPPORTED_EXTENSION;					/*! The supported extension to be loaded in the program.*/
    static const char        SPLIT_CHAR;							/*! The character delimiting the columns in the SUPPORTED_EXTENSION format. */
    static const char        SPLIT_CHAR_IN_COLUMN;					/*! The character delimiting the different data in the right column.*/
    static const std::string NORMAL_PREFIX;							/*! The usual prefix of the ensembles. */
    static const std::string OUTPUT_SUFFIX;							/*! The default suffix of the output file. */
    static const std::string OUTPUT_EXTENSION;						/*! The default extension of the output file. */

    static const std::string CONST_SHUFFLE_SUFFIX;
    static const std::string DEFAULT_DOWNLOADED_SUFFIX;
    static const std::string DEFAULT_DOWNLOADED_EXTENSION;
    static const std::string DEFAULT_DOWNLOAD_PATH;
    static const std::string DOWNLOAD_URL_1 ;               ///part of the URL before the ensemble name
    static const std::string DOWNLOAD_URL_2;                ///part of the URL after the ensemble name

    //formal parameters
    static const int const_igapPenalty1;	//Mysterious penalty value
    static const int const_igapPenalty2;	//Mysterious penalty value
    static const long const_lMaxDistance;	//The maximum distance between two targets (for "double")
    static const double DEFAULT_K_VALUE;

    static const char CONST_DEFAULT_SIGN;
    static const double const_dIdealScore2;	//The default ideal score

    static const unsigned long DEFAULT_NB_FIRST;

    //--------------------------
};

#endif // PARAMETERS_H
