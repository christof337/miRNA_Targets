#include "download.h"

using namespace std;

/* constants definitions */
const std::string Download::TEMPORARY_DOWNLOAD_FILE_NAME = "Download\\downloaded.txt";	//name of the temporary downloaded file path
const std::string Download::ERROR_OUT_OF_MEMORY = "Out of memory";						//error for an out of memory error type
const std::string Download::ERROR_DOWNLOAD_FAILURE = "Download fail";					//error for a download failure error type
const std::string Download::ERROR_APPEND_FAILURE = "Impossible to append the file.";	//error for a output file impossible to append to
const std::string Download::ERROR_APPEND_FAILURE_MESSAGE = "Fail to create one file for all the downloads.";				//error message for a output file impossible to append to
const std::string Download::ERROR_UNKNOW = "Unknow exception.";							//error message for an unknow exception

vector<vector<string>> Download::downloadEnsembles(vector<string> const& ensList, string const& _fileName)
{
	vector<vector<string>> result;
	vector<string> outOfMemory, downloadFailure;
	// URL To Download
	string url;
	// Filename To Save To
	string fileName(TEMPORARY_DOWNLOAD_FILE_NAME);
    const string outputFileName(PathHandler::getDownloadedFileName(_fileName));
    //const string outputFileName(DEFAULT_DOWNLOAD_PATH + utils::getName(_fileName) + DEFAULT_DOWNLOADED_SUFFIX + DEFAULT_DOWNLOADED_EXTENSION);
	remove(outputFileName.c_str());
	ofstream outputFile(outputFileName.c_str(), ios::app);

	for (unsigned int i(0); i < ensList.size(); i++)
	{
		try
		{
            // Download The File
            url = Settings::getSettings()->getDownload_DownloadURL1() + ensList[i] + Settings::getSettings()->getDownload_DownloadURL2();
			Download::downloadFile(url, fileName);	//downloading the file
			Format::formatFile(fileName);			//formatting it
			Download::appendFile(fileName, outputFile);	//appending it to the big one
		}
		catch (string const& e)
		{
			if (e == ERROR_OUT_OF_MEMORY)
			{
				outOfMemory.push_back(ensList[i]);
			}
			else if (e == ERROR_DOWNLOAD_FAILURE)
			{
				downloadFailure.push_back(ensList[i]);
			}
			else if (e == ERROR_APPEND_FAILURE)
			{
				throw ERROR_APPEND_FAILURE_MESSAGE;
			}
			else
			{
				throw ERROR_UNKNOW + "\n" + e;
			}
		}
        //std::cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl;
        //std::cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl;
        //std::cout << "progression : " << i << "/" << ensList.size() << "\nProgress : " << (double)i / (double)ensList.size() * 100.0 << "%";
	}
	remove(fileName.c_str());

	if (!outOfMemory.empty() || !downloadFailure.empty())
	{
		result.push_back(outOfMemory);
		result.push_back(downloadFailure);
	}

	return result;
}

/*
wchar_t * getWCharT(std::string const& str)
{
	// Convert to a wchar_t*
	size_t origsize = str.length() + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t * wcstring = new wchar_t[newsize];
	mbstowcs_s(&convertedChars, wcstring, origsize, str.c_str(), _TRUNCATE);
	return wcstring;
}

std::wstring s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}*/


void Download::downloadFile(string const& url, string const& dest)
{
   // std::wstring stemp = s2ws(url);
	HRESULT hr(
		URLDownloadToFileA(0, url.c_str(), dest.c_str(), 0, 0));
	if (hr == S_OK)
	{
		//OK
	}
	else if (hr == E_OUTOFMEMORY)
	{
		throw ERROR_OUT_OF_MEMORY;
	}
	else if (hr == INET_E_DOWNLOAD_FAILURE)
	{
		throw ERROR_DOWNLOAD_FAILURE;
	}
}

void Download::appendFile(string const& nameFileIn, ofstream & fileOut)
{
	ifstream fileIn(nameFileIn);
	string str;
	if (fileIn && fileOut)
	{
		fileOut << endl;	//par commodité
		while (getline(fileIn, str))
		{
			fileOut << str << endl;
		}
	}
	else
	{
		throw string(ERROR_APPEND_FAILURE);
	}
}
