#include "targetFinder.h"

using namespace std;

map<char, map<char, int>> TargetFinder::tabLetterIndex({
		{ 'a',	{ { 'a', 0 }, { 'g', 1 }, { 'c', 2 }, { 'u', 3 }, { 't', 3 }, { '-', 16 }, { 'n', 16 } }},
		{ 'g',	{ { 'a', 4 }, { 'g', 5 }, { 'c', 6 }, { 'u', 7 }, { 't', 7 }, { '-', 17 }, { 'n', 17 } } },
		{ 'c',	{ { 'a', 8 }, { 'g', 9 }, { 'c', 10 }, { 'u', 11 }, { 't', 11 }, { '-', 18 }, { 'n', 18 } } },
		{ 'u',	{ { 'a', 12 }, { 'g', 13 }, { 'c', 14 }, { 'u', 15 }, { 't', 15 }, { '-', 19 }, { 'n', 19 } } },
		{ 't',	{ { 'a', 12 }, { 'g', 13 }, { 'c', 14 }, { 'u', 15 }, { 't', 15 }, { '-', 19 }, { 'n', 19 } } },
		{ '-',	{ { 'a', 20 }, { 'g', 21 }, { 'c', 22 }, { 'u', 23 }, { 't', 23 }, { '-', 24 }, { 'n', 24 } } },
		{ 'n',	{ { 'a', 20 }, { 'g', 21 }, { 'c', 22 }, { 'u', 23 }, { 't', 23 }, { '-', 24 }, { 'n', 24 } } }
}); //initialization of the tabLetterIndex (corresponding of the int CLab3View::getLetterIndex(char letter1, char letter2) function)

map<char, map<char, char>> TargetFinder::tabLetterCorresp({
		{ 'a', { { 'a', ' ' }, { 'g', ' ' }, { 'c', ' ' }, { 'u', '|' }, { 't', '|' }, { '-', ' ' }, { ' ', ' ' } } },
		{ 'g', { { 'a', ' ' }, { 'g', ' ' }, { 'c', '|' }, { 'u', '.' }, { 't', '.' }, { '-', ' ' }, { ' ', ' ' } } },
		{ 'c', { { 'a', ' ' }, { 'g', '|' }, { 'c', ' ' }, { 'u', ' ' }, { 't', ' ' }, { '-', ' ' }, { ' ', ' ' } } },
		{ 'u', { { 'a', '|' }, { 'g', '.' }, { 'c', ' ' }, { 'u', ' ' }, { 't', ' ' }, { '-', ' ' }, { ' ', ' ' } } },
		{ 't', { { 'a', '|' }, { 'g', '.' }, { 'c', ' ' }, { 'u', ' ' }, { 't', ' ' }, { '-', ' ' }, { ' ', ' ' } } },
		{ '-', { { 'a', ' ' }, { 'g', ' ' }, { 'c', ' ' }, { 'u', ' ' }, { 't', ' ' }, { '-', ' ' }, { ' ', ' ' } } },
		{ ' ', { { 'a', ' ' }, { 'g', ' ' }, { 'c', ' ' }, { 'u', ' ' }, { 't', ' ' }, { '-', ' ' }, { ' ', ' ' } } }
}); //initialization of the tabLetterIndex (corresponding of the int CLab3View::getLetterIndex(char letter1, char letter2) function)

TargetFinder::TargetFinder() : m_pMiRNA(), m_iNumber_miRNA(0), m_oGene(), m_lTotalTargetNumber(0), m_lTotalPairNumber(0),
m_dPositionWeight(), m_dPositionWeightGaps(), m_dK()
{
}

TargetFinder::~TargetFinder()
{
	for (int i= m_sTempSeq.size() - 1; i >= 0; i--)
	{
		delete m_sTempSeq[i];
		m_sTempSeq.pop_back();
	}
}

bool TargetFinder::getMiRNAs(string const& inputFileName)
{
	error_check_miRNA_seq error_res;
	string l_sTemp;
	ifstream inputFile(inputFileName);
	string line;
	miRNA_data miRNAData;

	long l_iNumber_miRNA(-1);							//--------------- COUNTER ?

	if (inputFile)
	{
		m_iNumber_miRNA = 0;
		/*while (getline(inputFile,line))
		{
			if (line[0] == CHAR_BEGIN_HEADER_LINE)
			{
				m_iNumber_miRNA++;
			}
		} //counting the number of sequences .. why not for the progress bar
		*/
		//from here, we know how many sequences there is.

		//outputFile.open(utils::appendTextBeforeExtension(inputFileName,CONST_DEFAULT_SUFFIX));

		inputFile.seekg(0, inputFile.beg); //return to the beggining of the file

		while (getline(inputFile,line))
		{
			if (line[0] == CHAR_BEGIN_HEADER_LINE)
			{
				if (l_iNumber_miRNA >= 0)
				{
					//we have already written at least 1 complete sequence. Checking it
					error_res = TargetFinder::check_miRNA_string(miRNAData.miRNA);//---------------ASSUMING ONE sequence per line
					if (error_res == error_check_miRNA_seq::ERROR_BAD_SEQ_FORMAT)
					{
						throw ERROR_MESSAGE_BAD_SEQUENCE_FORMAT;
					}
					else if (error_res == error_check_miRNA_seq::ERROR_BAD_SEQ_LENGTH)
					{
						throw ERROR_MESSAGE_BAD_LENGTH;
					}
					else if (error_res == error_check_miRNA_seq::CORRECT_SEQ)
					{
						miRNAData.miRNA = TargetFinder::reverseSequence(miRNAData.miRNA, true);
						m_pMiRNA.push_back(miRNAData);
						miRNAData.clear();
					}
				}
				//header line
				l_iNumber_miRNA++;
				//l_iPos1 = line.find("-");
				//l_iPos2 = line.find(" ");
				//if (l_iPos1 < line.length() && l_iPos2 < line.length())
				//{
					//m_pMiRNA[l_iNumber_miRNA].name = line.substr(l_iPos1+1,l_iPos2-l_iPos1-1);
				//}
				miRNAData.name = line.substr(1);	//---------------SAVING IN MEMORY
			}else
			{
				if (l_iNumber_miRNA >= 0)
				{
					miRNAData.miRNA += line;
				}
				else
				{
					throw ERROR_MESSAGE_NO_HEADER;
				}
			}
		}
		//we load the last one
		//we have already written at least 1 complete sequence. Checking it
		error_res = TargetFinder::check_miRNA_string(miRNAData.miRNA);//---------------ASSUMING ONE sequence per line
		if (error_res == error_check_miRNA_seq::ERROR_BAD_SEQ_FORMAT)
		{
			throw ERROR_MESSAGE_BAD_SEQUENCE_FORMAT;
		}
		else if (error_res == error_check_miRNA_seq::ERROR_BAD_SEQ_LENGTH)
		{
			throw ERROR_MESSAGE_BAD_LENGTH;
		}
		else if (error_res == error_check_miRNA_seq::CORRECT_SEQ)
		{
			miRNAData.miRNA = TargetFinder::reverseSequence(miRNAData.miRNA, true);
			m_pMiRNA.push_back(miRNAData);	//adding the last sequence too
		}
	}
	else
	{
		throw ERROR_MESSAGE_NO_OPEN_INPUT_FILE;
	}
	m_iNumber_miRNA = l_iNumber_miRNA+1;
	return m_pMiRNA.size() == m_iNumber_miRNA;
}

error_check_miRNA_seq TargetFinder::check_miRNA_string(string p_pStr)
{
	string temp;
	int length = p_pStr.length();

	while (p_pStr.find(' ') < p_pStr.length()) //erasing the spaces
	{
		p_pStr.erase(p_pStr.find(' '));
	}

	if (length < DEFAULT_MINIMUM_MIRNA_LENGTH || length > DEFAULT_MAXIMUM_MIRNA_LENGTH)
	{
		return error_check_miRNA_seq::ERROR_BAD_SEQ_LENGTH;
	}

	p_pStr=utils::toLower(p_pStr);

	for (int i = 0; i < length; i++)
	{
		if (p_pStr[i] != 'a' && p_pStr[i] != 'c' && p_pStr[i] != 'g' && p_pStr[i] != 't' && p_pStr[i] != 'u')
		{
			return error_check_miRNA_seq::ERROR_BAD_SEQ_FORMAT;
		}
	}

	return error_check_miRNA_seq::CORRECT_SEQ;
}

string TargetFinder::reverseSequence(std::string strToReverse, bool shouldReplaceTWithU)
{
	unsigned int length = strToReverse.length();
	string temp;

	strToReverse = utils::toLower(strToReverse);
	for (int i = length - 1; i >= 0; i--) //reversing the whole sequence
	{
		if (strToReverse[i] == 't' && shouldReplaceTWithU)
		{
			temp.push_back('u');	//remplacing the 't' with the 'u'
		}
		else
		{
			temp.push_back(strToReverse[i]);
		}
	}
	return temp;
}

//CUTOFF should be multiplied by const_dIdealScore2!! to match the value of m_dCutoff2

void TargetFinder::loadSequenceFile(string const& inputFileName)
{
	bool l_bIsNotEnd(true), l_bIsHeader(false);
	string line;
	string l_sTemp;
	//int l_ilength;
	m_lTotalTargetNumber = 0;
	m_lTotalPairNumber = 0;
	unsigned int l_iPos0, l_iPos1, l_iPos2, l_iPos3, l_iPos4;
	ifstream inputFile(inputFileName);
	Target l_oTempTarget;

	if (inputFile)
	{
		if (m_file)
		{
			m_file.close();
		}
		m_file.open(utils::appendTextBeforeExtension(inputFileName, CONST_DEFAULT_SUFFIX));

		if (m_file)
		{
			// read file
			string l_sStr;
			int l_iLength;
			while (l_bIsNotEnd)
			{
				getline(inputFile, line);
				l_bIsNotEnd = inputFile.good();
				l_bIsHeader = false;
				if (line[0] == CHAR_BEGIN_HEADER_LINE)  //in order to do the last one
				{
					l_bIsHeader = true;
					//>ENSG00000134516 range=chr19:44430527-44430726 5'pad=100 3'pad=100 strand=+ repeatMasking=none
					l_iPos0 = line.find(" ");
					l_iPos1 = line.find("=");
					l_iPos2 = line.find(":");
					l_iPos3 = line.find("-");
					l_iPos4 = line.find("nd=");
					m_oGene.m_sGroupNumber = (l_iPos0 < line.length()) ? line.substr(1, l_iPos0) : "";									//ENSG00000134516
					m_oGene.m_sChrNumber = (l_iPos1 < line.length() && l_iPos2 < line.length()) ? line.substr(l_iPos1 + 1, l_iPos2 - l_iPos1 - 1) : "";			//chr19
					m_oGene.m_sStartPosition = (l_iPos3 < line.length() && l_iPos2 < line.length()) ? line.substr(l_iPos2 + 1, l_iPos3 - l_iPos2 - 1) : "";		//44430527
					m_oGene.m_sSign = (l_iPos4 < line.length()) ? line.substr(l_iPos4 + 3, 1)[0] : CONST_DEFAULT_SIGN;									//+
					//CFileException fe;
					//CString path = ofn.lpstrFile;
					//path.Insert(path.GetLength()-4,"_");
					//path.Insert(path.GetLength()-4,m_oGene.m_sChrNumber);
					//path.Insert(path.GetLength()-4,"_start_pos_");
					//path.Insert(path.GetLength()-4,m_oGene.m_sStartPosition);
					//path.Insert(path.GetLength()-4,m_oGene.m_sSign);
					//path.Insert(path.GetLength()-4,"_");
					//m_file.Open(path, CFile::modeCreate | CFile::modeNoTruncate|
					//CFile::modeReadWrite | CFile::shareExclusive, &fe);
					
				}
				if (l_bIsHeader || !l_bIsNotEnd)
				{
					if (m_oGene.m_sSequence != "")
					{
						m_oGene.m_sSequence = utils::toLower(m_oGene.m_sSequence);	//lowering the sequence
						m_sTempSeq.push_back(new string(m_oGene.m_sSequence));
						for (int i = 0; i < m_iNumber_miRNA; i++)					//for each miRNA...
						{
							cout << endl << "------------------------------------------------" << endl;
							cout << "MIRNA " << i << "/" << m_iNumber_miRNA ;
							cout << endl << "------------------------------------------------" << endl;
							l_iLength = m_pMiRNA[i].miRNA.length();
							//					l_sStr = "miRNA name = ";
							//					l_sStr += (m_pMiRNA+i)->name;
							//					l_sStr += '\n';	
							//					l_sStr += '\n';	
							//					m_file.Write(l_sStr, l_sStr.GetLength());
							m_dK = TargetFinder::getKFromMiRNALength(l_iLength);

							NormilizePositionWeight(l_iLength);
							NormilizePositionWeightGaps(l_iLength);

							l_oTempTarget.setMiRNA(m_pMiRNA[i]);
							//l_oTempTarget.setMiRNA(m_)
							if (m_oGene.m_sSign == '+')
							{
								l_oTempTarget.setSequence(m_sTempSeq[m_sTempSeq.size()-1]);
								for (double j = CONST_MINIMUM_CUTOFF; j <= CONST_MAXIMUM_CUTOFF; j += CONST_STEP_CUTOFF)
								{
									cout << endl << endl << "CUTOFF : " << j << endl;
									findTarget(l_oTempTarget, true, j*const_dIdealScore2);//naum	
								}						
							}
							else if (m_oGene.m_sSign == '-')
							{
								revertSequence(*m_sTempSeq[m_sTempSeq.size() - 1]);
								l_oTempTarget.setSequence(m_sTempSeq[m_sTempSeq.size() - 1]);
								for (double j = CONST_MINIMUM_CUTOFF; j <= CONST_MAXIMUM_CUTOFF; j += CONST_STEP_CUTOFF)
								{
									cout << endl << endl << "CUTOFF (neg) : " << j << endl;
									findTarget(l_oTempTarget, false, j*const_dIdealScore2);
								}
							}
							else
							{
								throw ERROR_MESSAGE_BAD_HEADER_FORMAT;
							}
						}
						m_oGene.m_sSequence = "";
						m_oGene.m_sChrNumber = "";
						m_oGene.m_sGroupNumber = "";
						m_oGene.m_sSign = CONST_DEFAULT_SIGN;
						//SetDlgItemInt(IDC_NUMBER, m_lTotalTargetNumber);			//--------------- ???
						//SetDlgItemInt(IDC_PAIR_NUMBER, m_lTotalPairNumber);		//--------------- ???
					}
				}
				else
				{
					m_oGene.m_sSequence += line;	//adding the non header line to m_oGene.m_sSequence
				}
			}//end while
			
			l_sStr = "total number of targets = " + utils::convertToString<long>(m_lTotalTargetNumber);
			l_sStr += '\n';
			m_file << l_sStr;
			l_sStr = "total number of target pairs = " + utils::convertToString<long>(m_lTotalPairNumber);
			l_sStr += '\n';
			l_sStr += '\n';
			m_file << l_sStr;
			m_oGene.m_sSequence="";
			m_oGene.m_sChrNumber = "";
			m_oGene.m_sGroupNumber = "";
			m_oGene.m_sStartPosition = "";
			m_oGene.m_sSign = CONST_DEFAULT_SIGN;
		}
		else
		{
			throw ERROR_MESSAGE_NO_OPEN_OUTPUT_FILE;
		}
	}
	else
	{
		throw ERROR_MESSAGE_NO_OPEN_INPUT_FILE;
	}
}


double TargetFinder::getKFromMiRNALength(unsigned int length)
{
	double K;
	switch (length)
	{
		case 17: K = 1.0177; break;
		case 18: K = 1.0177; break;
		case 19: K = 1.0177; break;
		case 20: K = 1.0177; break;
		case 21: K = 1.0177; break;
		case 23: K = 0.9818; break;
		case 24: K = 0.9634; break;
		case 25: K = 0.9634; break;
		case 26: K = 0.9634; break;
		case 27: K = 0.9634; break;
		case 28: K = 0.9634; break;
		default: K = DEFAULT_K_VALUE; break;
	}
	return K;
}


//all p_iLength was -1 before
void TargetFinder::NormilizePositionWeight(int p_iLength)
{
	for (int in = 0; in < p_iLength ; in++)
	{
		m_dPositionWeight.push_back(WEIGHT[in]);
	}

	double l_dMin = 10000;

	for (int in = 0; in < p_iLength ; in++)
	{
		if (l_dMin > m_dPositionWeight[in])
		{
			l_dMin = m_dPositionWeight[in];
		}
	}

	double l_dKoef = 0.75; // 0 =< l_dKoef < 1 

	for (int in = 0; in < p_iLength ; in++)
	{
		m_dPositionWeight[in] = m_dPositionWeight[in] - l_dKoef*l_dMin;
	}

	double l_dTotal = 0;

	for (int in = 0; in < p_iLength ; in++)
	{
		l_dTotal += m_dPositionWeight[in];
	}

	for (int in = 0; in < p_iLength ; in++)
	{
		m_dPositionWeight[in] /= l_dTotal;
		m_dPositionWeight[in] *= p_iLength;
	}
}

//all p_iLength was -1 before
void TargetFinder::NormilizePositionWeightGaps(int p_iLength)
{
	for (int in = 0; in < p_iLength; in++)
	{
		m_dPositionWeightGaps.push_back(WEIGHT_GAPS[in]);
	}

	double l_dMax = 0;
	for (int in = 0; in < p_iLength; in++)
	{
		if (l_dMax < m_dPositionWeightGaps[in])
		{
			l_dMax = m_dPositionWeightGaps[in];
		}
	}

	for (int in = 0; in < p_iLength; in++)
	{
		if (m_dPositionWeightGaps[in] == 0)
		{
			m_dPositionWeightGaps[in] = 100 / 20;					//------------------------- ?
		}
		else
		{
			m_dPositionWeightGaps[in] = l_dMax / m_dPositionWeightGaps[in] / 4;
		}
	}
}

//m_file should be open
void TargetFinder::findTarget(Target & p_oTarget, bool p_bStraitgh, double p_dCutoff) //p_bStraitgh true si '+' sign, false if '-'
{
	long l_lUTRSequenceLength = p_oTarget.getSequence().length();
	int l_iStart;
	double l_dScore;
	string l_sScore;
	//target_data td_temp;
//	int sign;

	//l_cMotif[l_lUTRSequenceLength] = '\0';


	//don't need to loop, we do everything in one shot!
	l_dScore = smithWaterman(p_oTarget, p_dCutoff);
	if (l_dScore > p_dCutoff*m_dK)
	{
		//l_dScore is greater than the cutoff : good finding
		p_oTarget.getMiRNA().incNumber();// m_pMiRNA[p_lMiRNA_number].number++;
		writeName(p_oTarget.getMiRNA());		//writing the miRNA name in the output file
				//------------------------------ DOUBLE
						/*if (m_lPosition_Start + i - l_lPosition_End < 200)
						{
							p_oTarget.getMiRNA().incPairs();
							m_lTotalPairNumber++;
							//m_iDistanceDistribution[(int)((m_lPosition_Start + i - l_lPosition_End)/10)]++;
							writeFoundDoubleAt(m_lPosition_Start + (long)i - l_lPosition_End);	//writing the found double in the output file
						}*/
				//------------------------------ /DOUBLE
		m_lTotalTargetNumber++;

		//writeInfo(m_oGene, p_bStraitgh,p_bStraitgh == true ? i + m_lPosition_Start + 1 : l_lUTRSequenceLength - i - m_lPosition_Start);
		writeInfo(m_oGene, p_bStraitgh);

		l_iStart = utils::convertStringToNumber<int>(m_oGene.m_sStartPosition);
		l_iStart += p_bStraitgh == true ? /*i +*/ p_oTarget.getPositionStart() + 1 : l_lUTRSequenceLength /*- i*/ - p_oTarget.getPositionStart();
		//l_iStart += p_bStraitgh == true ? i + p_oTarget.getPositionStart() + 1 : l_lUTRSequenceLength - i - m_lPosition_Start;
		/// positions

		p_oTarget.setPosition(l_iStart - utils::convertStringToNumber<long>(m_oGene.m_sStartPosition));
		//td_temp.miRNA_name = p_oTarget.getMiRNA().getName();
		//m_targets[m_lTotalTargetNumber - 1]
		p_oTarget.setScore(l_dScore);
		//m_targets.push_back(td_temp);

		l_sScore = " ";
		if (p_bStraitgh == true)
		{
			l_sScore += " +";
		}
		else
		{
			l_sScore += " -";
		}
		writeString(" Position = " + m_oGene.m_sStartPosition + l_sScore, false);
		writeRes(p_oTarget);		//writing the res into the output file

		l_sScore = "Score = ";
		//l_sScore += _fcvt_s(l_dScore / const_dIdealScore2, DEFAULT_SCORE_DECIMAL_NUMBER, &dec, &sign);
		l_sScore += utils::convertToString<double>(l_dScore / const_dIdealScore2);
		//l_sScore += ".";
		//l_sScore += utils::convertToString<int>(dec);
		l_sScore += '\n';
		writeString(l_sScore, false);
		p_oTarget.display();
	}
}

void TargetFinder::writeName(miRNA_data data)
{
	string l_sScore("miRNA name = ");
	l_sScore += data.name;
	l_sScore += '\n';
	writeString(l_sScore,false);
}

void TargetFinder::writeFoundDoubleAt(long position)
{
	string l_sScore("Found double at distance ");
	l_sScore += utils::convertToString<long>(position);
	l_sScore += " ";
	writeString(l_sScore);
}

void TargetFinder::writeInfo(Gene_Sequence const& p_oGene, bool p_bStraitgh)
{
	string l_sScore;
	l_sScore = p_oGene.m_sGroupNumber;
	l_sScore += "  ";
	l_sScore += p_oGene.m_sChrNumber;
	writeString(l_sScore, false, false);
}

void TargetFinder::writeInfo(Gene_Sequence const& p_oGene, bool p_bStraitgh, long value)
{
	string l_sScore;
	l_sScore = p_oGene.m_sGroupNumber;
	l_sScore += "  ";
	l_sScore += p_oGene.m_sChrNumber;
	l_sScore += " position=";
	///
	l_sScore += p_oGene.m_sStartPosition + utils::convertToString<long>(value);
	l_sScore += " ";
	if (p_bStraitgh == true)
	{
		l_sScore += " +";
	}
	else
	{
		l_sScore += " -";
	}
	writeString(l_sScore, false);
}

void TargetFinder::writeRes(Target const& p_oTarget)
{
	string l_sScore(utils::toUpper(p_oTarget.getRes1_long()));
	l_sScore += '\n';
	l_sScore += p_oTarget.getRes3_long();
	l_sScore += '\n';
	l_sScore += utils::toUpper(p_oTarget.getRes2_long());
	writeString(l_sScore, false);
}

void TargetFinder::writeString(string const& str, bool shouldInsertLineBefore, bool shouldInsertLineAfter)
{
	if (m_file)
	{
		if (shouldInsertLineBefore)
		{
			m_file << endl;
		}
		m_file << str;
		if (shouldInsertLineAfter)
		{
			m_file << endl;
		}
	}
	else
	{
		throw ERROR_MESSAGE_NO_OPEN_OUTPUT_FILE;
	}
}

double TargetFinder::smithWaterman(Target & p_oTarget, double p_dCutoff)
{
	//p_s2 : miRNA
	//p_s1 : sequence
	//p_iLength : length of p_s1

	//	Check_string(p_s1, true);
	int l_iscore = 0;
	string s2 = p_oTarget.getMiRNA().miRNA;

	int l_lenS1 = p_oTarget.getSequence().length(); // first sequence
	int lenS2 = s2.length(); // second sequence (miRNA)
	int lenS1 = l_lenS1; //---------------------- pas sur
	int scoreZero = 0;
	string s1;
	double l_dScoreMax = 0;
	int bestI = 0;
	int bestJ = 0;
	int bestScoreMatrix = scoreZero;
	int l_iGapPenalty1 = const_igapPenalty1;
	int l_iGapPenalty2 = const_igapPenalty2;
	int scoreIfGoingDown = 0;
	int scoreIfGoingRight = 0;
	int scoreIfGoingDown2 = 0;
	int scoreIfGoingRight2 = 0;
	int scoreIfGoingDownRight = 0;
	int bestScore = 0;
	int ind1 = 0;
	int ind2 = 0;
	// based on number of letters we create a matrix
	vector<vector<int>> weightMatrix;	//matrix of (lenS1 + 1) lines and (lenS2 + 1) columns
	vector<vector<int>> gapMatrix;		//matrix of (lenS1 + 1)*(lenS2 + 1) elements
	vector<int> temp1, temp2;


	//for (int iStr = 0; iStr <= l_lenS1 - p_oTarget.getSequence().length(); iStr++)
	//for (unsigned int iStr = 0; iStr <= l_lenS1 - p_oTarget.getMiRNA().miRNA.length(); iStr ++)
	{
		//s1 = p_oTarget.getSequence().substr(iStr, p_oTarget.getSequence().length()); 
		s1 = p_oTarget.getSequence().substr();
		lenS1 = s1.length();
			/*we first take all the sequence 
			then all the sequence minus the first char
			then all the sequence minus the two first chars...*/

		//fill out first row and column by 0	
		temp1.clear();
		temp2.clear();
		weightMatrix.clear();
		gapMatrix.clear();
		for (int j = 0; j <= lenS2; j++)
		{
			//weightMatrix[lenS1*j + i] = scoreZero;
			temp1.push_back(scoreZero);
			//gapMatrix[lenS1*j + i] = 0;
			temp2.push_back(0);
		}
		for (int i = 0; i <= lenS1; i++)
		{
			weightMatrix.push_back(temp1);
			gapMatrix.push_back(temp2);
		}

		//we browse the lenS1 (char by char?)
		for (int i = 1; i < lenS1; i++)
		{
			//strcpy(char1, s1.Mid(i - 1, 1));
			//strcpy(char12, s1.Mid(i, 1));
			for (int j = 1; j < lenS2; j++)
			{
				//strcpy(char2, s2.Mid(j - 1, 1));
				//strcpy(char22, s2.Mid(j, 1));
				ind1 = tabLetterIndex[s1[i - 1]][s1[i]];
				ind2 = tabLetterIndex[s2[j - 1]][s2[j]];

				if (gapMatrix[i - 1][j] != 1)
				{
					scoreIfGoingDown = weightMatrix[i - 1][j] + (int)(m_dPositionWeightGaps[lenS2 - 1 - j] * l_iGapPenalty1);
				}
				else
				{
					scoreIfGoingDown = weightMatrix[i - 1][j] + (int)(m_dPositionWeightGaps[lenS2 - 1 - j] * l_iGapPenalty2);
				}
				if (gapMatrix[i][j - 1] != 2)
				{
					scoreIfGoingRight = weightMatrix[i][j - 1] + (int)(m_dPositionWeightGaps[lenS2 - j] * l_iGapPenalty1); //bug
				}
				else
				{
					scoreIfGoingRight = weightMatrix[i][j - 1] + (int)(m_dPositionWeightGaps[lenS2 - j] * l_iGapPenalty2);
				}

				scoreIfGoingDownRight = weightMatrix[i - 1][j - 1  ] + (int)(1000 * m_dPositionWeight[lenS2 - 1 - j] * const_costMatrix[ind1][ind2]);

				bestScore = max(
					max(scoreIfGoingDown, scoreIfGoingRight),
					max(scoreIfGoingDownRight, scoreZero));
				if (bestScore == scoreIfGoingDown)
				{
					gapMatrix[i][j] = 1;
				}
				else if (bestScore == scoreIfGoingRight)
				{
					gapMatrix[i][j] = 2;
				}

				weightMatrix[i][j] = bestScore;
				if (bestScore > bestScoreMatrix)
				{
					bestScoreMatrix = bestScore;
					bestI = i;
					bestJ = j;
				}
			}
		}
		if (bestScoreMatrix > l_dScoreMax)
		{
			l_dScoreMax = bestScoreMatrix;
		}
	}

	if (l_dScoreMax / (lenS2 - 1) / 100 > m_dK*p_dCutoff)
	{
		string res1, res1t;
		string res2, res2t;

		//		m_lPosition_End = bestI;
		int ii = bestI;
		int jj = bestJ;

		// This way we return the score to the calling method!
		l_iscore = bestScoreMatrix;
		bool l_bNewGapDown = true;
		bool l_bNewGapRight = true;
		while (ii > 0 && jj > 0)
		{
			if (weightMatrix[ii][jj] == scoreZero)
			{
				break;	//------------------------ caca
			}

			ind1 = tabLetterIndex[s1[ii - 1]][s1[ii]];	//------------------------ important
			ind2 = tabLetterIndex[s2[jj - 1]][s2[jj]];	//------------------------ important

			scoreIfGoingDown = weightMatrix[ii - 1][jj] + (int)(m_dPositionWeightGaps[lenS2 - 1 - jj] * l_iGapPenalty1);
			scoreIfGoingDown2 = weightMatrix[ii - 1][jj] + (int)(m_dPositionWeightGaps[lenS2 - 1 - jj] * l_iGapPenalty2);
			scoreIfGoingRight = weightMatrix[ii][jj - 1] + (int)(m_dPositionWeightGaps[lenS2 - jj] * l_iGapPenalty1);
			scoreIfGoingRight2 = weightMatrix[ii][jj - 1] + (int)(m_dPositionWeightGaps[lenS2 - jj] * l_iGapPenalty2);

			if (weightMatrix[ii][jj] == scoreIfGoingDown || weightMatrix[ii][jj] == scoreIfGoingDown2)
			{
				// Go up !
				res1.insert(0, s1.substr(ii - 1, 1));
				res1t.insert(0, s1.substr(ii - 1, 1));
				res2.insert(0, "-");
				ii--;
				l_bNewGapDown = false;
				l_bNewGapRight = true;
			}
			else if (weightMatrix[ii][jj] == scoreIfGoingRight || weightMatrix[ii][jj] == scoreIfGoingRight2)
			{
				// Go left !
				res1.insert(0, "-");
				res2.insert(0, s2.substr(jj - 1, 1));
				res2t.insert(0, s2.substr(jj - 1, 1));
				jj--;
				l_bNewGapDown = true;
				l_bNewGapRight = false;
			}
			else
			{
				// Go up and left !
				res1.insert(0, s1.substr(ii - 1, 1));
				res2.insert(0, s2.substr(jj - 1, 1));
				res1t.insert(0, s1.substr(ii - 1, 1));
				res2t.insert(0, s2.substr(jj - 1, 1));
				ii--;
				jj--;
				l_bNewGapDown = true;
				l_bNewGapRight = true;
			}
		}
		while (ii > 0)
		{
			if (weightMatrix[ii][jj] == scoreZero)
			{
				break;
			}

			// Go up !		 
			res1.insert(0, s1.substr(ii - 1, 1));
			res1t.insert(0, s1.substr(ii - 1, 1));
			res2.insert(0, "-");
			ii--;
		}
		while (jj > 0)
		{
			if (weightMatrix[ii][jj] == scoreZero)
			{
				break;
			}

			// Go left !
			res1.insert(0, "-");
			res2.insert(0, s2.substr(jj - 1, 1));
			res2t.insert(0, s2.substr(ii - 1, 1));
			jj--;
		}
		int pos1 = s1.find(res1t);
		int pos2 = s2.find(res2t);
		res1 += s1.substr(pos1 + res1t.length(), 1);
		res2 += s2.substr(pos2 + res2t.length(), 1);
		if (pos2 > 0)
		{
			if (pos1 > 0)
			{
				res1.insert(0, s1.substr(pos1 - 1, 1));
			}
			else
			{
				res1.insert(0, " ");
			}
			res2.insert(0, s2.substr(pos2 - 1, 1));
		}
		p_oTarget.setPositionStart(ii);
		p_oTarget.setPositionEnd(p_oTarget.getPositionStart() + res1.length());
		p_oTarget.setRes1_long("5' ");
		p_oTarget.setRes2_long("3' ");
		p_oTarget.setRes1_long(p_oTarget.getRes1_long() + res1);
		p_oTarget.setRes2_long(p_oTarget.getRes2_long() + res2);
		p_oTarget.setRes1_long(p_oTarget.getRes1_long()+" 3'");
		p_oTarget.setRes2_long(p_oTarget.getRes2_long() + " 5'");

		string res3;
		for (unsigned int i = 0; i < res1.length(); i++)
		{
			res3 += tabLetterCorresp[res1[i]][res2[i]];		//------------------------ important
		}
		p_oTarget.setRes3_long("   ");
		p_oTarget.setRes3_long(p_oTarget.getRes3_long() + res3);
	}
	return l_dScoreMax / ((double)(lenS2 - 1)) / 100.0;
}

void TargetFinder::revertSequence(string & p_pStr)
{
	string temp;
	for (int i = p_pStr.length() - 1; i >= 0; i--)
	{
		switch (p_pStr[i])
		{
		case('a') :
			//			case('A'):
			temp+='t';
			break;
		case('c') :
			//			case('C'):
			temp += 'g';
			break;
		case('t') :
		case('u') :
				  //			case('T'):
				  //			case('U'):
				  temp += 'a';
			break;
		case('g') :
			//			case('G'):				
			temp += 'c';
			break;
			//			case('N'):
		case('n') :
			temp += 'n';
			break;
		}
	}
	p_pStr = temp;
}