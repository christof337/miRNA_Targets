#include "load.h"

using namespace std;

vector<string> Load::loadAFile(string const& path, int column, bool shouldWriteInAFile)
{

    ifstream input(path);
    vector<string> res;
    if(shouldWriteInAFile)
    {
        ofstream output(appendTextBeforeExtension(path,OUTPUT_SUFFIX));
        res.push_back(appendTextBeforeExtension(path,OUTPUT_SUFFIX));
    }
    string str,ens;
    int found(0),found2(0);
    int cpt(0);

    //first : test if the file exists
    if(!input)
    {
        //wrong file
        throw string("Erreur : impossible d'ouvrir le fichier en entree");
    }else
    {
        if(getExtension(path)!=SUPPORTED_EXTENSION)
        {
            //wrong format
            throw string("Erreur : merci de convertir le fichier en '."+SUPPORTED_EXTENSION+"'.");
        }else
        {
            //read from the file
            while(readline(input,str))
            {
                //selecting the "column"th column
                do
                {
                    found=str.find(SPLIT_CHAR,found+1);
                    cpt++;
                }while(found!=string::npos&&cpt<column);
                if(found!=string::npos)
                {
                    str=str.substr(found+1);
                    found=str.find(SPLIT_CHAR);
                    str=str.substr(0,found);
                    //here, str contains the good column
                    found=0;
                    do
                    {
                        found2=found;
                        found=str.find(SPLIT_CHAR_IN_COLUMN,found+1);
                        ens=str.substr(found2+1,found);
                        if(ens.find(NORMAL_PREFIX)!=string::npos)
                        {
                            //good string ==> writing it
`                           if(shouldWriteInAFile)
                            {
                                output << ens;
                            }else
                            {
                                res.push_back(ens);
                            }
                        }
                    }while(found!=string::npos);
                }
            }
        }
    }
    return res;
}
