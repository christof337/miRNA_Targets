#ifndef EXPORT_H
#define EXPORT_H

#include <string>
#include <vector>
#include <fstream>

#include "Target.h"
#include "utils.h"
#include "corelcoef.h"
#include "settings.h"

class Export
{
public:
    static bool exportCCData(std::string const& p_sFileName, std::vector<Target> const& p_vTargets, std::vector<Target> const& p_vTargetShuffle, unsigned int p_iNbSequences);
    static bool exportTargets(std::string const& p_sFileName, std::vector<Target> const& p_vTargets, std::vector<Target> const& p_vTargetsShuffle, unsigned int p_iNbSequences, double p_dCutoff, bool m_bShouldDisplayCC);
};

#endif // EXPORT_H
