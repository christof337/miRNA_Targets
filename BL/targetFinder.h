#ifndef TARGETFINDER_H
#define TARGETFINDER_H

#include <string>
#include <fstream>
#include <vector>

#include "utils.h"
#include "parameters.h"

const std::string CONST_DEFAULT_SUFFIX = "_result";

enum class error_check_miRNA_seq {
	CORRECT_SEQ,  /* The miRNA sequence has a correct format. */
	ERROR_BAD_SEQ_FORMAT, /* The miRNA sequence has not a correct format (not only 'a','c','g','t','u' for example). */
	ERROR_BAD_SEQ_LENGTH /* The miRNA sequence has a bad length, too short or too long. */
};


const std::string ERROR_MESSAGE_BAD_LENGTH = "The length of miRNA should be between 17 and 28 bp. The input file has wrong format.";
const std::string ERROR_MESSAGE_BAD_SEQUENCE_FORMAT = "miRNA sequence should include only letters 'A', 'T', 'C', 'G', and 'U'.";
const std::string ERROR_MESSAGE_BAD_HEADER_FORMAT = "miRNA header should be of this type.\n><name><space> cdna:KNOWN_miRNA";
const std::string ERROR_MESSAGE_NO_HEADER = "No header found. FASTA format required.";
const std::string ERROR_MESSAGE_NO_OPEN_INPUT_FILE = "Impossible to open the input file.";

//formal parameters
const char CONST_DEFAULT_SIGN = '+';
const int const_igapPenalty1 = -32000;
const int const_igapPenalty2 = -4000;
const double const_dIdealScore2 = 220;
const long const_lMaxDistance = 200;
const double DEFAULT_K_VALUE = 1.0;
const double WEIGHT[27] = { 62.3, 94.1, 96.9, 97.4, 91.7, 90.3, 91.2, 54.3, 46.2, 49.0, 48.0, 46.8, 51.6, 46.5, 47.4, 45.7, 44.2, 43.7, 49.0, 44.0, 44.0, 44.0, 44.0, 44.0, 44.0, 44.0, 44.0 };
const double WEIGHT_GAPS[27] = { .0, .0, .0, .0, .0, 1.6, 3.3, 5.0, 20.0, 17.0, 7.5, 5.8, 6.7, 5.0, 5.0, 3.3, 8.3, 6.7, 4.2, 1.7, 1.6, .0, .0, .0, .0, .0, .0 };

class miRNA_data
{
public:
	miRNA_data();
	void clear();
//private:
	std::string name;
	std::string miRNA;
	long number;
	long pairs;
};

class target_data
{
public:
	target_data();
	std::string miRNA_name;
	long position;
	double score;
};

struct Gene_Sequence
{
	std::string m_sGroupNumber;
	std::string m_sChrNumber;
	std::string m_sStartPosition;
	std::string m_sSequence;
	char m_sSign;
};

class TargetFinder
{
public:
	TargetFinder();
	~TargetFinder();

	/*!
	*  \brief Fill the m_pMiRNA vector with the miRNA of the given file, and also set 
	*  m_iNumber_miRNA to the numbers of miRNA found.
	*
	*  This static method open the file of the given path, and extract the miRNA from it.
	*  The extracted sequences should have a specific format, including having a length 
	*  between DEFAULT_MINIMUM_MIRNA_LENGTH and DEFAULT_MAXIMUM_MIRNA_LENGTH. 
	*
	*  \param inputFileName : The path of the input file, containing the miRNA sequences (FASTA format).
	*  \return true if the operation was a success, false if not.
	*/
	bool getMiRNAs(std::string const& inputFileName);

	void loadSequenceFile(std::string const& inputFileName);

	void NormilizePositionWeight(int p_iLength);
	void NormilizePositionWeightGaps(int p_iLength);

	void findTarget(long p_lMiRNA_number, std::string & p_sUTRsequence, bool p_bStraitgh);

	/*!
	*  \brief Reverse the order of the given string. Can also replace the 't' with 'u'.
	*
	*  This static method reverse each character of the given string, 
	*  meaning that a string like "Hello World!" will be changed in "!dlroW olleH".
	*  Usefull for transcription and target binding sites. 
	*  Return the resulting string <b>in lowercase</b>.
	*
	*  \param strToReverse : The string to be reversed.
	*  \param shouldReplaceTWithU : true if all the 't' in the given string should be replace by 'u' (optionnal : default is false)
	*  \return The reversed string in lowercase.
	*/
	static std::string reverseSequence(std::string strToReverse, bool shouldReplaceTWithU = false);

	static void revertSequence(std::string & p_pStr);
private:
	/*!
	*  \brief Check if a miRNA sequence is correct.
	*
	*  This static method analyze the given string, and return an error_check_miRNA_seq enumeration, depending on the result.
	*  Also, it erases the spaces.
	*
	*  \param p_pStr : The miRNA sequence having to be checked.
	*  \return A error_check_miRNA_seq : <br/> The value is : 
	* <ul>
	*   <li>CORRECT_SEQ if the sequence is ok.</li>
	*   <li>ERROR_BAD_SEQ_FORMAT if the miRNA sequence has not a correct format (not only 'a','c','g','t','u' for example).</li>
	*   <li>ERROR_BAD_SEQ_LENGTH if the miRNA sequence has a bad length, too short or too long.</li>
	* </ul>
	*/
	static error_check_miRNA_seq check_miRNA_string(std::string p_pStr);

	static double getKFromMiRNALength(unsigned int length);

	//CLab3Doc* GetDocument();
	//CRect			m_rcClient;
	//bool			m_bRedraw;
	//HANDLE			m_hf;
	//std::ifstream m_hf2;

	//CStdioFile	m_file;
	//static int* weightMatrix;
	//int m_igapPenalty1;
	//int m_igapPenalty2;
public:
	std::vector<miRNA_data> m_pMiRNA;
private:
	//double m_costMatrix[25][25];
	//double m_dCutoff2;
	//double m_dIdealScore2;
	//std::string m_sCutoff;
	Gene_Sequence m_oGene;				//probably Local
	//std::string m_sRes1_long;
	//std::string m_sRes2_long;
	//std::string m_sRes3_long;
	long m_lTotalTargetNumber;			//probably Local
	long m_lTotalPairNumber;			//probably Local
	//long m_lPosition_Start;
	//long m_lPosition_End;
	std::vector<double> m_dPositionWeight;
	std::vector<double> m_dPositionWeightGaps;
	//bool m_bMoreSeed;
	//bool m_b50versus80;
	//long m_lMaxDistance;
	////int m_iDistanceDistribution[20];	
	double m_dK;						//probably Local
	int m_iNumber_miRNA; /* count the number of sequences of the file. */
	std::vector<target_data> m_targets;
};



#endif	//TARGETFINDER_H