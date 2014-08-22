#ifndef CORELCOEF_H
#define CORELCOEF_H

#include <vector>
#include "Target.h"
#include "utils.h"

/*!
 * @brief The CorelCoef class
 *
 * Handling the calcul of the correlation coefficient.
 */
class CorelCoef
{
public:
    static double getCorelCoef(unsigned int _TP, unsigned int _TN, unsigned int _FP, unsigned int _FN);
    static unsigned int getNumberOfSequencesWithTargets(std::vector<Target> const& p_vVector);

    static const double CONST_DEFAULT_WRONG_CC_VALUE;
    static double getCC(double p_dCutoff, std::vector<Target> p_vTargets, std::vector<Target> p_vTargetsShuffle, unsigned int p_iNbSequences);
    static CorelCoef getCCData(double p_dCutoff, std::vector<Target> p_vTargets, std::vector<Target> p_vTargetsShuffle, unsigned int p_iNbSequences);

    unsigned int TP() const;
    unsigned int TN() const;
    unsigned int FP() const;
    unsigned int FN() const;
    double CC() const;
private:
    unsigned int m_iTP;
    unsigned int m_iTN;
    unsigned int m_iFP;
    unsigned int m_iFN;
    double m_dCC;
};

#endif // CORELCOEF_H
