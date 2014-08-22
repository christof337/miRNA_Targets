#ifndef TARGETFINDER_H
#define TARGETFINDER_H

#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <QObject>

#include "utils.h"
#include "target.h"
#include "settings.h"

const std::string CONST_DEFAULT_SUFFIX = "_result";	//default suffix of the output file
const std::string CONST_DEFAULT_EXTENSION = ".txt";	//default suffix of the output file

/**
* \enum error_check_miRNA_seq
* \brief MiRNA loading error constants.
*
* error_check_miRNA_seq is a serie of predefined error constants about what could bad happen during the loading.
*/
enum class error_check_miRNA_seq {
	CORRECT_SEQ,  /* The miRNA sequence has a correct format. */
	ERROR_BAD_SEQ_FORMAT, /* The miRNA sequence has not a correct format (not only 'a','c','g','t','u' for example). */
	ERROR_BAD_SEQ_LENGTH /* The miRNA sequence has a bad length, too short or too long. */
};

class TargetFinder : public QObject
{
    Q_OBJECT
public:
    explicit TargetFinder();
    //TargetFinder();
    TargetFinder(TargetFinder const& _toCopy);
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

	/*!
	*  \brief Load the sequence file which name is inputFileName and try to find bindings with each of its sequences. 
	*
	*  This static method open the file of the given path, and extract the sequences from it.
	*  Then it try to find binding sites between the found sequences and each of the m_pMiRNA of this TargetFinder object.
	*  Currently, it uses the Smith Waterman algorithm.
	*  See the documentation of the findTarget function for more information.
	*
	*  \param inputFileName : The path of the input file, containing the sequences (FASTA format).
	*/
	void loadSequenceFile(std::string const& inputFileName);

	void NormilizePositionWeight(int p_iLength);
	void NormilizePositionWeightGaps(int p_iLength);

	void findTarget(Target & p_target, bool p_bStraitgh, double p_dCutoff);

	/*!
	*  \brief Find all the targets of the given miRNA in the given sequence which score is greater than the given cutoff.
	*
	*  This method look for the best binding sites of the miRNA on the given gene, using the Smith-Waterman algorithm.
	*  It also loops in order to find all the targets whose score is greater than the cutoff.
	*
	*  \param p_oTarget : The Target containing the given miRNA and gene. It will be copied in each found target returned.
	*  \param p_dCutoff : the value of the cutoff below which no target should be retrieve.
	*  \return A vector of Targets, containing each target fonud which score is lower than the given cutoff.
	*/
	std::vector<Target> smithWaterman(Target & p_oTarget, double p_dCutoff);

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

	/*Change all the 'g' in 'c', all the 'a' in 't', all the 'c' in 'g' and all the 't' and 'u' in 'a' in the given sequence.
	*/
	static void revertSequence(std::string * p_pStr);

    int getNbSequences(std::string const& fileName);

    std::vector<miRNA_data*> getMiRNA() const;
    std::vector<Target> getTargets() const;

    bool estEgal(TargetFinder const& b) const;
private:
	/*!
	*  \brief Check if a miRNA sequence is correct.
	*
	*  This static method analyze the given string, and return an error_check_miRNA_seq enumeration, depending on the result.
	*  Also, it erases the spaces.
	*
	*  \param p_pStr : The miRNA sequence having to be checked.
	*  \return An error_check_miRNA_seq : <br/> The value is : 
	* <ul>
	*   <li>CORRECT_SEQ if the sequence is ok.</li>
	*   <li>ERROR_BAD_SEQ_FORMAT if the miRNA sequence has not a correct format (not only 'a','c','g','t','u' for example).</li>
	*   <li>ERROR_BAD_SEQ_LENGTH if the miRNA sequence has a bad length, too short or too long.</li>
	* </ul>
	*/
	static error_check_miRNA_seq check_miRNA_string(std::string p_pStr);

    static double getKFromMiRNALength(size_t length);

	/**
		This method erase a shorter occurence of p_oCurrentTarget in the last element of p_oVector.
		For example, if we found a nice target which is AAAG, and the previous element is AAA (and begin in the same position)
		then we can erase AAA et add AAAG (to the vector).
		Careful : also check that the score of the previous element isn't greater than the new one : in which case we should keep the old one.
	*/
	static bool erasePrevious(Target const& p_oCurrentTarget, std::vector<Target> & p_oVector);


    static std::string changeTinU(std::string const& input);

	/*!
	*  \brief Write the name of the given miRNA_data in the output file of the current TargetFinder (m_file).
	*
	*  Throw a ERROR_MESSAGE_NO_OPEN_INPUT_FILE exception if the output file (m_file) is not open.
	*
	*  \param miRNA_data : The miRNA data which name should be printed in the file.
	*/
	void writeName(miRNA_data data);
	/*!
	*  \brief Write the given position of a found double in the output file of the current TargetFinder (m_file).
	*
	*  Throw a ERROR_MESSAGE_NO_OPEN_INPUT_FILE exception if the output file (m_file) is not open.
	*
	*  \param miRNA_data : The position which should be printed in the file.
	*/
	void writeFoundDoubleAt(long position);

    void writeInfo(Gene_Sequence const& p_oGene);
	void writeInfo(Gene_Sequence const& p_oGene, bool p_bStraitgh, long value);
	void writeRes(Target const& p_oTarget);
	/*!
	*  \brief Write the given string in the output file of the current TargetFinder (m_file).
	*
	*  Throw a ERROR_MESSAGE_NO_OPEN_INPUT_FILE exception if the output file (m_file) is not open.
	*
	*  \param miRNA_data : The string which should be printed in the file.
	*  \param shouldInsertLineBefore : (optional : default=true) true if a line should be inserted before writing the string.
	*  \param shouldInsertLineAfter : (optional : default=true) true if a line should be inserted after having wrote the string.
	*/
	void writeString(std::string const& str, bool shouldInsertLineBefore = true, bool shouldInsertLineAfter = true);



    int countSequences(std::string const& inputFileName) const;
	//CLab3Doc* GetDocument();
	//CRect			m_rcClient;
	//bool			m_bRedraw;
	//HANDLE			m_hf;
	//std::ifstream m_hf2;

	std::ofstream m_file;
	//static int* weightMatrix;
	//int m_igapPenalty1;
	//int m_igapPenalty2;
private:
	static /*const*/ std::map<char, std::map<char, int>> tabLetterIndex;
	static /*const*/ std::map<char, std::map<char, char>> tabLetterCorresp;
	static /*const*/ std::map<std::string, std::map<std::string, double>> const_costMatrix2;

	std::vector<miRNA_data*> m_pMiRNA;
//private:
	std::vector<std::string *> m_sTempSeq;
	std::vector<Target> m_vTarget;
	long m_lTotalTargetNumber;			//probably Local
	long m_lTotalPairNumber;			//probably Local
	std::vector<double> m_dPositionWeight;
	std::vector<double> m_dPositionWeightGaps;
	double m_dK;						//probably Local
	int m_iNumber_miRNA; /* count the number of sequences of the file. */
    int m_iNbSequences;


	static const std::string ERROR_MESSAGE_BAD_LENGTH;				//Error message for a bad miRNA length
	static const std::string ERROR_MESSAGE_BAD_SEQUENCE_FORMAT;		//Error message for a bad miRNA content
	static const std::string ERROR_MESSAGE_BAD_HEADER_FORMAT;		//Error message for a bad miRNA header format
	static const std::string ERROR_MESSAGE_NO_HEADER;				//Error message for a bad input file format (no header)
	static const std::string ERROR_MESSAGE_NO_OPEN_INPUT_FILE;		//Error message for an impossible to open input file
	static const std::string ERROR_MESSAGE_NO_OPEN_OUTPUT_FILE;		//Error message for an impossible to open output file
	//static const int CONST_NB_LOOP = 10;

signals:
    void signalProgress(int);
public slots:
};

bool operator==(TargetFinder const& a, TargetFinder const& b);
bool operator!=(TargetFinder const& a, TargetFinder const& b);


//const double const_costMatrix[25][25] = {
//	//		  AA     AG      AC      AU      GA      GG      GC      GU      CA      CG      CC      CU      UA      UG      UC      UU,   A-  G-  C-  U-  -A  –G  –C  -U --},
//	/* 1AA*/{ -10, -10, -10, 6.4, -10, -10, -10, 6.4, -10, -10, -10, 6.4, 6.4, 6.4, 6.4, 22, -10, -10, -10, 6.4, -10, -10, -10, 6.4, -10 },
//	/* 2AG*/{ -10, -10, 6.4, 3, -10, -10, 6.4, 3, -10, -10, 6.4, 3, 6.4, 6.4, 22, 9.9, -10, -10, -10, 6.4, -10, -10, 6.4, 3, -10 },
//	/* 3AC*/{ -10, 6.4, -10, -10, -10, 6.4, -10, -10, -10, 6.4, -10, -10, 6.4, 22, 6.4, 6.4, -10, -10, -10, 6.4, -10, 6.4, -10, -10, -10 },
//	/* 4AU*/{ 6.4, 3, -10, -10, 6.4, 3, -10, -10, 6.4, 3, -10, -10, 22, 9.9, 6.4, 6.4, -10, -10, -10, 6.4, 6.4, 3, -10, -10, -10 },
//	/* 5GA*/{ -10, -10, -10, 6.4, -10, -10, -10, 6.4, 6.4, 6.4, 6.4, 22, 3, 3, 3, 9.9, -10, -10, 6.4, 3, -10, -10, -10, 6.4, -10 },
//	/* 6GG*/{ -10, -10, 6.4, 3, -10, -10, 6.4, 3, 6.4, 6.4, 22, 9.9, 3, 3, 9.9, 6, -10, -10, 6.4, 3, -10, -10, 6.4, -10, -10 },
//	/* 7GC*/{ -10, 6.4, -10, -10, -10, 6.4, -10, -10, 6.4, 22, 6.4, 6.4, 3, 9.9, 3, 3, -10, -10, 6.4, 3, -10, 6.4, -10, -10, -10 },
//	/* 8GU*/{ 6.4, 3, -10, -10, 6.4, 3, -10, -10, 22, 9.9, 6.4, 6.4, 9.9, 6, 3, 3, -10, -10, 6.4, 3, 6.4, -10, -10, -10, -10 },
//	/* 9CA*/{ -10, -10, -10, 6.4, 6.4, 6.4, 6.4, 22, -10, -10, -10, 6.4, -10, -10, -10, 6.4, -10, 6.4, -10, -10, -10, -10, -10, 6.4, -10 },
//	/*10CG*/{ -10, -10, 6.4, 3, 6.4, 6.4, 22, 9.9, -10, -10, 6.4, 3, -10, -10, 6.4, 3, -10, 6.4, -10, -10, -10, -10, 6.4, 3, -10 },
//	/*11CC*/{ -10, 6.4, -10, -10, 6.4, 22, 6.4, 6.4, -10, 6.4, -10, -10, -10, 6.4, -10, -10, -10, 6.4, -10, -10, -10, 6.4, -10, -10, -10 },
//	/*12CU*/{ 6.4, 3, -10, -10, 22, 9.9, 6.4, 6.4, 6.4, 3, -10, -10, 6.4, 3, -10, -10, -10, 6.4, -10, -10, 6.4, 3, -10, -10, -10 },
//	/*13UA*/{ 6.4, 6.4, 6.4, 22, 3, 3, 3, 9.9, -10, -10, -10, 6.4, -10, -10, -10, 6.4, 6.4, 3, -10, -10, -10, -10, -10, 6.4, -10 },
//	/*14UG*/{ 6.4, 6.4, 22, 9.9, 3, 3, 9.9, 6, -10, -10, 6.4, 3, -10, -10, 6.4, 3, 6.4, 3, -10, -10, -10, -10, 6.4, -10, -10 },
//	/*15UC*/{ 6.4, 22, 6.4, 6.4, 3, 9.9, 3, 3, -10, 6.4, -10, -10, -10, 6.4, -10, -10, 6.4, 3, -10, -10, -10, 6.4, -10, -10, -10 },
//	/*16UU*/{ 22, 9.9, 6.4, 6.4, 9.9, 6, 3, 3, 6.4, 3, -10, -10, 6.4, 3, -10, -10, 6.4, 3, -10, -10, 6.4, -10, -10, -10, -10 },
//	/*07A-*/{ -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, 6.4, 6.4, 6.4, 6.4, -10, -10, -10, -10, -10, -10, -10, -10, -10 },
//	/*08G-*/{ -10, -10, -10, -10, -10, -10, -10, -10, 6.4, 6.4, 6.4, 6.4, 3, 3, 3, 3, -10, -10, -10, -10, -10, -10, -10, -10, -10 },
//	/*09C-*/{ -10, -10, -10, -10, 6.4, 6.4, 6.4, 6.4, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10 },
//	/*20U-*/{ 6.4, 6.4, 6.4, 6.4, 3, 3, 3, 3, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10 },
//	/*20-A*/{ -10, -10, -10, 6.4, -10, -10, -10, 6.4, -10, -10, -10, 6.4, -10, -10, -10, 6.4, -10, -10, -10, -10, -10, -10, -10, -10, -10 },
//	/*22-G*/{ -10, -10, 6.4, 3, -10, -10, 6.4, 3, -10, -10, 6.4, 3, -10, -10, 6.4, 3, -10, -10, -10, -10, -10, -10, -10, -10, -10 },
//	/*23-C*/{ -10, 6.4, -10, -10, -10, 6.4, -10, -10, -10, 6.4, -10, -10, -10, 6.4, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10 },
//	/*24-U*/{ 6.4, 3, -10, -10, 6.4, 3, -10, -10, 6.4, 3, -10, -10, 6.4, 3, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10 },
//	/*24--*/{ -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10 } };

const double WEIGHT[27] = { 62.3, 94.1, 96.9, 97.4, 91.7, 90.3, 91.2, 54.3, 46.2, 49.0, 48.0, 46.8, 51.6, 46.5, 47.4, 45.7, 44.2, 43.7, 49.0, 44.0, 44.0, 44.0, 44.0, 44.0, 44.0, 44.0, 44.0 };

const double WEIGHT_GAPS[27] = { .0, .0, .0, .0, .0, 1.6, 3.3, 5.0, 20.0, 17.0, 7.5, 5.8, 6.7, 5.0, 5.0, 3.3, 8.3, 6.7, 4.2, 1.7, 1.6, .0, .0, .0, .0, .0, .0 };


#endif	//TARGETFINDER_H
