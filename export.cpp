#include "export.h"

using namespace std;

bool Export::exportCCData(string const& p_sFileName, vector<Target> const& p_vTargets, vector<Target> const& p_vTargetShuffle, unsigned int p_iNbSequences)
{
    bool res(false);
    CorelCoef CCData;
    ofstream fileOut(p_sFileName.substr(0,p_sFileName.find(utils::getExtension(p_sFileName)))+"_CCdata.csv");
    if(fileOut)
    {
        fileOut << "cutoff, CC, TP (number of sequences with target), TN (number of shuffled sequences with target), FP, FN" << endl;
        for(double i=Settings::getSettings()->getResults_minimumCutoff();i<=Settings::getSettings()->getResults_maximumCutoff();i+=Settings::getSettings()->getResults_stepCutoff())
        {
            CCData=CorelCoef::getCCData(i, p_vTargets, p_vTargetShuffle, p_iNbSequences);
            fileOut << i<<",";
            if(CCData.CC()==CorelCoef::CONST_DEFAULT_WRONG_CC_VALUE)
            {
                fileOut<< "-";
            }else
            {
                fileOut << CCData.CC();
            }
            fileOut<<","<<CCData.TP()<<","<<CCData.TN()<<","<<CCData.FP()<<","<<CCData.FN()<<endl;
        }
        fileOut<<endl<<"Number of sequences,Total number of found targets"<<endl; // headers
        fileOut << p_iNbSequences << "," << p_vTargets.size();
        res=true;
    }else
    {
        res=false;
    }
    return res;
}

bool Export::exportTargets(string const& p_sFileName, vector<Target> const& p_vTargets, vector<Target> const& p_vTargetsShuffle, unsigned int p_iNbSequences, double p_dCutoff, bool m_bShouldDisplayCC)
{
    bool res(false);
    //m_vTargets
    //selected cutoff
    //m_vTargetsShuffle
    //fileName
    //
    ofstream fileOut(p_sFileName);

    if(fileOut)
    {
        vector<Target> vectorCutoff=Target::getTargets(p_vTargets,p_dCutoff);
        vector<Target> vectorShuffleCutoff=Target::getTargets(p_vTargetsShuffle,p_dCutoff);

        //headers
        fileOut<< "Gene name, MiRNA name, Position start, Position end, Nb Match In Firsts "<<Settings::getSettings()->getResults_nbFirsts()<<", Score under one, Score" << endl;
        for(unsigned int i= 0;i<vectorCutoff.size();i++)
        {
            fileOut << vectorCutoff[i] << endl;
        }
        //write common informations in the end
        //------------------------------------------------------------------------
        unsigned int TP, FP, FN, TN;
        fileOut<<endl<<"Cutoff,Number of sequences,"<<"Number of found targets";// headers
        if(m_bShouldDisplayCC)
        {
            fileOut<<",TP,"<<"FP,"<<"FN,"<<"TN,"<<"CC"; // headers
        }
        fileOut << endl;
        fileOut << p_dCutoff << ",";
        //Col 0 : Number of sequences
        fileOut << p_iNbSequences << "," ;
        //Col 1 : Total number of found targets
        fileOut << vectorCutoff.size()<< "," ;

        if(m_bShouldDisplayCC)
        {
            //Col 2 : TP
            TP = CorelCoef::getNumberOfSequencesWithTargets(vectorCutoff);
            //Col 3 : FP
            FP = CorelCoef::getNumberOfSequencesWithTargets(vectorShuffleCutoff);
            //Col 4 : FN
            FN = p_iNbSequences - TP;
            //Col 5 : TN
            TN = p_iNbSequences - FP;
            fileOut << TP << "," << FP << ","  << FN<< ","  <<TN<< ",";
            //Col 6 : CC
            fileOut << CorelCoef::getCorelCoef(TP,TN,FP,FN);
            //------------------------------------------------------------------------
        }
        res=true;
    }else
    {
        res=false;
    }
    return res;
}
