#ifndef DOWNLOAD_H
#define DOWNLOAD_H

/*!
* \file download.h
* \brief Download the previously imported ensemble list.
* \author Christophe Pont
* \version 0.1
*/

// Download To File
#include <UrlMon.h>

// Link The UrlMon Static Library
#pragma comment(lib, "UrlMon.lib")

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "parameters.h"
#include "utils.h"
#include "format.h"

//http://rest.ensembl.org/sequence/id/ENSG00000134516?content-type=text/x-fasta;mask_feature=1;type=cdna;multiple_sequences=1
//#define DOWNLOAD_URL_TEST "http://rest.ensembl.org/sequence/id/ENSG00000134516?content-type=text/x-fasta;mask_feature=1;type=cdna;multiple_sequences=1"

const std::string DOWNLOAD_URL_1 = "http://rest.ensembl.org/sequence/id/";
const std::string DOWNLOAD_URL_2 = "?content-type=text/x-fasta;mask_feature=1;type=cdna;multiple_sequences=1";

const std::string TEMPORARY_DOWNLOAD_FILE_NAME = "Download\\downloaded.txt";

const std::string ERROR_OUT_OF_MEMORY = "Out of memory";
const std::string ERROR_DOWNLOAD_FAILURE = "Download fail";
const std::string ERROR_APPEND_FAILURE = "Impossible to append the file.";
const std::string ERROR_APPEND_FAILURE_MESSAGE = "Fail to create one file for all the downloads.";
const std::string ERROR_UNKNOW = "Unknow exception.";

/*! \class Download
* \brief Class representing the downloading of the ensembles.
*
*  This class handle the downloading (mainly with static methods) of the imported ensemble list.
*/
class Download
{
public:
	/*!
	*  \brief Download the list of ensembles from an excel file.
	*
	*  This static method read from the list of ensembles of the input vector,
	* and download all these ensembles.
	*
	*  \param ensList : The vector containing the list of the names of the ensembles.
	*  \return if no errors has happened during the downloading, an empty vector.
	*	 if errors has occured, a vector containing :
	*	-first element : list of the ensembles which have thrown a "OUT OF MEMORY" exception during downloading.
	*   -second element : list of the ensembles which have thrown a "DOWNLOAD FAILURE" exception during downloading.
	*/
	static std::vector<std::vector<std::string>> downloadEnsembles(std::vector<std::string> const& ensList);
private:
	static void downloadFile(std::string const& url, std::string const& dest, bool shouldAppend=false);
	static void appendFile(std::string const& nameFileIn, std::ofstream & nameFileOut);
};

#endif // DOWNLOAD_H

