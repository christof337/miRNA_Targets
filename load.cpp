#include "load.h"

using namespace std;

/* Constants definitions */
/*
\brief The extension that can be analyzed with the static method Load"()".
*/

//ERROR MESSAGES
const string Load::ERROR_MESSAGE_FILE_DO_NOT_EXISTS = "Error : input file doesn't exists.";		//error message of the absence of the input file
const string Load::ERROR_MESSAGE_WRONG_FORMAT_1 = "Error : thank you to convert the file into '.";	//error message of bad input format 1
const string Load::ERROR_MESSAGE_WRONG_FORMAT_2 = "'.";											//error message of bad input format 2
const string Load::ERROR_MESSAGE_NO_ENSEMBLE_FOUND_1 = "Error : No ensembles found in the given column (";			//error message of no ensemble found in the given file and column 1
const string Load::ERROR_MESSAGE_NO_ENSEMBLE_FOUND_2 = ") of the given file.\n";					//error message of no ensemble found in the given file and column 2
const string Load::ERROR_MESSAGE_NOT_ENOUGH_COLUMNS_1 = "Error : Not enough columns found in the given file (";	//error message of the lack of columns 1
const string Load::ERROR_MESSAGE_NOT_ENOUGH_COLUMNS_2 = ") to reach the specified column (";		//error message of the lack of columns 2
const string Load::ERROR_MESSAGE_NOT_ENOUGH_COLUMNS_3 = ").";										//error message of the lack of columns 3


vector<string> Load::loadAFile(string const& path, bool const shouldWriteInAFile, unsigned int const column)
{
	ifstream input(path.c_str());	/* Input file to be loaded. */
	vector<string> res;		/* Output of this function. */

    string outputFileName(utils::appendTextBeforeExtension(path,  Settings::getSettings()->getOOO_outputSuffix())); /* Name of the output file. */
    outputFileName = outputFileName.substr(0, outputFileName.length() - utils::getExtension(outputFileName).length() - 1) +  Settings::getSettings()->getOOO_outputExtension(); //remove the old extension
	//add the new extension
	ofstream output(outputFileName.c_str());	/* Output file to be written in if shouldWriteInAFile is true. */
    string str,ens;
    bool isEnoughColumns(false);    /* Used to know if there is enough columns in the given file.*/
	bool isEnsemblesInFile(false); /* Used to know if there is ensembles in the given file at the given column.*/
    vector<string> splitTab;        /* Vector containing the different parts of one line or cell. */

	if (shouldWriteInAFile)
	{
		//by convention, the only output if this function should write its results in a file is a vector containing the path of the output file.
		res.push_back(outputFileName);
	}
	else
	{
		//if this function shouldn't write in an output file, we erase the output file created sooner.
		output.close();
		remove(outputFileName.c_str());
	}

    //first : test if the file exists
    if(!input)
    {
        //wrong file
		if (output)
		{
			output.close();
			remove(outputFileName.c_str());
		}
		throw ERROR_MESSAGE_FILE_DO_NOT_EXISTS;
    }else
    {
        //the file exists
        if (utils::getExtension(path) != Settings::getSettings()->getExtract_supportedExtension())
        {
            //wrong format
			if (output)
			{
				output.close();
				remove(outputFileName.c_str());
			}
            throw ERROR_MESSAGE_WRONG_FORMAT_1 + Settings::getSettings()->getExtract_supportedExtension() + ERROR_MESSAGE_WRONG_FORMAT_2;
        }else
        {
			//good format
            //read from the file
            while(getline(input,str))
            {
				//splitting the line with the separator of the DEFAULT_EXTENSION

                splitTab = utils::splitString(str, Settings::getSettings()->getExtract_splitChar(),true);
                if(splitTab.size()>=column)
                {
					//the file has the correct amount of columns.
					isEnoughColumns = true;
					//getting just the column needed
					//selecting the "column"th column
					str = splitTab[column];	
                    //here, str contains the good column
					//the column should be like "ENSEMBL;ENSG00000142937;ortholog_one2many;% identity 69;ENSG00000214310;ortholog_one2many;% identity 53;"
                    //we just need the ENSG00000......
                    splitTab = utils::splitString(str, Settings::getSettings()->getExtract_splitCharInColumn());
					for (unsigned int i = 0; i<splitTab.size();i++)
                    {
						ens = splitTab[i];
                        //here we have the data between the ";"
                        if(ens.find(Settings::getSettings()->getExtract_normalEnsemblPrefix())<ens.length())
                        {	//if it starts with what we're looking for...
							isEnsemblesInFile = true;
                            //good string ==> writing it
                           if(shouldWriteInAFile)
                           {
                               output << ens << endl;
                           }else
                           {
                               res.push_back(ens);
                           }
                        }
                    }
                }
            }
			if (!isEnoughColumns)	//if not enough column in the file (wrong format)
			{
				str = ERROR_MESSAGE_NOT_ENOUGH_COLUMNS_1 + path + ERROR_MESSAGE_NOT_ENOUGH_COLUMNS_2 + utils::convertToString<int>(column) + ERROR_MESSAGE_NOT_ENOUGH_COLUMNS_3;
				throw str;
			}
			if (!isEnsemblesInFile)//if no ensemble found (wrong column or file)
			{
				str = ERROR_MESSAGE_NO_ENSEMBLE_FOUND_1 + utils::convertToString<int>(column) +ERROR_MESSAGE_NO_ENSEMBLE_FOUND_2 + path;
				throw str;
			}
        }
    }
	
    return res;
}
