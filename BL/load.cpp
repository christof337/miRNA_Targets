#include "load.h"

using namespace std;

vector<string> Load::loadAFile(string const& path, bool const shouldWriteInAFile, unsigned int const column)
{
	ifstream input(path);	/* Input file to be loaded. */
	vector<string> res;		/* Output of this function. */
	string outputFileName(utils::appendTextBeforeExtension(path, OUTPUT_SUFFIX)); /* Name of the output file. */
	outputFileName = outputFileName.substr(0, outputFileName.length() - utils::getExtension(outputFileName).length() - 1) + OUTPUT_EXTENSION; //remove the old extension
	//add the new extension
	ofstream output(outputFileName);	/* Output file to be written in if shouldWriteInAFile is true. */
    string str,ens;
	bool isEnoughColumns(false); /* Used to know if there is enough columns in the given file.*/
	bool isEnsemblesInFile(false); /* Used to know if there is ensembles in the given file at the given column.*/
	vector<string> splitTab; /* Vector containing the different parts of one line or cell. */

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
		if (utils::getExtension(path) != SUPPORTED_EXTENSION)
        {
            //wrong format
			if (output)
			{
				output.close();
				remove(outputFileName.c_str());
			}
			throw ERROR_MESSAGE_WRONG_FORMAT_1 + SUPPORTED_EXTENSION + ERROR_MESSAGE_WRONG_FORMAT_2;
        }else
        {
			//good format
            //read from the file
            while(getline(input,str))
            {
				//splitting the line with the separator of the DEFAULT_EXTENSION
				splitTab = utils::splitString(str, SPLIT_CHAR,true);
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
					splitTab = utils::splitString(str, SPLIT_CHAR_IN_COLUMN);
					for (unsigned int i = 0; i<splitTab.size();i++)
                    {
						ens = splitTab[i];
						//here we have the data between the ";"
                        if(ens.find(NORMAL_PREFIX)<ens.length())
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
				str = ERROR_MESSAGE_NOT_ENOUGH_COLUMNS_1 + path + ERROR_MESSAGE_NOT_ENOUGH_COLUMNS_2 + utils::convertInt(column) + ERROR_MESSAGE_NOT_ENOUGH_COLUMNS_3;
				throw str;
			}
			if (!isEnsemblesInFile)//if no ensemble found (wrong column or file)
			{
				str = ERROR_MESSAGE_NO_ENSEMBLE_FOUND_1 + utils::convertInt(column) + ERROR_MESSAGE_NO_ENSEMBLE_FOUND_2 + path;
				throw str;
			}
        }
    }
	
    return res;
}