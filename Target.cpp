#include "target.h"

using namespace std;



bool operator==(Gene_Sequence const& a, Gene_Sequence const& b)
{
    return (a.m_sChrNumber==b.m_sChrNumber&&a.m_sGroupNumber==b.m_sGroupNumber&&*a.m_sSequence==*b.m_sSequence&&
            a.m_sSign==b.m_sSign&&a.m_sStartPosition==b.m_sStartPosition);
}


Target::Target() :	m_oMiRNA(), 
					//m_sSequence(0),
					m_lPositionStart(0),
					m_lPositionEnd(0),
					m_sRes1_long(),
					m_sRes2_long(),
					m_sRes3_long()
{
	m_oGene.m_sChrNumber = "";
	m_oGene.m_sGroupNumber = "";
	m_oGene.m_sSequence = 0;
    m_oGene.m_sSign = Settings::getSettings()->getFind_defaultSign();
	m_oGene.m_sStartPosition = "";
}

Target::Target(Target const& _copy) :	m_oMiRNA(_copy.m_oMiRNA),
										m_oGene(_copy.m_oGene),	//copying the value of the pointer (and not the pointed value)
										m_lPositionStart(_copy.m_lPositionStart),
										m_lPositionEnd(_copy.m_lPositionEnd),
										m_sRes1_long(_copy.m_sRes1_long),
										m_sRes2_long(_copy.m_sRes2_long),
										m_sRes3_long(_copy.m_sRes3_long),
										m_dScore(_copy.m_dScore)
{}

//Target::Target(miRNA_data * p_oMiRNA, std::string * p_sSequence) : 
//					m_oMiRNA(p_oMiRNA),
//					m_sSequence(p_sSequence),
//					m_lPositionStart(0),
//					m_lPositionEnd(0),
//					m_sRes1_long(),
//					m_sRes2_long(),
//					m_sRes3_long()
//{}

Target::~Target()
{
}

bool Target::estEgal(Target const& b) const
{
    return (m_oMiRNA == b.m_oMiRNA && m_oGene == b.m_oGene && m_lPositionStart == b.m_lPositionStart &&
            m_lPositionEnd == b.m_lPositionEnd && m_sRes1_long == b.m_sRes1_long && m_sRes2_long == b.m_sRes2_long &&
            m_sRes3_long == b.m_sRes3_long && m_dScore==b.m_dScore);
}

bool operator==(Target const& a, Target const& b)
{
    return a.estEgal(b);
}

bool operator!=(Target const& a, Target const& b)
{
    return !(a==b);
}

miRNA_data * Target::getMiRNA()
{
	return m_oMiRNA;
}

long Target::getPositionStart() const
{
	return m_lPositionStart;
}

long Target::getPositionEnd() const
{
	return m_lPositionEnd;
}

void Target::setMiRNA(miRNA_data * p_oMiRNA)
{
	m_oMiRNA = p_oMiRNA;
}

string Target::getRes1_long() const
{
	return m_sRes1_long;
}

string Target::getRes2_long() const
{
	return m_sRes2_long;
}

string Target::getRes3_long() const
{
	return m_sRes3_long;
}

double Target::getScore() const
{
    return m_dScore;
}

double Target::getScoreUnderOne() const
{
    return m_dScore/Settings::getSettings()->getFind_maximumScore();
}

void Target::setRes1_long(string const& p_sRes1)
{
	m_sRes1_long = p_sRes1;
}

void Target::setRes2_long(string const& p_sRes2)
{
	m_sRes2_long = p_sRes2;
}

void Target::setRes3_long(string const& p_sRes3)
{
	m_sRes3_long = p_sRes3;
}

void Target::setPositionStart(long p_lPositionStart)
{
	m_lPositionStart = p_lPositionStart;
}

void Target::setPositionEnd(long p_lPositionEnd)
{
	m_lPositionEnd = p_lPositionEnd;
}

void Target::setScore(double p_dScore)
{
	m_dScore = p_dScore;
}

void Target::display(ostream & flux) const
{
    /*m_vTargets[i].getGene().m_sGroupNumber;
    m_vTargets[i].getMiRNA()->name;
   m_vTargets[i].getPositionStart();
    m_vTargets[i].getPositionEnd();
    m_vTargets[i].getNbMatchInFirsts();
    m_vTargets[i].getScoreUnderOne();*/
    flux << m_oGene.m_sGroupNumber << "," << m_oMiRNA->name << "," << m_lPositionStart << "," << m_lPositionEnd
            << "," << getNbMatchInFirsts() << "," << getScoreUnderOne() << "," << m_dScore;
    flux<<","<<m_sRes1_long<<","<<m_sRes2_long<<","<<m_sRes3_long;
    /*cout << endl;
	cout << "miRNA_data m_oMiRNA = " <<endl<< *m_oMiRNA << endl;
	//cout << "string m_sSequence = " << m_sSequence << endl;
	cout << "long m_lPosition_Start = " << m_lPositionStart << endl;
	cout << "long m_lPosition_End = " << m_lPositionEnd << endl;
	cout << "string m_sRes1_long = " << m_sRes1_long << endl;
	cout << "string m_sRes3_long =    " << m_sRes3_long << endl;
	cout << "string m_sRes2_long = " << m_sRes2_long << endl;
	cout << "double m_dScore = " << m_dScore << endl;
    cout << "long nbMatchInFirst = " << getNbMatchInFirsts() << endl;*/
}

ostream& operator<<(ostream &flux, miRNA_data const& data)
{
	//Affichage des attributs
	flux << "\tName :\t" << data.name << endl;
	flux << "\tmiRNA :\t" << data.miRNA << endl;
	flux << "\tNumber :\t" << data.number << endl;
	flux << "\tPairs :\t" << data.pairs;
	return flux;
}


ostream& operator<<(ostream &flux, Target const& data)
{
    data.display(flux);
    return flux;
}


unsigned long Target::getNbMatchInFirsts(unsigned long nbFirst) const
{
	unsigned long res(0);
	for (unsigned int i = 0; i < nbFirst&&i < m_sRes3_long.length(); i++)
	{
		if (m_sRes3_long[i] == '|')
			res++;
	}
	return res;
}


Gene_Sequence Target::getGene()
{
	return m_oGene;
}

void Target::setGene(Gene_Sequence const& p_oGene)
{
	m_oGene = p_oGene;
}

vector<Target> Target::getTargets(vector<Target> p_vTargets,double p_dCutoff)
{
    vector<Target> temp;
    for(unsigned int i=0;i<p_vTargets.size();i++)
    {
        if(p_vTargets[i].getScoreUnderOne()>=p_dCutoff)
        {
            temp.push_back(p_vTargets[i]);
        }
    }
    return temp;
}
