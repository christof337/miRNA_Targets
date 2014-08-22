#include "format.h"

using namespace std;

/* constants definitions */
const string Format::CONST_SUFFIX_FILE_OUT = "_format";		//the default suffix of the output file
//const std::string Format::DEFAULT_HEADER_LINE = " range=chr19:44430527-44430726 5'pad=100 3'pad=100 strand=+ repeatMasking=none";
const string Format::DEFAULT_HEADER_LINE = "";				//the default header line (only if needed)
//const string Format::PATTERN_FILTER = "utr3";				//the pattern filter (only the header containing it should be taken). Not used in fact.


/**
fileIn doit être ouvert en lecture et fileOut en écriture
*/
int Format::formatFileInIntoFileOut(ifstream & fileIn, ofstream & fileOut)
{
    bool flagIsHeader(false);
    char currentChar(0);

	if (fileIn&&fileOut)
	{
		while (fileIn.good())
		{
			currentChar = fileIn.get();	//read one character
			if (fileIn.good())
			{
                if (currentChar == Settings::getSettings()->getLoad_charBeginHeaderLine())
				{
					//begin of header
					fileOut << endl;
				}
				if (currentChar >= '0' && currentChar <= '9')
				{
					flagIsHeader = true;
				}
				else if (flagIsHeader)
				{
					//end of header
					fileOut << " " << DEFAULT_HEADER_LINE;// << endl;
					flagIsHeader = false;
				}
				fileOut << currentChar;
			}
		}

		return 0;
	}
	else
	{
		return -1;
	}
}

int Format::formatFile(string const& fileName)
{
	int res(-1);
	string nameFileOut(utils::appendTextBeforeExtension(fileName, CONST_SUFFIX_FILE_OUT));
	ifstream fileIn(fileName);
	ofstream fileOut(nameFileOut);

	if (fileIn&&fileOut)
	{
		//il ne nous reste plus qu'à écrire le contenu mis à jour du fichier d'entrée dans le fichier de sortie

		res = formatFileInIntoFileOut(fileIn, fileOut);

		//now we have to delete the original file and rename the new one
		fileIn.close();
		remove(fileName.c_str());
		fileOut.close();
		rename(nameFileOut.c_str(), fileName.c_str());
	}
	else
	{
		if (!fileIn)
		{
			throw string("Impossible to open the file " + fileName + ".\n");
		}
		if (!fileOut)
		{
			throw string("Impossible to create the file " + nameFileOut + ".\n");
		}
	}

	return res;
}
