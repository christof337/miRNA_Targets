#include "shuffle.h"

using namespace std;

const unsigned int Shuffle::CONST_NUMBER_SHUFFLE = 10;

string Shuffle::shuffleString(string & mot)
{
   string melange(mot);
   int position(0);
   for(unsigned int i=0;i<CONST_NUMBER_SHUFFLE;i++)
   {
       mot=melange;
       melange="";
       //Tant qu'on n'a pas extrait toutes les lettres du mot
       while (mot.size() != 0)
       {
           //On choisit un numéro de lettre au hasard dans le mot
           position = rand() % mot.size();
           //On ajoute la lettre dans le mot mélangé
           melange += mot[position];
           //On retire cette lettre du mot mystère
           //Pour ne pas la prendre une deuxième fois
           mot.erase(position, 1);
       }
   }

   //On renvoie le mot mélangé
   return melange;
}

int Shuffle::mainShuffling(string const& _fileName){
    srand(time(0));
    ifstream fileIn(_fileName.c_str());
    ofstream fileOut(PathHandler::getShuffledFileName(_fileName).c_str());
   // cerr<<path.c_str();

    //fileIn.open(path.c_str(),fstream::in);
    if(!fileIn){
        fileIn.clear();
        return -1;
    }
    else{
         string line;
         string sequence;
         unsigned int cpt(0);
         /*read file*/
         while (!fileIn.eof()){
             /*one sequence*/
             getline(fileIn,line);
             if(line[0]=='>'){
                 if(!sequence.empty()){
                     //writing seq in the output file
                     fileOut << shuffleString(sequence) <<endl;
                     sequence="";
                 }
                 //writing line in the output file
                 fileOut << line << endl;
                 //utils::splitString(line,' ')[0];
                 cpt++;
             }
             /*read one sequence*/
             else if(line.empty())
             {
             }else
             {
                 /*concatenation*/
                 for(unsigned int j=0;j<line.size()-1;j++){
                      sequence=sequence+line[j];
                 }
             }
         }

         if(!sequence.empty()){
             //writing seq in the output file
             fileOut << shuffleString(sequence);
         }
        fileIn.close();
        return 0;
    }
}
