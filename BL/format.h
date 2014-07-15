#ifndef FORMAT_H
#define FORMAT_H

#include <iostream>
#include <fstream>
#include <string>

#include "utils.h"
#include "parameters.h"

const std::string CONST_DEFAULT_FILE_NAME = "C:\\Users\\Public\\Documents\\zaki-pont\\miRNA\\Tests\\mix2\\mix_ensemble3";
const std::string CONST_DEFAULT_FILE_EXTENSION = ".txt";
const std::string CONST_SUFFIX_FILE_OUT = "_format";
//const std::string DEFAULT_HEADER_LINE = " range=chr19:44430527-44430726 5'pad=100 3'pad=100 strand=+ repeatMasking=none";
const std::string DEFAULT_HEADER_LINE = "";
const std::string PATTERN_FILTER = "utr3";

class Format
{
public:
	static int formatFileInIntoFileOut(std::ifstream & fileIn, std::ofstream & fileOut);

	static int formatFile(std::string fileName);
};

#endif	// FORMAT_H