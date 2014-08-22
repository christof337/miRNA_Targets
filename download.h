#ifndef DOWNLOAD_H
#define DOWNLOAD_H

// Download To File
#include <UrlMon.h>

// Link The UrlMon Static Library
#pragma comment(lib, "UrlMon.lib")

#include <fstream>
#include <string>
#include <vector>

#include "format.h"
#include "pathhandler.h"

//The kind of URL that interests us is : http://rest.ensembl.org/sequence/id/ENSG00000134516?content-type=text/x-fasta;mask_feature=1;type=cdna;multiple_sequences=1

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
	*  and download all these ensembles.
	*
	*  \param ensList : The vector containing the list of the names of the ensembles.
	*  \param _fileName : The name of the file in which these ensembles have been retrieved ; useful to create the name of the output file.
	*  \return if no errors has happened during the downloading, an empty vector.
	*	 if errors has occured, a vector containing :
	*	-first element : list of the ensembles which have thrown a "OUT OF MEMORY" exception during downloading.
	*   -second element : list of the ensembles which have thrown a "DOWNLOAD FAILURE" exception during downloading.
	*/
	static std::vector<std::vector<std::string>> downloadEnsembles(std::vector<std::string> const& ensList, std::string const& _fileName);
private:
	/*!
    *  \brief Download the file of the given url and put its content in a file named dest.
	*
	*  This static method download the file at the specified URL (using URLDownloadToFileA) and write its whole content in the file named dest.
	*
	*  \param url : the url of the input web file
	*  \param dest : the path of the output file.
	*/
	static void downloadFile(std::string const& url, std::string const& dest);

	/*!
	* \brief Append the given input file at the end of the given output file.
	*
	*  This static method add all the content of the file whose name is nameFileIn and add it in the end of the file nameFileOut.
	*  nameFileOut should be open in writing (appending mode).
	*  
	*  \param nameFileIn the name of the input file that should be opened
    *  \param fileOut the output ofstream which should be written in. <br/>
    *   Note : fileOut is given directly as a file (and not as a filename) in order to avoid the multiple reopening
    *   of the (large) downloaded file. There, we only onpen it once.
	*/
    static void appendFile(std::string const& nameFileIn, std::ofstream & fileOut);

    static const std::string TEMPORARY_DOWNLOAD_FILE_NAME ;	///name of the temporary downloaded file path

    static const std::string ERROR_OUT_OF_MEMORY ;          ///error for an out of memory error type
    static const std::string ERROR_DOWNLOAD_FAILURE;        ///error for a download failure error type
    static const std::string ERROR_APPEND_FAILURE ;         ///error for a output file impossible to append to
    static const std::string ERROR_APPEND_FAILURE_MESSAGE ;	///error message for a output file impossible to append to
    static const std::string ERROR_UNKNOW ;                 ///error message for an unknow exception
};

#endif // DOWNLOAD_H

