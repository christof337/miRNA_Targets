#include "targetFinder.h"

using namespace std;

miRNA_data::miRNA_data() : name(), miRNA(), number(0), pairs(0)
{

}


target_data::target_data() : miRNA_name(), position(0), score(0)
{

}

void miRNA_data::clear()
{
	name = "";
	miRNA = "";
	number = 0;
	pairs = 0;
}

TargetFinder::TargetFinder() : m_pMiRNA(), m_iNumber_miRNA(0), m_targets()
{
}


TargetFinder::~TargetFinder()
{
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
		}
	}
	else
	{
		throw ERROR_MESSAGE_NO_OPEN_INPUT_FILE;
	}
	m_iNumber_miRNA = l_iNumber_miRNA;
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
	string line;
	string l_sTemp;
	int l_ilength;
	m_lTotalTargetNumber = 0;
	m_lTotalPairNumber = 0;
	unsigned int l_iPos0, l_iPos1, l_iPos2, l_iPos3, l_iPos4;
	ifstream inputFile(inputFileName);

	if (inputFile)
	{
		ofstream outputFile(utils::appendTextBeforeExtension(inputFileName, CONST_DEFAULT_SUFFIX));

		if (outputFile)
		{
			// read file
			string l_sStr;
			int l_iLength;
			while (getline(inputFile, line))
			{
				if (line[0] == CHAR_BEGIN_HEADER_LINE)  //>
				{
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
					if (m_oGene.m_sSequence != "")
					{
						m_oGene.m_sSequence=utils::toLower(m_oGene.m_sSequence);	//lowering the sequence
						for (int i = 0; i < m_iNumber_miRNA; i++)					//for each miRNA...
						{
							l_iLength = m_pMiRNA[i].miRNA.length();
							//					l_sStr = "miRNA name = ";
							//					l_sStr += (m_pMiRNA+i)->name;
							//					l_sStr += '\n';	
							//					l_sStr += '\n';	
							//					m_file.Write(l_sStr, l_sStr.GetLength());
							m_dK = TargetFinder::getKFromMiRNALength(l_iLength);

							NormilizePositionWeight(l_iLength);
							NormilizePositionWeightGaps(l_iLength);

							if (m_oGene.m_sSign == '+')
							{
								findTarget(i, m_oGene.m_sSequence, true);//naum							
							}
							else if (m_oGene.m_sSign == '-')
							{
								l_ilength = m_oGene.m_sSequence.length();
								l_sTemp = m_oGene.m_sSequence;
								revertSequence(l_sTemp);
								findTarget(i, l_sTemp, false);
							}
							else
							{
								throw ERROR_MESSAGE_BAD_HEADER_FORMAT;
							}
						}
						//			    l_sStr = "total number of targets = ";					
						//				_itoa(m_lTotalTargetNumber,buffer,10);
						//				l_sStr += buffer;
						//				l_sStr += '\n';				
						//				m_file.Write(l_sStr, l_sStr.GetLength());
						//				l_sStr = "total number of target pairs = ";	
						//				_itoa(m_lTotalPairNumber,buffer,10);
						//				l_sStr += buffer;
						//				l_sStr += '\n';				
						//				m_file.Write(l_sStr, l_sStr.GetLength());
						//m_file.Close();
						m_oGene.m_sSequence="";
						m_oGene.m_sChrNumber="";
						m_oGene.m_sGroupNumber="";
						m_oGene.m_sSign=CONST_DEFAULT_SIGN;
						//SetDlgItemInt(IDC_NUMBER, m_lTotalTargetNumber);			//--------------- ???
						//SetDlgItemInt(IDC_PAIR_NUMBER, m_lTotalPairNumber);		//--------------- ???
					}
				}
				else
				{
					m_oGene.m_sSequence += l_sTemp;	//adding the non header line to m_oGene.m_sSequence
				}
			}//end while

			m_oGene.m_sSequence = utils::toLower(m_oGene.m_sSequence);	//lowering the sequence
			for (int i = 0; i < m_iNumber_miRNA; i++)					//for each miRNA...
			{
				l_iLength = m_pMiRNA[i].miRNA.length();
				//					l_sStr = "miRNA name = ";
				//					l_sStr += (m_pMiRNA+i)->name;
				//					l_sStr += '\n';	
				//					l_sStr += '\n';	
				//					m_file.Write(l_sStr, l_sStr.GetLength());
				m_dK = TargetFinder::getKFromMiRNALength(l_iLength);

				NormilizePositionWeight(l_iLength);
				NormilizePositionWeightGaps(l_iLength);

				if (m_oGene.m_sSign == '+')
				{
					findTarget(i, m_oGene.m_sSequence, true);//naum							
				}
				else if (m_oGene.m_sSign == '-')
				{
					l_ilength = m_oGene.m_sSequence.length();
					l_sTemp = m_oGene.m_sSequence;
					revertSequence(l_sTemp);
					findTarget(i, l_sTemp, false);
				}
				else
				{
					throw ERROR_MESSAGE_BAD_HEADER_FORMAT;
				}
			}
			//		for(int i = 0; i < m_iNumber_miRNA; i++)
			//{
			//l_sStr = "miRNA name = ";
			//l_sStr += (m_pMiRNA+i)->name;
			//l_sStr += '\n';
			//m_file.Write(l_sStr, l_sStr.GetLength());
			//l_sStr = "number of targets = ";
			//_itoa((m_pMiRNA+i)->number,buffer,10);
			//l_sStr += buffer;
			//l_sStr += '\n';
			//m_file.Write(l_sStr, l_sStr.GetLength());
			//l_sStr = "number of target pairs = ";
			//_itoa((m_pMiRNA+i)->pairs,buffer,10);
			//l_sStr += buffer;
			//l_sStr += '\n';
			//l_sStr += '\n';
			//m_file.Write(l_sStr, l_sStr.GetLength());
			//}

			l_sStr = "total number of targets = " + utils::convertToString<long>(m_lTotalTargetNumber);
			l_sStr += '\n';
			outputFile << l_sStr;
			l_sStr = "total number of target pairs = " + utils::convertToString<long>(m_lTotalPairNumber);
			l_sStr += '\n';
			l_sStr += '\n';
			outputFile << l_sStr;

			//SetDlgItemInt(IDC_NUMBER, m_lTotalTargetNumber);				//--------------- ???
			//SetDlgItemInt(IDC_PAIR_NUMBER, m_lTotalPairNumber);			//--------------- ???
			m_oGene.m_sSequence = "";
			m_oGene.m_sChrNumber = "";
			m_oGene.m_sGroupNumber = "";
			m_oGene.m_sStartPosition = "";
			m_oGene.m_sSign = CONST_DEFAULT_SIGN;
			//SortTargetPosition();
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


void TargetFinder::NormilizePositionWeight(int p_iLength)
{
	for (int in = 0; in < p_iLength - 1; in++)
	{
		m_dPositionWeight.push_back(WEIGHT[in]);
	}

	double l_dMin = 10000;

	for (int in = 0; in < p_iLength - 1; in++)
	{
		if (l_dMin > m_dPositionWeight[in])
		{
			l_dMin = m_dPositionWeight[in];
		}
	}

	double l_dKoef = 0.75; // 0 =< l_dKoef < 1 

	for (int in = 0; in < p_iLength - 1; in++)
	{
		m_dPositionWeight[in] = m_dPositionWeight[in] - l_dKoef*l_dMin;
	}

	double l_dTotal = 0;

	for (int in = 0; in < p_iLength - 1; in++)
	{
		l_dTotal += m_dPositionWeight[in];
	}

	for (int in = 0; in < p_iLength - 1; in++)
	{
		m_dPositionWeight[in] /= l_dTotal;
		m_dPositionWeight[in] *= (p_iLength - 1);
	}
}


void TargetFinder::NormilizePositionWeightGaps(int p_iLength)
{
	for (int in = 0; in < p_iLength - 1; in++)
	{
		m_dPositionWeightGaps.push_back(WEIGHT_GAPS[in]);
	}

	double l_dMax = 0;
	for (int in = 0; in < p_iLength - 1; in++)
	{
		if (l_dMax < m_dPositionWeightGaps[in])
		{
			l_dMax = m_dPositionWeightGaps[in];
		}
	}

	for (int in = 0; in < p_iLength - 1; in++)
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



void TargetFinder::findTarget(long p_lMiRNA_number, string & p_sUTRsequence, bool p_bStraitgh)
{
	/*long l_lUTRSequenceLength = (*p_sUTRsequence).GetLength();
	char* l_cMotif = new char[l_lUTRSequenceLength + 1];
	char* l_cLong = new char[LENGTH2 + 1];
	char buffer[20];
	int l_iStart;
	double l_dScore;
	CString l_sScore;
	int dec;
	int sign;
	CString l_sString1;
	CString l_sString2 = (m_pMiRNA + p_lMiRNA_number)->miRNA;
	strcpy(l_cMotif, (*p_sUTRsequence));
	l_cMotif[l_lUTRSequenceLength] = '\0';
	long l_lPosition_End = -1000000;
	for (int i = 0; i < l_lUTRSequenceLength + 1 - 7; i++)
	{
		strncpy(l_cLong, l_cMotif + i, LENGTH2);
		l_cLong[LENGTH2] = '\0';
		l_sString1 = l_cLong;
		//		l_sString1.MakeLower();		
		l_dScore = SmithWaterman(&l_sString2, &l_sString1, l_sString1.GetLength() < LENGTH2 ? l_sString1.GetLength() : LENGTH2);
		if (l_dScore > m_dCutoff2*m_dK)
		{
			if (m_lPosition_End <= LENGTH1)
			{
				(m_pMiRNA + p_lMiRNA_number)->number++;
				l_sScore = "miRNA name = ";
				l_sScore += (m_pMiRNA + p_lMiRNA_number)->name;
				l_sScore += '\n';
				l_sScore += '\n';
				m_file.Write(l_sScore, l_sScore.GetLength());
				if (m_lPosition_Start + i - l_lPosition_End < 200)
				{
					(m_pMiRNA + p_lMiRNA_number)->pairs++;
					m_lTotalPairNumber++;
					//m_iDistanceDistribution[(int)((m_lPosition_Start + i - l_lPosition_End)/10)]++;
					_itoa(m_lPosition_Start + i - l_lPosition_End, buffer, 10);
					l_sScore = '\n';
					l_sScore += "Found double at distance ";
					l_sScore += buffer;
					l_sScore += " ";
					l_sScore += '\n';
					m_file.Write(l_sScore, l_sScore.GetLength());
				}
				l_lPosition_End = m_lPosition_End + i;
				m_lTotalTargetNumber++;
				l_sScore = m_oGene.m_sGroupNumber;
				l_sScore += "  ";
				l_sScore += m_oGene.m_sChrNumber;
				l_sScore += " position=";
				l_iStart = atoi(m_oGene.m_sStartPosition);
				l_iStart += p_bStraitgh == true ? i + m_lPosition_Start + 1 : l_lUTRSequenceLength - i - m_lPosition_Start;
				/// positions
				m_targets[m_lTotalTargetNumber - 1].position = l_iStart - atoi(m_oGene.m_sStartPosition);
				m_targets[m_lTotalTargetNumber - 1].miRNA_name = (m_pMiRNA + p_lMiRNA_number)->name;
				m_targets[m_lTotalTargetNumber - 1].score = l_dScore;
				///
				_itoa(l_iStart, buffer, 10);
				l_sScore += buffer;
				l_sScore += " ";
				if (p_bStraitgh == true)
				{
					l_sScore += " +";
				}
				else
				{
					l_sScore += " -";
				}
				l_sScore += '\n';
				m_file.Write(l_sScore, l_sScore.GetLength());
				l_sScore = m_sRes1_long.MakeUpper();
				l_sScore += '\n';
				l_sScore += m_sRes3_long;
				l_sScore += '\n';
				l_sScore += m_sRes2_long.MakeUpper();
				l_sScore += '\n';
				m_file.Write(l_sScore, l_sScore.GetLength());

				l_sScore = "Score = ";
				m_file.Write(l_sScore, l_sScore.GetLength());
				l_sScore = fcvt(l_dScore / m_dIdealScore2, 3, &dec, &sign);
				l_sScore.Insert(dec, ".");
				l_sScore += '\n';
				l_sScore += '\n';
				m_file.Write(l_sScore, l_sScore.GetLength());

				i += m_lPosition_End - 1;
			}
			else
			{
				strncpy(l_cLong, l_cMotif + i, m_lPosition_Start);
				l_cLong[m_lPosition_Start] = '\0';
				l_sString1 = l_cLong;
				//				l_sString1.MakeLower();				
				l_dScore = SmithWaterman(&l_sString2, &l_sString1, l_sString1.GetLength());
				if (l_dScore > m_dCutoff2*m_dK)//(1 + (22-l_sString2.GetLength())/22.0))
				{
					(m_pMiRNA + p_lMiRNA_number)->number++;
					l_sScore = "miRNA name = ";
					l_sScore += (m_pMiRNA + p_lMiRNA_number)->name;
					l_sScore += '\n';
					l_sScore += '\n';
					m_file.Write(l_sScore, l_sScore.GetLength());
					if (m_lPosition_Start + i - l_lPosition_End < 200)
					{
						(m_pMiRNA + p_lMiRNA_number)->pairs++;
						m_lTotalPairNumber++;
						//m_iDistanceDistribution[(int)((m_lPosition_Start + i - l_lPosition_End)/10)]++;
						_itoa(m_lPosition_Start + i - l_lPosition_End, buffer, 10);
						l_sScore = '\n';
						l_sScore += "Found double at distance ";
						l_sScore += buffer;
						l_sScore += " ";
						l_sScore += '\n';
						m_file.Write(l_sScore, l_sScore.GetLength());
					}

					l_lPosition_End = m_lPosition_End + i;

					m_lTotalTargetNumber++;
					l_sScore = m_oGene.m_sGroupNumber;
					l_sScore += "  ";
					l_sScore += m_oGene.m_sChrNumber;
					l_sScore += " position=";
					l_iStart = atoi(m_oGene.m_sStartPosition);
					l_iStart += p_bStraitgh == true ? i + m_lPosition_Start - 1 : l_lUTRSequenceLength - i - m_lPosition_Start;
					/// positions
					m_targets[m_lTotalTargetNumber - 1].position = l_iStart - atoi(m_oGene.m_sStartPosition);;
					m_targets[m_lTotalTargetNumber - 1].miRNA_name = (m_pMiRNA + p_lMiRNA_number)->name;
					m_targets[m_lTotalTargetNumber - 1].score = l_dScore;
					///
					_itoa(l_iStart, buffer, 10);
					l_sScore += buffer;
					l_sScore += " ";
					if (p_bStraitgh == true)
					{
						l_sScore += " +";
					}
					else
					{
						l_sScore += " -";
					}
					l_sScore += '\n';
					m_file.Write(l_sScore, l_sScore.GetLength());
					l_sScore = m_sRes1_long.MakeUpper();
					l_sScore += '\n';
					l_sScore += m_sRes3_long;
					l_sScore += '\n';
					l_sScore += m_sRes2_long.MakeUpper();
					l_sScore += '\n';
					m_file.Write(l_sScore, l_sScore.GetLength());
					l_sScore = "Score = ";
					m_file.Write(l_sScore, l_sScore.GetLength());
					l_sScore = fcvt(l_dScore / m_dIdealScore2, 3, &dec, &sign);
					l_sScore.Insert(dec, ".");
					l_sScore += '\n';
					l_sScore += '\n';
					m_file.Write(l_sScore, l_sScore.GetLength());
					i += m_lPosition_End - 1;
				}
				else
				{
					i += m_lPosition_Start;
				}
			}
		}
		else
		{

			i += LENGTH2 - LENGTH1 - 1;
		}
	}
	delete[]l_cMotif;
	delete[]l_cLong;*/
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