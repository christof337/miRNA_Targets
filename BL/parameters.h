#ifndef PARAMETERS_H
#define PARAMETERS_H

/*!
 * \file parameters.h
 * \brief Contains the default application parameters.
 * \author Christophe Pont
 * \version 0.1
 */
const bool DEFAULT_PARAM_SHOULD_USE_ROM = false;
const unsigned int DEFAULT_PARAM_LOAD_COLUMN = 10;
const std::string DEFAULT_SEQUENCE_FILTER = "utr3";
const std::string DEFAULT_DOWNLOADED_PATH = "Download\\ENSG00000112294_format.txt";// DOWNLOADED_ENSEMBLES.txt";
const unsigned int DEFAULT_MINIMUM_MIRNA_LENGTH = 17;
const unsigned int DEFAULT_MAXIMUM_MIRNA_LENGTH = 28;
const char CHAR_BEGIN_HEADER_LINE = '>';
const double CONST_MINIMUM_CUTOFF = 0.5;
const double CONST_MAXIMUM_CUTOFF = 0.5;
const double CONST_STEP_CUTOFF = 0.1;
const int DEFAULT_SCORE_DECIMAL_NUMBER = 3;

#endif // PARAMETERS_H
