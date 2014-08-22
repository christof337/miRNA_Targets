#include "targetFinder.h"

using namespace std;

/* Constants definitions */
const string TargetFinder::ERROR_MESSAGE_BAD_LENGTH = "The length of miRNA should be between 17 and 28 bp. The input file has wrong format.";	//Error message for a bad miRNA length
const string TargetFinder::ERROR_MESSAGE_BAD_SEQUENCE_FORMAT = "miRNA sequence should include only letters 'A', 'T', 'C', 'G', and 'U'.";		//Error message for a bad miRNA content
const string TargetFinder::ERROR_MESSAGE_BAD_HEADER_FORMAT = "miRNA header should be of this type.\n><name><space> cdna:KNOWN_miRNA";			//Error message for a bad miRNA header format
const string TargetFinder::ERROR_MESSAGE_NO_HEADER = "No header found. FASTA format required.";													//Error message for a bad input file format (no header)
const string TargetFinder::ERROR_MESSAGE_NO_OPEN_INPUT_FILE = "Impossible to open the input file.";												//Error message for an impossible to open input file
const string TargetFinder::ERROR_MESSAGE_NO_OPEN_OUTPUT_FILE = "Error : The output file is not open.";											//Error message for an impossible to open output file
//const int TargetFinder::CONST_NB_LOOP = 10;


map<char, map<char, int>> TargetFinder::tabLetterIndex({
		{ 'a', { { 'a', 0 }, { 'g', 1 }, { 'c', 2 }, { 'u', 3 }, { 't', 3 }, { '-', 16 }, { 'n', 16 } } },
		{ 'g', { { 'a', 4 }, { 'g', 5 }, { 'c', 6 }, { 'u', 7 }, { 't', 7 }, { '-', 17 }, { 'n', 17 } } },
		{ 'c', { { 'a', 8 }, { 'g', 9 }, { 'c', 10 }, { 'u', 11 }, { 't', 11 }, { '-', 18 }, { 'n', 18 } } },
		{ 'u', { { 'a', 12 }, { 'g', 13 }, { 'c', 14 }, { 'u', 15 }, { 't', 15 }, { '-', 19 }, { 'n', 19 } } },
		{ 't', { { 'a', 12 }, { 'g', 13 }, { 'c', 14 }, { 'u', 15 }, { 't', 15 }, { '-', 19 }, { 'n', 19 } } },
		{ '-', { { 'a', 20 }, { 'g', 21 }, { 'c', 22 }, { 'u', 23 }, { 't', 23 }, { '-', 24 }, { 'n', 24 } } },
		{ 'n', { { 'a', 20 }, { 'g', 21 }, { 'c', 22 }, { 'u', 23 }, { 't', 23 }, { '-', 24 }, { 'n', 24 } } }
}); //initialization of the tabLetterIndex (corresponding of the int CLab3View::getLetterIndex(char letter1, char letter2) function)

map<char, map<char, char>> TargetFinder::tabLetterCorresp({
		{ 'a', { { 'a', ' ' }, { 'g', ' ' }, { 'c', ' ' }, { 'u', '|' }, { 't', '|' }, { '-', ' ' }, { ' ', ' ' } } },
		{ 'g', { { 'a', ' ' }, { 'g', ' ' }, { 'c', '|' }, { 'u', '.' }, { 't', '.' }, { '-', ' ' }, { ' ', ' ' } } },
		{ 'c', { { 'a', ' ' }, { 'g', '|' }, { 'c', ' ' }, { 'u', ' ' }, { 't', ' ' }, { '-', ' ' }, { ' ', ' ' } } },
		{ 'u', { { 'a', '|' }, { 'g', '.' }, { 'c', ' ' }, { 'u', ' ' }, { 't', ' ' }, { '-', ' ' }, { ' ', ' ' } } },
		{ 't', { { 'a', '|' }, { 'g', '.' }, { 'c', ' ' }, { 'u', ' ' }, { 't', ' ' }, { '-', ' ' }, { ' ', ' ' } } },
		{ '-', { { 'a', ' ' }, { 'g', ' ' }, { 'c', ' ' }, { 'u', ' ' }, { 't', ' ' }, { '-', ' ' }, { ' ', ' ' } } },
		{ ' ', { { 'a', ' ' }, { 'g', ' ' }, { 'c', ' ' }, { 'u', ' ' }, { 't', ' ' }, { '-', ' ' }, { ' ', ' ' } } }
}); //initialization of the tabLetterCorresp (corresponding of the int CLab3View::getLetterIndex(char letter1, char letter2) function)

TargetFinder::TargetFinder() : m_pMiRNA(), m_iNumber_miRNA(0), m_vTarget(), m_lTotalTargetNumber(0), m_lTotalPairNumber(0),
m_dPositionWeight(), m_dPositionWeightGaps(), m_dK(Settings::getSettings()->getFind_defaultKValue()),m_sTempSeq(), m_iNbSequences(-1)
{

}

TargetFinder::TargetFinder(TargetFinder const& _toCopy): m_pMiRNA(_toCopy.m_pMiRNA), m_iNumber_miRNA(_toCopy.m_iNumber_miRNA),
    m_vTarget(_toCopy.m_vTarget), m_lTotalTargetNumber(_toCopy.m_lTotalTargetNumber), m_lTotalPairNumber(_toCopy.m_lTotalPairNumber),
    m_dPositionWeight(_toCopy.m_dPositionWeight), m_dPositionWeightGaps(_toCopy.m_dPositionWeightGaps), m_dK(_toCopy.m_dK),
   m_sTempSeq(_toCopy.m_sTempSeq), m_iNbSequences(_toCopy.m_iNbSequences)
{}

TargetFinder::~TargetFinder()
{
    for (int i = (int)m_sTempSeq.size() - 1; i >= 0; i--)
	{
		delete m_sTempSeq[i];
		m_sTempSeq.pop_back();
	}
    for (int i = (int)m_pMiRNA.size() - 1; i >= 0; i--)
	{
		delete m_pMiRNA[i];
		m_pMiRNA.pop_back();
	}
}

bool TargetFinder::estEgal(TargetFinder const& b) const
{
    return (m_pMiRNA == b.m_pMiRNA && m_iNumber_miRNA == b.m_iNumber_miRNA && m_vTarget==b.m_vTarget &&
            m_lTotalTargetNumber == b.m_lTotalTargetNumber && m_lTotalPairNumber == b.m_lTotalPairNumber &&
            m_dPositionWeight == b.m_dPositionWeight && m_dPositionWeightGaps == b.m_dPositionWeightGaps &&
            m_dK == b.m_dK && m_sTempSeq==b.m_sTempSeq);
}

bool operator==(TargetFinder const& a, TargetFinder const& b)
{
    return a.estEgal(b);
}

bool operator!=(TargetFinder const& a, TargetFinder const& b)
{
    return !(a==b);
}

int TargetFinder::getNbSequences(string const& fileName)
{
    if(m_iNbSequences==-1)
    {
        m_iNbSequences=countSequences(fileName);
    }
    return m_iNbSequences;
}

int TargetFinder::countSequences(string const& inputFileName) const
{
    string line;
    ifstream inputFile(inputFileName);
    int cpt(0);

    if (inputFile)
    {
        // read file
        while (getline(inputFile, line))
        {
             if(line[0]==Settings::getSettings()->getLoad_charBeginHeaderLine())
                cpt++;
        }
    }
    else
    {
        throw ERROR_MESSAGE_NO_OPEN_INPUT_FILE;
    }
    return cpt;
}

std::vector<miRNA_data*> TargetFinder::getMiRNA() const
{
    return m_pMiRNA;
}

bool TargetFinder::getMiRNAs(string const& inputFileName)
{
	error_check_miRNA_seq error_res;
	string l_sTemp;
	ifstream inputFile(inputFileName);
	string line;
	miRNA_data * miRNAData(0);
	bool l_bIsEnd(false);

	long l_iNumber_miRNA(0);							//--------------- COUNTER ?

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

		while (!l_bIsEnd)
		{
			getline(inputFile, line);
			l_bIsEnd = !inputFile.good();
            if (line[0] != Settings::getSettings()->getLoad_charBeginHeaderLine())
			{
				//not a header
				if (l_iNumber_miRNA > 0)
				{
					miRNAData->miRNA += line;
				}
				else
				{
					throw ERROR_MESSAGE_NO_HEADER;
				}
			}
            if (line[0] == Settings::getSettings()->getLoad_charBeginHeaderLine() || l_bIsEnd)
			{
				//is there a sequence in miRNAData->miRNA?
				if (l_iNumber_miRNA > 0)
				{
					//we have already written at least 1 complete sequence. Checking it
					error_res = TargetFinder::check_miRNA_string(miRNAData->miRNA);//---------------ASSUMING ONE sequence per line
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
						//good sequence
						miRNAData->miRNA = TargetFinder::reverseSequence(miRNAData->miRNA, true);
						m_pMiRNA.push_back(miRNAData);	//---------------SAVING IN MEMORY
					}
				}
			}
            if (line[0] == Settings::getSettings()->getLoad_charBeginHeaderLine())
			{
				//header line
				l_iNumber_miRNA++;
				//l_iPos1 = line.find("-");
				//l_iPos2 = line.find(" ");
				//if (l_iPos1 < line.length() && l_iPos2 < line.length())
				//{
				//m_pMiRNA[l_iNumber_miRNA].name = line.substr(l_iPos1+1,l_iPos2-l_iPos1-1);
				//}
				miRNAData = new miRNA_data();
				miRNAData->name = line.substr(1);
			}
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
    size_t length = p_pStr.length();

	while (p_pStr.find(' ') < p_pStr.length()) //erasing the spaces
	{
		p_pStr.erase(p_pStr.find(' '));
	}

    if (length < (unsigned int)Settings::getSettings()->getLoad_minMiRNALength() || length > (unsigned int)Settings::getSettings()->getLoad_maxMiRNALength())
	{
		return error_check_miRNA_seq::ERROR_BAD_SEQ_LENGTH;
	}

	p_pStr = utils::toLower(p_pStr);

    for (unsigned int i = 0; i < length; i++)
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
    size_t length = strToReverse.length();
	string temp;

	strToReverse = utils::toLower(strToReverse);
    for (int i = (int)length - 1; i >= 0; i--) //reversing the whole sequence
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
	//int l_ilength;
	m_lTotalTargetNumber = 0;
	m_lTotalPairNumber = 0;
    size_t l_iPos0, l_iPos1, l_iPos2, l_iPos3, l_iPos4;
	ifstream inputFile(inputFileName);
	Target l_oTempTarget;
	Gene_Sequence l_oTempSequence;
	string l_sSequence;
	//Gene_Sequence m_oGene;
    int cpt(0);

	if (inputFile)
	{
		if (m_file)
		{
			m_file.close();
		}
        m_file.open(inputFileName.substr(0,inputFileName.length()-utils::getExtension(inputFileName).length())+ CONST_DEFAULT_SUFFIX+CONST_DEFAULT_EXTENSION);

		if (m_file)
		{
			// read file
			string l_sStr("");
            size_t l_iLength(0);
			while (l_bIsNotEnd)
			{
				getline(inputFile, line);
				l_bIsNotEnd = inputFile.good();
				l_bIsHeader = false;
                l_bIsHeader = line[0] == Settings::getSettings()->getLoad_charBeginHeaderLine();
                if(!l_bIsHeader || (!l_bIsNotEnd && l_sSequence!=""))
                {
                    l_sSequence += line;	//adding the non header line to m_oGene.m_sSequence
                }

				if (l_bIsHeader || !l_bIsNotEnd)
				{
                    if (l_sSequence != "")
                    {
                        cpt++;
						l_sSequence = utils::toLower(l_sSequence);	//lowering the sequence
						m_sTempSeq.push_back(new string(l_sSequence));
						for (int i = 0; i < m_iNumber_miRNA; i++)					//for each miRNA...
						{
                            //cout << endl << "------------------------------------------------" << endl;
                            //cout << "MIRNA " << i + 1 << "/" << m_iNumber_miRNA;
                            //cout << endl << "------------------------------------------------" << endl;
							l_iLength = m_pMiRNA[i]->miRNA.length();
							//					l_sStr = "miRNA name = ";
							//					l_sStr += (m_pMiRNA+i)->name;
							//					l_sStr += '\n';	
							//					l_sStr += '\n';	
							//					m_file.Write(l_sStr, l_sStr.GetLength());
							m_dK = TargetFinder::getKFromMiRNALength(l_iLength);

                            NormilizePositionWeight((int)l_iLength);
                            NormilizePositionWeightGaps((int)l_iLength);

							l_oTempTarget.setMiRNA(m_pMiRNA[i]);
							//l_oTempTarget.setMiRNA(m_)
							if (l_oTempSequence.m_sSign == '+')
							{
								l_oTempSequence.m_sSequence = m_sTempSeq[m_sTempSeq.size() - 1];
								l_oTempTarget.setGene(l_oTempSequence);
                                //for (double j = CONST_MINIMUM_CUTOFF; j <= CONST_MAXIMUM_CUTOFF; j += CONST_STEP_CUTOFF)
                                //{
                                    //cout << endl << endl << "CUTOFF : " << CONST_MINIMUM_CUTOFF << endl;
                                    findTarget(l_oTempTarget, true, Settings::getSettings()->getResults_minimumCutoff()*Settings::getSettings()->getFind_maximumScore());//naum
                                //}
							}
							else if (l_oTempSequence.m_sSign == '-')
							{
								revertSequence(m_sTempSeq[m_sTempSeq.size() - 1]);
								l_oTempSequence.m_sSequence = m_sTempSeq[m_sTempSeq.size() - 1];
								l_oTempTarget.setGene(l_oTempSequence);
                                //for (double j = CONST_MINIMUM_CUTOFF; j <= CONST_MAXIMUM_CUTOFF; j += CONST_STEP_CUTOFF)
                                //{
                                    //cout << endl << endl << "CUTOFF (neg) : " << CONST_MINIMUM_CUTOFF << endl;
                                    findTarget(l_oTempTarget, false, Settings::getSettings()->getResults_minimumCutoff()*Settings::getSettings()->getFind_maximumScore());
                                //}
							}
							else
							{
								throw ERROR_MESSAGE_BAD_HEADER_FORMAT;
							}
						}
						l_sSequence = "";
						l_oTempSequence.m_sSequence = 0;
						l_oTempSequence.m_sChrNumber = "";
						l_oTempSequence.m_sGroupNumber = "";
                        l_oTempSequence.m_sSign = Settings::getSettings()->getFind_defaultSign();
                        emit signalProgress(cpt);
                        //SetDlgItemInt(IDC_NUMBER, m_lTotalTargetNumber);			//--------------- ???
						//SetDlgItemInt(IDC_PAIR_NUMBER, m_lTotalPairNumber);		//--------------- ???
					}
					if (l_bIsHeader)  //in order to do the last one
					{
						//>ENSG00000134516 range=chr19:44430527-44430726 5'pad=100 3'pad=100 strand=+ repeatMasking=none
						l_iPos0 = line.find(" ");
						l_iPos1 = line.find("=");
						l_iPos2 = line.find(":");
						l_iPos3 = line.find("-");
						l_iPos4 = line.find("nd=");
						l_oTempSequence.m_sGroupNumber = (l_iPos0 < line.length()) ? line.substr(1, l_iPos0) : "";									//ENSG00000134516
						l_oTempSequence.m_sChrNumber = (l_iPos1 < line.length() && l_iPos2 < line.length()) ? line.substr(l_iPos1 + 1, l_iPos2 - l_iPos1 - 1) : "";			//chr19
						l_oTempSequence.m_sStartPosition = (l_iPos3 < line.length() && l_iPos2 < line.length()) ? line.substr(l_iPos2 + 1, l_iPos3 - l_iPos2 - 1) : "";		//44430527
                        l_oTempSequence.m_sSign = (l_iPos4 < line.length()) ? line.substr(l_iPos4 + 3, 1)[0] : Settings::getSettings()->getFind_defaultSign();									//+
					}
                }
			}//end while

			l_sStr = "total number of targets = " + utils::convertToString<long>(m_lTotalTargetNumber);
			l_sStr += '\n';
			m_file << l_sStr;
			l_sStr = "total number of target pairs = " + utils::convertToString<long>(m_lTotalPairNumber);
			l_sStr += '\n';
			l_sStr += '\n';
			m_file << l_sStr;
			l_sSequence = "";
			l_oTempSequence.m_sSequence = 0;
			l_oTempSequence.m_sChrNumber = "";
			l_oTempSequence.m_sGroupNumber = "";
			l_oTempSequence.m_sStartPosition = "";
            l_oTempSequence.m_sSign = Settings::getSettings()->getFind_defaultSign();
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


double TargetFinder::getKFromMiRNALength(size_t length)
{
	double K;
	switch (length)
	{
	case 16: K = 1.0177; break;
	case 17: K = 1.0177; break;
	case 18: K = 1.0177; break;
	case 19: K = 1.0177; break;
	case 20: K = 1.0177; break;
	case 21: K = 1.0177; break;
	case 22: K = 0.9818; break;
    case 23: K = 0.9634; break;
    case 24: K = 0.9634; break;
	case 25: K = 0.9634; break;
	case 26: K = 0.9634; break;
	case 27: K = 0.9634; break;
    default: K = Settings::getSettings()->getFind_defaultKValue(); break;
	}
	return K;
}


//all p_iLength was -1 before
void TargetFinder::NormilizePositionWeight(int p_iLength)
{
	m_dPositionWeight.clear();
	for (int in = 0; in < p_iLength; in++)
	{
		m_dPositionWeight.push_back(WEIGHT[in]);
	}

	double l_dMin(10000);

	for (int in = 0; in < p_iLength; in++)
	{
		if (l_dMin > m_dPositionWeight[in])
		{
			l_dMin = m_dPositionWeight[in];
		}
	}

	double l_dKoef(0.75); // 0 =< l_dKoef < 1 

	for (int in = 0; in < p_iLength; in++)
	{
		m_dPositionWeight[in] = m_dPositionWeight[in] - l_dKoef*l_dMin;
	}

	double l_dTotal(0.0);

	for (int in = 0; in < p_iLength; in++)
	{
		l_dTotal += m_dPositionWeight[in];
	}

	for (int in = 0; in < p_iLength; in++)
	{
		m_dPositionWeight[in] /= l_dTotal;
		m_dPositionWeight[in] *= p_iLength;
	}
}

//all p_iLength was -1 before
void TargetFinder::NormilizePositionWeightGaps(int p_iLength)
{
	m_dPositionWeightGaps.clear();
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
	vector<Target> targetVector;
	string l_sScore;

	//don't need to loop, we do everything in one shot!
	targetVector = smithWaterman(p_oTarget, p_dCutoff);
	//l_dScore = smithWaterman(p_oTarget, p_dCutoff);
	for (unsigned int i = 0; i < targetVector.size(); i++)
	{
		//l_dScore is greater than the cutoff : good finding
		//p_oTarget.getMiRNA().incNumber();// m_pMiRNA[p_lMiRNA_number].number++;
		writeName(*targetVector[i].getMiRNA());		//writing the miRNA name in the output file
		if (i > 1)
		{
			//------------------------------ DOUBLE
            if (targetVector[i].getPositionStart() - targetVector[i - 1].getPositionEnd() < Settings::getSettings()->getFind_maxDistanceBetweenDouble()
				&& 
				targetVector[i].getPositionStart() - targetVector[i - 1].getPositionEnd() > 0)
			{
				targetVector[i].getMiRNA()->pairs++;
				m_lTotalPairNumber++;
                writeFoundDoubleAt(targetVector[i].getPositionStart() - targetVector[i - 1].getPositionEnd());	//writing the found double in the output file
			}
			//------------------------------ /DOUBLE
		}
		m_lTotalTargetNumber++;

		writeInfo(p_oTarget.getGene(), p_bStraitgh, targetVector[i].getPositionStart());
		//m_targets[m_lTotalTargetNumber - 1]
		//p_oTarget.setScore(l_dScore);

		l_sScore = " ";
		if (p_bStraitgh == true)
		{
			l_sScore += " +";
		}
		else
		{
			l_sScore += " -";
		}
		writeRes(targetVector[i]);		//writing the res into the output file

		l_sScore = "Score = ";
        //l_sScore += _fcvt_s(l_dScore / const_dIdealScore2, DEFAULT_SCORE_DECIMAL_NUMBER, &dec, &sign);
        l_sScore += utils::convertToString<double>(targetVector[i].getScoreUnderOne());
		//l_sScore += ".";
		//l_sScore += utils::convertToString<int>(dec);
		l_sScore += '\n';
		writeString(l_sScore, false);
        //targetVector[i].display();
	}
	m_vTarget.insert(m_vTarget.end(), targetVector.begin(), targetVector.end());	//adding the current targetVector in the end of m_vTarget
}

void TargetFinder::writeName(miRNA_data data)
{
	string l_sScore("miRNA name = ");
	l_sScore += data.name;
	l_sScore += '\n';
	writeString(l_sScore, false);
}

void TargetFinder::writeFoundDoubleAt(long position)
{
	string l_sScore("Found double at distance ");
	l_sScore += utils::convertToString<long>(position);
	l_sScore += " ";
	writeString(l_sScore);
}

void TargetFinder::writeInfo(Gene_Sequence const& p_oGene)
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
	l_sScore += utils::convertToString<long>(utils::convertStringToNumber<long>(p_oGene.m_sStartPosition) + value);
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
	l_sScore += "   "+ p_oTarget.getRes3_long();
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

vector<Target> TargetFinder::smithWaterman(Target & p_oTarget, double p_dCutoff)
{
	//p_s2 : miRNA
	//p_s1 : sequence
	//p_iLength : length of p_s1
	Target currentTarget(p_oTarget);	//the current target
	vector<Target> resVector;			//the vector containing all the found targets

	//	Check_string(p_s1, true);
    //int l_iscore = 0;
	string s2 = currentTarget.getMiRNA()->miRNA;

    size_t l_lenS1 = currentTarget.getGene().m_sSequence->length(); // first sequence
    size_t lenS2 = s2.length(); // second sequence (miRNA)
    size_t lenS1 = l_lenS1; //---------------------- pas sur
	int scoreZero = 0;
	string s1;
	double l_dScoreMax = 0;
	int bestI = 0;
	int bestJ = 0;
	int bestScoreMatrix = scoreZero;
    int l_iGapPenalty1 = Settings::getSettings()->getFind_gapPenalty1();
    int l_iGapPenalty2 = Settings::getSettings()->getFind_gapPenalty2();
	int scoreIfGoingDown = 0;
	int scoreIfGoingRight = 0;
	int scoreIfGoingDown2 = 0;
	int scoreIfGoingRight2 = 0;
	int scoreIfGoingDownRight = 0;
	int bestScore = 0;
    int add(0); //the amount we add to each weight matrix each turn
	//int ind1 = 0;
	//int ind2 = 0;
	// based on number of letters we create a matrix
	vector<vector<int>> weightMatrix;	//matrix of (lenS1 + 1) lines and (lenS2 + 1) columns
	vector<vector<int>> gapMatrix;		//matrix of (lenS1 + 1)*(lenS2 + 1) elements
	vector<int> temp1, temp2;

	//------------
	string res1, res1t;
	string res2, res2t;

	//		m_lPosition_End = bestI;
	int ii;
	int jj;
    size_t pos1;
    size_t pos2;
    //------------
	bool l_bShouldExitLoop(false);

	//for (int iStr = 0; iStr <= l_lenS1 - p_oTarget.getSequence().length(); iStr++)
	//for (unsigned int iStr = 0; iStr <= l_lenS1 - p_oTarget.getMiRNA().miRNA.length(); iStr ++)
	//{
	//s1 = p_oTarget.getSequence().substr(iStr, p_oTarget.getSequence().length()); 
	s1 = currentTarget.getGene().m_sSequence->substr();
	lenS1 = s1.length();

	//fill out first row and column by 0 (actually the whole matrix)	
	temp1.clear();
	temp2.clear();
	weightMatrix.clear();
	gapMatrix.clear();
    for (unsigned int j = 0; j <= lenS2; j++)
	{
		//weightMatrix[lenS1*j + i] = scoreZero;
		temp1.push_back(scoreZero);
		//gapMatrix[lenS1*j + i] = 0;
		temp2.push_back(0);
	}
    for (unsigned int i = 0; i <= lenS1; i++)
	{
		weightMatrix.push_back(temp1);
		gapMatrix.push_back(temp2);
	}

	//we browse the lenS1 (char by char?)
    for (unsigned int i = 1; i < lenS1; i++)
	{
		//strcpy(char1, s1.Mid(i - 1, 1));
		//strcpy(char12, s1.Mid(i, 1));

        for (unsigned int j = 1; j < lenS2; j++)
		{
			//strcpy(char2, s2.Mid(j - 1, 1));
			//strcpy(char22, s2.Mid(j, 1));
			//ind1 = tabLetterIndex[s1[i - 1]][s1[i]];
			//ind2 = tabLetterIndex[s2[j - 1]][s2[j]];

			if (gapMatrix[i - 1][j] != 1)
			{
                add=(int)(m_dPositionWeightGaps[lenS2 - 1 - j] * l_iGapPenalty1);
                scoreIfGoingDown = weightMatrix[i - 1][j] + add;
			}
			else
			{
                add =(int)(m_dPositionWeightGaps[lenS2 - 1 - j] * l_iGapPenalty2);
                scoreIfGoingDown = weightMatrix[i - 1][j] + add;
			}
			if (gapMatrix[i][j - 1] != 2)
			{
                add=(int)(m_dPositionWeightGaps[lenS2 - j] * l_iGapPenalty1); //bug
                scoreIfGoingRight = weightMatrix[i][j - 1] + add;
			}
			else
			{
                add= (int)(m_dPositionWeightGaps[lenS2 - j] * l_iGapPenalty2);
                scoreIfGoingRight = weightMatrix[i][j - 1] + add;
			}

			//scoreIfGoingDownRight = weightMatrix[i - 1][j - 1] + (int)(974.228 * m_dPositionWeight[lenS2 - 1 - j] * const_costMatrix[ind1][ind2]); //["AA"]["CG"]
            scoreIfGoingDownRight = weightMatrix[i - 1][j - 1] + (int)(974.228*m_dPositionWeight[lenS2 - 1 - j] * const_costMatrix2[TargetFinder::changeTinU(s1.substr(i - 1, 2))][TargetFinder::changeTinU(s2.substr(j - 1, 2))]);
            //scoreIfGoingDownRight = weightMatrix[i - 1][j - 1] + (int)(963.4*m_dPositionWeight[lenS2 - 1 - j] * const_costMatrix2[TargetFinder::changeTinU(s1.substr(i - 1, 2))][TargetFinder::changeTinU(s2.substr(j - 1, 2))]);
            scoreIfGoingDownRight = weightMatrix[i - 1][j - 1] + (int)(1000*m_dPositionWeight[lenS2 - 1 - j] * const_costMatrix2[TargetFinder::changeTinU(s1.substr(i - 1, 2))][TargetFinder::changeTinU(s2.substr(j - 1, 2))]);

            //supposed to be scoreIfGoingDownRight = ...(int)( 1000	* m_dPositionWeight[lenS2 - 1 - j] * ...; //["AA"]["CG"]

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
            if(i<=8&&j<=8&&max(max(scoreIfGoingDown, scoreIfGoingRight),max(scoreIfGoingDownRight, scoreZero))==scoreIfGoingDownRight)
            {
                weightMatrix[i][j]+=(int)(974.228*m_dPositionWeight[lenS2 - 1 - j] * const_costMatrix2[TargetFinder::changeTinU(s1.substr(i - 1, 2))][TargetFinder::changeTinU(s2.substr(j - 1, 2))]);
            }else if(i<=8&&j<=8)
            {
                weightMatrix[i][j]+=add;
            }

            if (bestScore / (double)(lenS2 - 1) / 100.0 > m_dK*p_dCutoff)
			{
				l_dScoreMax = bestScore;// / (double)(lenS2 - 1) / 100.0;
				bestScoreMatrix = bestScore;
				bestI = i;
				bestJ = j;
				ii = bestI;
				jj = bestJ;
				res1 = "";
				res1t = "";
				res2 = "";
				res2t = "";
				l_bShouldExitLoop = false;
				while (!l_bShouldExitLoop)
				{
					//ind1 = tabLetterIndex[s1[ii - 1]][s1[ii]];	//------------------------ important
					//ind2 = tabLetterIndex[s2[jj - 1]][s2[jj]];	//------------------------ important

					scoreIfGoingDown = weightMatrix[ii - 1][jj] + (int)(m_dPositionWeightGaps[lenS2 - 1 - jj] * l_iGapPenalty1);
					scoreIfGoingDown2 = weightMatrix[ii - 1][jj] + (int)(m_dPositionWeightGaps[lenS2 - 1 - jj] * l_iGapPenalty2);		
                    scoreIfGoingRight = weightMatrix[ii][jj - 1] + (int)(m_dPositionWeightGaps[lenS2 - jj] * l_iGapPenalty1);
					scoreIfGoingRight2 = weightMatrix[ii][jj - 1] + (int)(m_dPositionWeightGaps[lenS2 - jj] * l_iGapPenalty2);

					if (weightMatrix[ii][jj] == scoreIfGoingDown || weightMatrix[ii][jj] == scoreIfGoingDown2)
					{
                       /// if(ii<=8&&jj<=8)
                       ///     l_dScoreMax+=(weightMatrix[ii][jj] == scoreIfGoingDown)?scoreIfGoingDown:scoreIfGoingDown2;
						// Go up !
						res1.insert(0, s1.substr(ii - 1, 1));
						res1t.insert(0, s1.substr(ii - 1, 1));
						res2.insert(0, "-");
						ii--;
					}
					else if (weightMatrix[ii][jj] == scoreIfGoingRight || weightMatrix[ii][jj] == scoreIfGoingRight2)
                    {
                       /// if(ii<=8&&jj<=8)
                       ///     l_dScoreMax+=(weightMatrix[ii][jj] == scoreIfGoingRight)?scoreIfGoingRight:scoreIfGoingRight2;

						// Go left !
						res1.insert(0, "-");
						res2.insert(0, s2.substr(jj - 1, 1));
						res2t.insert(0, s2.substr(jj - 1, 1));
						jj--;
					}
					else
					{
                       /// if(ii<=8&&jj<=8)
                        ///    l_dScoreMax+= weightMatrix[ii - 1][jj - 1] + (int)(974.228*m_dPositionWeight[lenS2 - 1 - jj] * const_costMatrix2[TargetFinder::changeTinU(s1.substr(ii - 1, 2))][TargetFinder::changeTinU(s2.substr(jj - 1, 2))]);
						// Go up and left !
						res1.insert(0, s1.substr(ii - 1, 1));
						res2.insert(0, s2.substr(jj - 1, 1));
						res1t.insert(0, s1.substr(ii - 1, 1));
						res2t.insert(0, s2.substr(jj - 1, 1));
						ii--;
						jj--;
					}
                    /*if (ii > 0 && jj > 0)
					{
						l_bShouldExitLoop = weightMatrix[ii][jj] == scoreZero;
					}
					else
					{
						l_bShouldExitLoop = true;
                    }*/ //more easily :
                    l_bShouldExitLoop = !(ii>0 && jj>0) || (weightMatrix[ii][jj] == scoreZero);
				}
				while (ii > 0)
                {
					if (weightMatrix[ii][jj] == scoreZero)
					{
						break;
					}
                    ///if(ii<=8&&jj<=8)
                    ///    l_dScoreMax+=(weightMatrix[ii][jj] == scoreIfGoingDown)?scoreIfGoingDown:scoreIfGoingDown2;

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
                    ///if(ii<=8&&jj<=8)
                   ///     l_dScoreMax+=(weightMatrix[ii][jj] == scoreIfGoingRight)?scoreIfGoingRight:scoreIfGoingRight2;

					// Go left !
					res1.insert(0, "-");
					res2.insert(0, s2.substr(jj - 1, 1));
					res2t.insert(0, s2.substr(ii - 1, 1));
					jj--;
                }
				pos1 = s1.find(res1t);
				pos2 = s2.find(res2t);
				res1 += s1.substr(pos1 + res1t.length(), 1);
				res2 += s2.substr(pos2 + res2t.length(), 1);
				if (pos2 < s2.length() && pos2 > 0)
				{
                    if (pos1>0 && pos1 < s1.length() && pos2 > 0)
					{
						res1.insert(0, s1.substr(pos1 - 1, 1));
					}
					else
					{
						res1.insert(0, " ");
					}
					res2.insert(0, s2.substr(pos2 - 1, 1));
				}
				currentTarget.getMiRNA()->pairs++;
				currentTarget.setPositionStart(ii);
                currentTarget.setPositionEnd(currentTarget.getPositionStart() + (long)res1.length());
				
				currentTarget.setRes1_long("5' " + res1 + " 3'");
				currentTarget.setRes2_long("3' " + res2 + " 5'");
				currentTarget.setScore(l_dScoreMax / ((double)(lenS2 - 1)) / 100.0);

				string res3;
				for (unsigned int i = 0; i < res1.length(); i++)
				{
					res3 += tabLetterCorresp[res1[i]][res2[i]];		//------------------------ important
				}
				currentTarget.setRes3_long(res3);
				TargetFinder::erasePrevious(currentTarget, resVector);	//checking if the previous one isn't the same that the current (but shorter)
				resVector.push_back(currentTarget);					//------------------------ adding the target to the vector
			}
		}
	}
	//	return l_dScoreMax / ((double)(lenS2-1)) / 100.0;
	return resVector;
}

void TargetFinder::revertSequence(string * p_pStr)
{
	string temp;
    for (size_t i = p_pStr->length() - 1; i >= 0; i--)
	{
		switch ((*p_pStr)[i])
		{
		case('a') :
			//			case('A'):
			temp += 't';
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
	*p_pStr = temp;
}


bool TargetFinder::erasePrevious(Target const& p_oCurrentTarget, vector<Target> & p_oVector)
{
	bool l_bRes(false);
    /*if (p_oVector.size() > 0)
    /{
		if (p_oVector[p_oVector.size() - 1].getPositionStart() == p_oCurrentTarget.getPositionStart()	//if the start position is the same
			&&
			p_oVector[p_oVector.size() - 1].getPositionEnd() < p_oCurrentTarget.getPositionEnd()		//AND the end position is lower
			&&
			p_oVector[p_oVector.size()-1].getScore()<p_oCurrentTarget.getScore())
		{
			p_oVector.pop_back();	//we erase the last one
			l_bRes = true;
		}
    }*/
    unsigned int i=0;
    while(i<p_oVector.size()&&!l_bRes)
    {
        if (((p_oVector[i].getPositionStart() == p_oCurrentTarget.getPositionStart()	//if the start position is the same
            &&
            p_oVector[i].getPositionEnd() < p_oCurrentTarget.getPositionEnd())		//AND the end position is lower
                ||                                                              //OR
             (p_oVector[i].getPositionStart() < p_oCurrentTarget.getPositionStart()     //if the start position is lower
            &&
            p_oVector[i].getPositionEnd() == p_oCurrentTarget.getPositionEnd()))    //AND the end position is the same
            &&
            p_oVector[i].getScore()<p_oCurrentTarget.getScore())        //AND, anyway, the old score is lower than the current
        {
            p_oVector.erase(p_oVector.begin()+i);	//we erase the wrong one
            l_bRes = true;
        }
        i++;
    }
	return l_bRes;
}

string TargetFinder::changeTinU(string const& input)
{
	string res("");
	for (unsigned int i = 0; i < input.length(); i++)
	{
		if (input[i] == 't')
		{
			res += 'u';
		}
		else
		{
			res += input[i];
		}
	}
	return res;
}


vector<Target> TargetFinder::getTargets() const
{
    return m_vTarget;
}

std::map<std::string, std::map<std::string, double>> TargetFinder::const_costMatrix2({
	//				aa				ag				ac				au				ga				gg				gc				gu				ca				cg				cc			cu			ua				ug				uc      uu,						a-			g-			c-				u-			-a				–g				–c			-u		--},
	/* 1aa*/{ "aa",	{ { "aa", -10 },	{ "ag", -10 },	{ "ac", -10 },	{ "au", 6.4 },	{ "ga", -10 },	{ "gg", -10 },	{ "gc", -10 },	{ "gu", 6.4 },	{ "ca", -10 },	{ "cg", -10 },	{ "cc", -10 },	{ "cu", 6.4 },	{ "ua", 6.4 },	{ "ug", 6.4 },	{ "uc", 6.4 },	{ "uu", 22 },	{ "a-", -10 },	{ "g-", -10 },	{ "c-", -10 },	{ "u-", 6.4 },	{ "-a", -10 },	{ "-g", -10 },	{ "-c", -10 },	{ "-u", 6.4 },	{ "--", -10 } } },
	/* 2ag*/{ "ag",	{ { "aa", -10 },	{ "ag", -10 },	{ "ac", 6.4 },	{ "au", 3 },	{ "ga", -10 },	{ "gg", -10 },	{ "gc", 6.4 },	{ "gu", 3 },	{ "ca", -10 },	{ "cg", -10 },	{ "cc", 6.4 },	{ "cu", 3 },	{ "ua", 6.4 },	{ "ug", 6.4 },	{ "uc", 22 },	{ "uu", 9.9 },	{ "a-", -10 },	{ "g-", -10 },	{ "c-", -10 },	{ "u-", 6.4 },	{ "-a", -10 },	{ "-g", -10 },	{ "-c", 6.4 },	{ "-u", 3 },	{ "--", -10 } } },
	/* 3ac*/{ "ac",	{ { "aa", -10 },	{ "ag", 6.4 },	{ "ac", -10 },	{ "au", -10 },	{ "ga", -10 },	{ "gg", 6.4 },	{ "gc", -10 },	{ "gu", -10 },	{ "ca", -10 },	{ "cg", 6.4 },	{ "cc", -10 },	{ "cu", -10 },	{ "ua", 6.4 },	{ "ug", 22 },	{ "uc", 6.4 },	{ "uu", 6.4 },	{ "a-", -10 },	{ "g-", -10 },	{ "c-", -10 },	{ "u-", 6.4 },	{ "-a", -10 },	{ "-g", 6.4 },	{ "-c", -10 },	{ "-u", -10 },	{ "--", -10 } } },
	/* 4au*/{ "au",	{ { "aa", 6.4 },	{ "ag", 3 },	{ "ac", -10 },	{ "au", -10 },	{ "ga", 6.4 },	{ "gg", 3 },	{ "gc", -10 },	{ "gu", -10 },	{ "ca", 6.4 },	{ "cg", 3 },	{ "cc", -10 },	{ "cu", -10 },	{ "ua", 22 },	{ "ug", 9.9 },	{ "uc", 6.4 },	{ "uu", 6.4 },	{ "a-", -10 },	{ "g-", -10 },	{ "c-", -10 },	{ "u-", 6.4 },	{ "-a", 6.4 },	{ "-g", 3 },	{ "-c", -10 },	{ "-u", -10 },	{ "--", -10 } } },
	/* 5ga*/{ "ga",	{ { "aa", -10 },	{ "ag", -10 },	{ "ac", -10 },	{ "au", 6.4 },	{ "ga", -10 },	{ "gg", -10 },	{ "gc", -10 },	{ "gu", 6.4 },	{ "ca", 6.4 },	{ "cg", 6.4 },	{ "cc", 6.4 },	{ "cu", 22 },	{ "ua", 3 },	{ "ug", 3 },	{ "uc", 3 },	{ "uu", 9.9 },	{ "a-", -10 },	{ "g-", -10 },	{ "c-", 6.4 },	{ "u-", 3 },	{ "-a", -10 },	{ "-g", -10 },	{ "-c", -10 },	{ "-u", 6.4 },	{ "--", -10 } } },
	/* 6gg*/{ "gg",	{ { "aa", -10 },	{ "ag", -10 },	{ "ac", 6.4 },	{ "au", 3 },	{ "ga", -10 },	{ "gg", -10 },	{ "gc", 6.4 },	{ "gu", 3 },	{ "ca", 6.4 },	{ "cg", 6.4 },	{ "cc", 22 },	{ "cu", 9.9 },	{ "ua", 3 },	{ "ug", 3 },	{ "uc", 9.9 },	{ "uu", 6 },	{ "a-", -10 },	{ "g-", -10 },	{ "c-", 6.4 },	{ "u-", 3 },	{ "-a", -10 },	{ "-g", -10 },	{ "-c", 6.4 },	{ "-u", -10 },	{ "--", -10 } } },
	/* 7gc*/{ "gc",	{ { "aa", -10 },	{ "ag", 6.4 },	{ "ac", -10 },	{ "au", -10 },	{ "ga", -10 },	{ "gg", 6.4 },	{ "gc", -10 },	{ "gu", -10 },	{ "ca", 6.4 },	{ "cg", 22 },	{ "cc", 6.4 },	{ "cu", 6.4 },	{ "ua", 3 },	{ "ug", 9.9 },	{ "uc", 3 },	{ "uu", 3 },	{ "a-", -10 },	{ "g-", -10 },	{ "c-", 6.4 },	{ "u-", 3 },	{ "-a", -10 },	{ "-g", 6.4 },	{ "-c", -10 },	{ "-u", -10 },	{ "--", -10 } } },
	/* 8gu*/{ "gu",	{ { "aa", 6.4 },	{ "ag", 3 },	{ "ac", -10 },	{ "au", -10 },	{ "ga", 6.4 },	{ "gg", 3 },	{ "gc", -10 },	{ "gu", -10 },	{ "ca", 22 },	{ "cg", 9.9 },	{ "cc", 6.4 },	{ "cu", 6.4 },	{ "ua", 9.9 },	{ "ug", 6 },	{ "uc", 3 },	{ "uu", 3 },	{ "a-", -10 },	{ "g-", -10 },	{ "c-", 6.4 },	{ "u-", 3 },	{ "-a", 6.4 },	{ "-g", -10 },	{ "-c", -10 },	{ "-u", -10 },	{ "--", -10 } } },
	/* 9ca*/{ "ca",	{ { "aa", -10 },	{ "ag", -10 },	{ "ac", -10 },	{ "au", 6.4 },	{ "ga", 6.4 },	{ "gg", 6.4 },	{ "gc", 6.4 },	{ "gu", 22 },	{ "ca", -10 },	{ "cg", -10 },	{ "cc", -10 },	{ "cu", 6.4 },	{ "ua", -10 },	{ "ug", -10 },	{ "uc", -10 },	{ "uu", 6.4 },	{ "a-", -10 },	{ "g-", 6.4 },	{ "c-", -10 },	{ "u-", -10 },	{ "-a", -10 },	{ "-g", -10 },	{ "-c", -10 },	{ "-u", 6.4 },	{ "--", -10 } } },
	/*10cg*/{ "cg",	{ { "aa", -10 },	{ "ag", -10 },	{ "ac", 6.4 },	{ "au", 3 },	{ "ga", 6.4 },	{ "gg", 6.4 },	{ "gc", 22 },	{ "gu", 9.9 },	{ "ca", -10 },	{ "cg", -10 },	{ "cc", 6.4 },	{ "cu", 3 },	{ "ua", -10 },	{ "ug", -10 },	{ "uc", 6.4 },	{ "uu", 3 },	{ "a-", -10 },	{ "g-", 6.4 },	{ "c-", -10 },	{ "u-", -10 },	{ "-a", -10 },	{ "-g", -10 },	{ "-c", 6.4 },	{ "-u", 3 },	{ "--", -10 } } },
	/*11cc*/{ "cc",	{ { "aa", -10 },	{ "ag", 6.4 },	{ "ac", -10 },	{ "au", -10 },	{ "ga", 6.4 },	{ "gg", 22 },	{ "gc", 6.4 },	{ "gu", 6.4 },	{ "ca", -10 },	{ "cg", 6.4 },	{ "cc", -10 },	{ "cu", -10 },	{ "ua", -10 },	{ "ug", 6.4 },	{ "uc", -10 },	{ "uu", -10 },	{ "a-", -10 },	{ "g-", 6.4 },	{ "c-", -10 },	{ "u-", -10 },	{ "-a", -10 },	{ "-g", 6.4 },	{ "-c", -10 },	{ "-u", -10 },	{ "--", -10 } } },
	/*12cu*/{ "cu",	{ { "aa", 6.4 },	{ "ag", 3 },	{ "ac", -10 },	{ "au", -10 },	{ "ga", 22 },	{ "gg", 9.9 },	{ "gc", 6.4 },	{ "gu", 6.4 },	{ "ca", 6.4 },	{ "cg", 3 },	{ "cc", -10 },	{ "cu", -10 },	{ "ua", 6.4 },	{ "ug", 3 },	{ "uc", -10 },	{ "uu", -10 },	{ "a-", -10 },	{ "g-", 6.4 },	{ "c-", -10 },	{ "u-", -10 },	{ "-a", 6.4 },	{ "-g", 3 },	{ "-c", -10 },	{ "-u", -10 },	{ "--", -10 } } },
	/*13ua*/{ "ua",	{ { "aa", 6.4 },	{ "ag", 6.4 },	{ "ac", 6.4 },	{ "au", 22 },	{ "ga", 3 },	{ "gg", 3 },	{ "gc", 3 },	{ "gu", 9.9 },	{ "ca", -10 },	{ "cg", -10 },	{ "cc", -10 },	{ "cu", 6.4 },	{ "ua", -10 },	{ "ug", -10 },	{ "uc", -10 },	{ "uu", 6.4 },	{ "a-", 6.4 },	{ "g-", 3 },	{ "c-", -10 },	{ "u-", -10 },	{ "-a", -10 },	{ "-g", -10 },	{ "-c", -10 },	{ "-u", 6.4 },	{ "--", -10 } } },
	/*14ug*/{ "ug",	{ { "aa", 6.4 },	{ "ag", 6.4 },	{ "ac", 22 },	{ "au", 9.9 },	{ "ga", 3 },	{ "gg", 3 },	{ "gc", 9.9 },	{ "gu", 6 },	{ "ca", -10 },	{ "cg", -10 },	{ "cc", 6.4 },	{ "cu", 3 },	{ "ua", -10 },	{ "ug", -10 },	{ "uc", 6.4 },	{ "uu", 3 },	{ "a-", 6.4 },	{ "g-", 3 },	{ "c-", -10 },	{ "u-", -10 },	{ "-a", -10 },	{ "-g", -10 },	{ "-c", 6.4 },	{ "-u", -10 },	{ "--", -10 } } },
	/*15uc*/{ "uc",	{ { "aa", 6.4 },	{ "ag", 22 },	{ "ac", 6.4 },	{ "au", 6.4 },	{ "ga", 3 },	{ "gg", 9.9 },	{ "gc", 3 },	{ "gu", 3 },	{ "ca", -10 },	{ "cg", 6.4 },	{ "cc", -10 },	{ "cu", -10 },	{ "ua", -10 },	{ "ug", 6.4 },	{ "uc", -10 },	{ "uu", -10 },	{ "a-", 6.4 },	{ "g-", 3 },	{ "c-", -10 },	{ "u-", -10 },	{ "-a", -10 },	{ "-g", 6.4 },	{ "-c", -10 },	{ "-u", -10 },	{ "--", -10 } } },
	/*16uu*/{ "uu",	{ { "aa", 22 },		{ "ag", 9.9 },	{ "ac", 6.4 },	{ "au", 6.4 },	{ "ga", 9.9 },	{ "gg", 6 },	{ "gc", 3 },	{ "gu", 3 },	{ "ca", 6.4 },	{ "cg", 3 },	{ "cc", -10 },	{ "cu", -10 },	{ "ua", 6.4 },	{ "ug", 3 },	{ "uc", -10 },	{ "uu", -10 },	{ "a-", 6.4 },	{ "g-", 3 },	{ "c-", -10 },	{ "u-", -10 },	{ "-a", 6.4 },	{ "-g", -10 },	{ "-c", -10 },	{ "-u", -10 },	{ "--", -10 } } },
	/*07a-*/{ "a-",	{ { "aa", -10 },	{ "ag", -10 },	{ "ac", -10 },	{ "au", -10 },	{ "ga", -10 },	{ "gg", -10 },	{ "gc", -10 },	{ "gu", -10 },	{ "ca", -10 },	{ "cg", -10 },	{ "cc", -10 },	{ "cu", -10 },	{ "ua", 6.4 },	{ "ug", 6.4 },	{ "uc", 6.4 },	{ "uu", 6.4 },	{ "a-", -10 },	{ "g-", -10 },	{ "c-", -10 },	{ "u-", -10 },	{ "-a", -10 },	{ "-g", -10 },	{ "-c", -10 },	{ "-u", -10 },	{ "--", -10 } } },
	/*08g-*/{ "g-",	{ { "aa", -10 },	{ "ag", -10 },	{ "ac", -10 },	{ "au", -10 },	{ "ga", -10 },	{ "gg", -10 },	{ "gc", -10 },	{ "gu", -10 },	{ "ca", 6.4 },	{ "cg", 6.4 },	{ "cc", 6.4 },	{ "cu", 6.4 },	{ "ua", 3 },	{ "ug", 3 },	{ "uc", 3 },	{ "uu", 3 },	{ "a-", -10 },	{ "g-", -10 },	{ "c-", -10 },	{ "u-", -10 },	{ "-a", -10 },	{ "-g", -10 },	{ "-c", -10 },	{ "-u", -10 },	{ "--", -10 } } },
	/*09c-*/{ "c-",	{ { "aa", -10 },	{ "ag", -10 },	{ "ac", -10 },	{ "au", -10 },	{ "ga", 6.4 },	{ "gg", 6.4 },	{ "gc", 6.4 },	{ "gu", 6.4 },	{ "ca", -10 },	{ "cg", -10 },	{ "cc", -10 },	{ "cu", -10 },	{ "ua", -10 },	{ "ug", -10 },	{ "uc", -10 },	{ "uu", -10 },	{ "a-", -10 },	{ "g-", -10 },	{ "c-", -10 },	{ "u-", -10 },	{ "-a", -10 },	{ "-g", -10 },	{ "-c", -10 },	{ "-u", -10 },	{ "--", -10 } } },
	/*20u-*/{ "u-",	{ { "aa", 6.4 },	{ "ag", 6.4 },	{ "ac", 6.4 },	{ "au", 6.4 },	{ "ga", 3 },	{ "gg", 3 },	{ "gc", 3 },	{ "gu", 3 },	{ "ca", -10 },	{ "cg", -10 },	{ "cc", -10 },	{ "cu", -10 },	{ "ua", -10 },	{ "ug", -10 },	{ "uc", -10 },	{ "uu", -10 },	{ "a-", -10 },	{ "g-", -10 },	{ "c-", -10 },	{ "u-", -10 },	{ "-a", -10 },	{ "-g", -10 },	{ "-c", -10 },	{ "-u", -10 },	{ "--", -10 } } },
	/*20-a*/{ "-a",	{ { "aa", -10 },	{ "ag", -10 },	{ "ac", -10 },	{ "au", 6.4 },	{ "ga", -10 },	{ "gg", -10 },	{ "gc", -10 },	{ "gu", 6.4 },	{ "ca", -10 },	{ "cg", -10 },	{ "cc", -10 },	{ "cu", 6.4 },	{ "ua", -10 },	{ "ug", -10 },	{ "uc", -10 },	{ "uu", 6.4 },	{ "a-", -10 },	{ "g-", -10 },	{ "c-", -10 },	{ "u-", -10 },	{ "-a", -10 },	{ "-g", -10 },	{ "-c", -10 },	{ "-u", -10 },	{ "--", -10 } } },
	/*22-g*/{ "-g",	{ { "aa", -10 },	{ "ag", -10 },	{ "ac", 6.4 },	{ "au", 3 },	{ "ga", -10 },	{ "gg", -10 },	{ "gc", 6.4 },	{ "gu", 3 },	{ "ca", -10 },	{ "cg", -10 },	{ "cc", 6.4 },	{ "cu", 3 },	{ "ua", -10 },	{ "ug", -10 },	{ "uc", 6.4 },	{ "uu", 3 },	{ "a-", -10 },	{ "g-", -10 },	{ "c-", -10 },	{ "u-", -10 },	{ "-a", -10 },	{ "-g", -10 },	{ "-c", -10 },	{ "-u", -10 },	{ "--", -10 } } },
	/*23-c*/{ "-c",	{ { "aa", -10 },	{ "ag", 6.4 },	{ "ac", -10 },	{ "au", -10 },	{ "ga", -10 },	{ "gg", 6.4 },	{ "gc", -10 },	{ "gu", -10 },	{ "ca", -10 },	{ "cg", 6.4 },	{ "cc", -10 },	{ "cu", -10 },	{ "ua", -10 },	{ "ug", 6.4 },	{ "uc", -10 },	{ "uu", -10 },	{ "a-", -10 },	{ "g-", -10 },	{ "c-", -10 },	{ "u-", -10 },	{ "-a", -10 },	{ "-g", -10 },	{ "-c", -10 },	{ "-u", -10 },	{ "--", -10 } } },
	/*24-u*/{ "-u",	{ { "aa", 6.4 },	{ "ag", 3 },	{ "ac", -10 },	{ "au", -10 },	{ "ga", 6.4 },	{ "gg", 3 },	{ "gc", -10 },	{ "gu", -10 },	{ "ca", 6.4 },	{ "cg", 3 },	{ "cc", -10 },	{ "cu", -10 },	{ "ua", 6.4 },	{ "ug", 3 },	{ "uc", -10 },	{ "uu", -10 },	{ "a-", -10 },	{ "g-", -10 },	{ "c-", -10 },	{ "u-", -10 },	{ "-a", -10 },	{ "-g", -10 },	{ "-c", -10 },	{ "-u", -10 },	{ "--", -10 } } },
	/*24--*/{ "--",	{ { "aa", -10 },	{ "ag", -10 },	{ "ac", -10 },	{ "au", -10 },	{ "ga", -10 },	{ "gg", -10 },	{ "gc", -10 },	{ "gu", -10 },	{ "ca", -10 },	{ "cg", -10 },	{ "cc", -10 },	{ "cu", -10 },	{ "ua", -10 },	{ "ug", -10 },	{ "uc", -10 },	{ "uu", -10 },	{ "a-", -10 },	{ "g-", -10 },	{ "c-", -10 },	{ "u-", -10 },	{ "-a", -10 },	{ "-g", -10 },	{ "-c", -10 },	{ "-u", -10 },	{ "--", -10 } } } });
	
