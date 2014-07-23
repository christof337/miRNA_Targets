#ifndef TARGET_H
#define TARGET_H

#include <string>
#include <iostream>
#include <ostream>
#include <Windows.h>

class miRNA_data
{
public:
	miRNA_data();
	miRNA_data(miRNA_data const& toCopy);
	void clear();
//private:
	std::string name;
	std::string miRNA;
	long number;
	long pairs;

	void incNumber(long toAdd = 1);
	void incPairs(long toAdd = 1);
	std::string getName() const;
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
	Target(miRNA_data const& p_oMiRNA, std::string & p_sSequence);
	~Target();

private:
	miRNA_data m_oMiRNA;
	std::string * m_sSequence;	//too heavy, must be deleted
	long m_lPositionStart;
	long m_lPositionEnd;
	std::string m_sRes1_long;
	std::string m_sRes2_long;
	std::string m_sRes3_long;
	long m_lPosition;
	double m_dScore;
public:
	miRNA_data getMiRNA() const;
	std::string getSequence() const;
	std::string getRes1_long() const;
	std::string getRes2_long() const;
	std::string getRes3_long() const;
	long getPositionStart() const;
	long getPositionEnd() const;
	long getPosition() const;
	double getScore() const;
	void setMiRNA(miRNA_data const& p_oMiRNA);
	void setSequence(std::string * p_sSequence);
	void incNumber();
	void setRes1_long(std::string const& p_sRes1);
	void setRes2_long(std::string const& p_sRes2);
	void setRes3_long(std::string const& p_sRes3);
	void setPositionStart(long p_iPositionStart);
	void setPositionEnd(long p_iPositionEnd);
	void setPosition(long p_lPosition);
	void setScore(double p_dScore);

	void display() const;
};

#endif	//	TARGET_H
