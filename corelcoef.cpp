#include "corelcoef.h"

using namespace std;

const double CorelCoef::CONST_DEFAULT_WRONG_CC_VALUE=-1;

double CorelCoef::getCorelCoef(unsigned int _TP, unsigned int _TN, unsigned int _FP, unsigned int _FN)
{
    double corelCoef;
    double TP(_TP),TN(_TN),FP(_FP),FN(_FN);
    if (((TP + FN)*(TN + FP)*(TP + FP)*(TN + FN)) > 0)
    {
        corelCoef = ((TP*TN) - (FN*FP)) / (sqrt((TP + FN)*(TN + FP)*(TP + FP)*(TN + FN)));
    }else
    {
        corelCoef=CONST_DEFAULT_WRONG_CC_VALUE;
    }
    return corelCoef;
}

unsigned int CorelCoef::getNumberOfSequencesWithTargets(vector<Target> const& p_vVector)
{
    vector<string*> found;
    Target t;
    for(unsigned int i=0;i<p_vVector.size();i++)
    {
        t=p_vVector[i];
        if(!utils::contain<string*>(found,t.getGene().m_sSequence))
        {
            found.push_back(t.getGene().m_sSequence);
        }
    }
    return (unsigned int)found.size();
}

double CorelCoef::getCC(double p_dCutoff, vector<Target> p_vTargets, vector<Target> p_vTargetsShuffle, unsigned int p_iNbSequences)
{
    return CorelCoef::getCCData(p_dCutoff,p_vTargets,p_vTargetsShuffle,p_iNbSequences).CC();
}

CorelCoef CorelCoef::getCCData(double p_dCutoff, std::vector<Target> p_vTargets, std::vector<Target> p_vTargetsShuffle, unsigned int p_iNbSequences)
{
    CorelCoef res;
    vector<Target> vectorCutoff=Target::getTargets(p_vTargets,p_dCutoff);
    vector<Target> vectorShuffleCutoff=Target::getTargets(p_vTargetsShuffle,p_dCutoff);

    res.m_iTP = CorelCoef::getNumberOfSequencesWithTargets(vectorCutoff);
    res.m_iFP = CorelCoef::getNumberOfSequencesWithTargets(vectorShuffleCutoff);
    res.m_iFN = p_iNbSequences - res.m_iTP;
    res.m_iTN = p_iNbSequences - res.m_iFP;
    res.m_dCC= CorelCoef::getCorelCoef(res.m_iTP,res.m_iTN,res.m_iFP,res.m_iFN);
    return res;
}

unsigned int CorelCoef::TP() const
{
    return m_iTP;
}

unsigned int CorelCoef::TN() const
{
    return m_iTN;
}

unsigned int CorelCoef::FP() const
{
    return m_iFP;
}
unsigned int CorelCoef::FN() const
{
    return m_iFN;
}

double CorelCoef::CC() const
{
    return m_dCC;
}
