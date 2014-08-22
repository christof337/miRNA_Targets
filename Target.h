#ifndef TARGET_H
#define TARGET_H

#include <string>
#include <ostream>
#include <vector>

#include "settings.h"

struct Gene_Sequence
{
	std::string m_sGroupNumber;
	std::string m_sChrNumber;
	std::string m_sStartPosition;
	std::string * m_sSequence;
	char m_sSign;
};

bool operator==(Gene_Sequence const& a, Gene_Sequence const& b);

struct miRNA_data
{
	std::string name;
	std::string miRNA;
	long number;
	long pairs;
};

std::ostream& operator<<(std::ostream &flux, miRNA_data const& data);



/*! \class Target
* \brief This class records the information about a found target between a sequence and a miRNA.
*
*  This class records information such as the start position of the binding site, its end position, its score and so on.
*/
class Target
{
public:
	Target();
	Target(Target const& _copy);
	//Target(miRNA_data * p_oMiRNA, std::string * p_sSequence);
	~Target();
    static std::vector<Target> getTargets(std::vector<Target> p_vTargets, double p_dCutoff);
private:
	miRNA_data * m_oMiRNA;
	long m_lPositionStart;
	long m_lPositionEnd;
	//long m_lSeqPositionStart;
	//long m_lmiRnaPosition
	std::string m_sRes1_long;
	std::string m_sRes2_long;
	std::string m_sRes3_long;
	double m_dScore;
	Gene_Sequence m_oGene;	
public:
	miRNA_data * getMiRNA();
	std::string getRes1_long() const;
	std::string getRes2_long() const;
	std::string getRes3_long() const;
	long getPositionStart() const;
    long getPositionEnd() const;
    double getScore() const;
    double getScoreUnderOne() const;
	Gene_Sequence getGene();
    unsigned long getNbMatchInFirsts(unsigned long nbFirst = Settings::getSettings()->getResults_nbFirsts()) const;
	void setMiRNA(miRNA_data * p_oMiRNA);
	void incNumber();
	void setRes1_long(std::string const& p_sRes1);
	void setRes2_long(std::string const& p_sRes2);
	void setRes3_long(std::string const& p_sRes3);
	void setPositionStart(long p_iPositionStart);
	void setPositionEnd(long p_iPositionEnd);
	void setScore(double p_dScore);
	void setGene(Gene_Sequence const& p_oGene);

    void display(std::ostream & flux) const;

    bool estEgal(Target const& b) const;
};

bool operator==(Target const& a, Target const& b);
bool operator!=(Target const& a, Target const& b);
std::ostream& operator<<(std::ostream &flux, Target const& data);

#endif	//	TARGET_H
