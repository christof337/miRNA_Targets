#include "load.h"

using namespace std;

vector<string> Load::loadAFile(string const& path, bool const shouldWriteInAFile, int const column)
{
	ifstream input(path);	/* Input file to be loaded. */
	vector<string> res;		/* Output of this function. */
	string outputFileName(utils::appendTextBeforeExtension(path, OUTPUT_SUFFIX)); /* Name of the output file. */
	outputFileName = outputFileName.substr(0, outputFileName.length() - utils::getExtension(outputFileName).length()-1); //remove the old extension
	outputFileName += OUTPUT_EXTENSION; //add the new extension
	ofstream output(outputFileName);	/* Output file to be written in if shouldWriteInAFile is true. */
    string str,ens;
	int found(0);			/* Used to get the position of the different delimiting characters. */
	unsigned int found2(0); /* Used to remember the old position of "found", in order to deal with parts of the string. */
	int cpt(0);				/* Used to get to the corresponding column.*/
	bool isEnoughColumns(false); /* Used to know if there is enough columns in the given file.*/
	bool isEnsemblesInFile(false); /* Used to know if there is ensembles in the given file at the given column.*/


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
		throw string(ERROR_MESSAGE_FILE_DO_NOT_EXISTS);
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
			string tmp(SUPPORTED_EXTENSION);
			throw string(ERROR_MESSAGE_WRONG_FORMAT_1 + tmp + ERROR_MESSAGE_WRONG_FORMAT_2);
        }else
        {
			//good format
            //read from the file
            while(getline(input,str))
            {
				cpt = 0;
				found = 0;

                //selecting the "column"th column
                do
                {
                    found=str.find(SPLIT_CHAR,found+1);
					//checking if there is any \" in the line (meaning that the current comma in it should be ignored)
					if (!Load::isBetweenQuotes(found, str))
					{
						cpt++;
					}
                }while(found!=string::npos&&cpt<column);
                if(found!=string::npos)
                {
					//the file has the correct amount of columns.
					isEnoughColumns = true;
					//getting just the column needed
                    str=str.substr(found+1);	//getting the column and the end of the line
                    found=str.find(SPLIT_CHAR); //finding the next delimiting characted
                    str=str.substr(0,found);	//getting only the space between the column and the next delimiting character
                    //here, str contains the good column
                    found=0;
					//the column should be like "ENSEMBL;ENSG00000142937;ortholog_one2many;% identity 69;ENSG00000214310;ortholog_one2many;% identity 53;"
					//we just need the ENSG00000......
                    do
                    {
                        found2=found;	//saving the last position of found
                        found=str.find(SPLIT_CHAR_IN_COLUMN,found+1);
						if (found != string::npos)
						{
							ens=str.substr(found2+1,found-found2-1);
						}
						else if (found2 + 1>str.length())
						{
							//chaine vide
							ens = "";
						}else
						{
							ens = str.substr(found2 + 1);//end of line
						}
						//here we have the data between the ";"
                        if(ens.find(NORMAL_PREFIX)!=string::npos)
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
                    }while(found!=string::npos);
                }
            }
			if (!isEnoughColumns)	//if not enough column in the file (wrong format)
			{
				str = ERROR_MESSAGE_NOT_ENOUGH_COLUMNS_1 + path + ERROR_MESSAGE_NOT_ENOUGH_COLUMNS_2 + utils::convertInt(column) + ERROR_MESSAGE_NOT_ENOUGH_COLUMNS_3;
				throw string(str);
			}
			if (!isEnsemblesInFile)//if no ensemble found (wrong column or file)
			{
				str = ERROR_MESSAGE_NO_ENSEMBLE_FOUND_1 + utils::convertInt(column) + ERROR_MESSAGE_NO_ENSEMBLE_FOUND_2 + path;
				throw string(str);
			}
        }
    }
	
    return res;
}


bool Load::isBetweenQuotes(int const position, string const& str)
{
	int found(0),found2(0);
	bool isBeetween(true); /* flag raised when the LAST part of the string beetween 2 quotes (or the beggining of the string) IS beetween accolades
						   isBetween HAS to be initializied at "true".
						   When the loop is exited, isBeetween has changed once more than it should.
						   So by inverting the "natural" first value of isBetween, the order is settled again.
						   Try this algorithm to verify :
						   ------------------------
						   string str1 = "012\"456\"89"; //the string is : 012"456"89
														//456 should return true, the other, false
						   cout << str1 << endl;
						   for (int i = 0; i < 10; i++)
						   {
								if (Load::isBetweenQuotes(i, str1))
								{
									cout << i << " : " << "true" << endl;
								}
								else
								{
									cout << i << " : " << "false" << endl;
								 }
						   }
						   ------------------------
						   */
	while (found != string::npos && !(position>found2 && position<found))
		//while the string isn't finished and the position hasn't been found yet... (==between the last found " and the next)
	{
		found2 = found;
		found = str.find("\"", found + 1);
		isBeetween = !isBeetween;
	}
	return isBeetween;
}