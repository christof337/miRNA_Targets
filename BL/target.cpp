#include "target.h"

using namespace std;

Target::Target() :	m_oMiRNA(), 
					m_sSequence(0),
					m_lPositionStart(0),
					m_lPositionEnd(0),
					m_sRes1_long(),
					m_sRes2_long(),
					m_sRes3_long()
{
}

Target::Target(miRNA_data const& p_oMiRNA, std::string & p_sSequence) : 
					m_oMiRNA(p_oMiRNA),
					m_sSequence(&p_sSequence),
					m_lPositionStart(0),
					m_lPositionEnd(0),
					m_sRes1_long(),
					m_sRes2_long(),
					m_sRes3_long()
{

}


miRNA_data::miRNA_data() : 
					name(),
					miRNA(),
					number(0),
					pairs(0)
{

}


miRNA_data::miRNA_data(miRNA_data const& toCopy) :
					name(toCopy.name),
					miRNA(toCopy.miRNA),
					number(toCopy.number),
					pairs(toCopy.pairs)
{

}


void miRNA_data::incNumber(long toAdd)
{
	number += toAdd;
}

void miRNA_data::incPairs(long toAdd)
{
	pairs += toAdd;
}

void miRNA_data::clear()
{
	name = "";
	miRNA = "";
	number = 0;
	pairs = 0;
}

string miRNA_data::getName() const
{
	return name;
}


Target::~Target()
{
}


miRNA_data Target::getMiRNA() const
{
	return m_oMiRNA;
}


string Target::getSequence() const
{
	return *m_sSequence;
}

long Target::getPositionStart() const
{
	return m_lPositionStart;
}

long Target::getPositionEnd() const
{
	return m_lPositionEnd;
}

void Target::setMiRNA(miRNA_data const& p_oMiRNA)
{
	m_oMiRNA = p_oMiRNA;
}

void Target::setSequence(std::string * p_sSequence)
{
	m_sSequence = p_sSequence;
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

long Target::getPosition() const
{
	return m_lPosition;
}

double Target::getScore() const
{
	return m_dScore;
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

void Target::setPosition(long p_lPosition)
{
	m_lPosition = p_lPosition;
}

void Target::setScore(double p_dScore)
{
	m_dScore = p_dScore;
}

void Target::display() const
{
	cout << endl;
	cout << "miRNA_data m_oMiRNA = " << m_oMiRNA << endl;
	//cout << "string m_sSequence = " << m_sSequence << endl;
	cout << "long m_lPosition_Start = " << m_lPositionStart << endl;
	cout << "long m_lPosition_End = " << m_lPositionEnd << endl;
	cout << "string m_sRes1_long = " << m_sRes1_long << endl;
	cout << "string m_sRes3_long = " << m_sRes3_long << endl;
	cout << "string m_sRes2_long = " << m_sRes2_long << endl;
	cout << "long m_lPosition = " << m_lPosition << endl;
	cout << "double m_dScore = " << m_dScore << endl;
	//Sleep(200);
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