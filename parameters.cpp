#include "parameters.h"

using namespace std;

const bool Parameters::DEFAULT_PARAM_SHOULD_USE_ROM = false;        ///true if the user prefer to use ROM instead of RAM ONLY DURING THE LOADING. It is slower but less memory-expensive. Not used in reality
const unsigned int Parameters::DEFAULT_PARAM_LOAD_COLUMN = 10;      ///The column in which the ensembles shall be found.
const std::string Parameters::DEFAULT_SEQUENCE_FILTER = "";         ///The default filter used to determine weather a header should be added or not.
const unsigned int Parameters::DEFAULT_MINIMUM_MIRNA_LENGTH = 17;   ///The default minimum length of the miRNA
const unsigned int Parameters::DEFAULT_MAXIMUM_MIRNA_LENGTH = 28;   ///The default maximum length of the miRNA (algorithm can work badly if too much modified).
const char Parameters::CHAR_BEGIN_HEADER_LINE = '>';                ///The default char indicating the current fasta format line is a header
const double Parameters::CONST_MINIMUM_CUTOFF = 0.5;                ///The minimum cutoff that the user can choose. Note : all the finding will consider CONST_MINIMUM_CUTOFF as the current cutoff.
const double Parameters::CONST_MAXIMUM_CUTOFF = 1;                  ///The maximum cutoff that the user can choose (shouldn't be changed).
//const double CONST_STEP_CUTOFF = 0.1;                   ///The step

const double Parameters::CONST_STEP_CUTOFF=0.01;
const double Parameters::CONST_LARG_STEP_CUTOFF=0.05;

const string Parameters::SUPPORTED_EXTENSION = "csv";			/* The supported extension to be loaded in the program.*/
const char Parameters::SPLIT_CHAR = ',';								/* The character delimiting the columns in the SUPPORTED_EXTENSION format. */
const char Parameters::SPLIT_CHAR_IN_COLUMN = ';';					/* The character delimiting the different data in the right column.*/
const string Parameters::NORMAL_PREFIX = "ENSG";					/* The usual prefix of the ensembles. */
const string Parameters::OUTPUT_SUFFIX = "_extract";				/* The default suffix of the output file. */
const string Parameters::OUTPUT_EXTENSION = ".txt";				/* The default extension of the output file. */

const string Parameters::DEFAULT_DOWNLOADED_SUFFIX = "_download";
const string Parameters::DEFAULT_DOWNLOADED_EXTENSION = ".txt";
const string Parameters::DEFAULT_DOWNLOAD_PATH = "Download/";
const std::string Parameters::DOWNLOAD_URL_1 = "http://rest.ensembl.org/sequence/id/";	//part of the URL before the ensemble name
const std::string Parameters::DOWNLOAD_URL_2 = "?content-type=text/x-fasta;mask_feature=1;type=cdna;multiple_sequences=1";	//part of the URL after the ensemble name

const string Parameters::CONST_SHUFFLE_SUFFIX="_shuffle";

const int	Parameters::const_igapPenalty1 = -32000;	//Mysterious penalty value
const int	Parameters::const_igapPenalty2 = -4000;	//Mysterious penalty value
const long	Parameters::const_lMaxDistance = 200;	//The maximum distance between two targets (for "double")
const double Parameters::DEFAULT_K_VALUE = 1.0;

const char Parameters::CONST_DEFAULT_SIGN='+';	//The default sign that should be used when a header don't specify the "strand"

const unsigned long Parameters::DEFAULT_NB_FIRST = 10;

//const double Parameters::const_dIdealScore2 = 220;	//The default ideal score
const double Parameters::const_dIdealScore2 = 261.975;	//The default ideal score
