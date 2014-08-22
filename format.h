#ifndef FORMAT_H
#define FORMAT_H

#include <fstream>
#include <string>

#include "settings.h"
#include "utils.h"

/*! \class Format
*	\brief Class doing the formatting of the input sequence FASTA format files.
* 
*	This class handle the formatting of the input sequence FASTA file.
*	Note : useless class since the program don't need a specific header to work properly.
*/
class Format
{
public:
    /*!
	*	\brief Format the file of the given name.
	*
	*	This method format the given file.
	*
	*	\param fileName : name of the file that should be formatted.
    */
	static int formatFile(std::string const& fileName);
private:
    /*!
	*	\brief Format the given inputFile into the given output file.
	*
	*	fileIn must be opened in reading mode, fileOut must be opened in writing mode.
	*
	*	\param fileIn : the ifstream opened in reading that should be formatted
	*	\param fileOut : the ofstream opened in writing that should receive the formatted file
    */
	static int formatFileInIntoFileOut(std::ifstream & fileIn, std::ofstream & fileOut);

	static const std::string CONST_SUFFIX_FILE_OUT;			//the default suffix of the output file
    //const std::string DEFAULT_HEADER_LINE = " range=chr19:44430527-44430726 5'pad=100 3'pad=100 strand=+ repeatMasking=none";
	static const std::string DEFAULT_HEADER_LINE;			//the default header line (only if needed)
    //static const std::string PATTERN_FILTER;				//the pattern filter (only the header containing it should be taken). Not used in fact.
};

#endif	// FORMAT_H
