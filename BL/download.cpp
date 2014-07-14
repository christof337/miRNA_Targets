#include "download.h"

using namespace std;


vector<vector<string>> & Download::downloadEnsembles(vector<string> const& ensList)
{
	vector<vector<string>> result;
	vector<string> outOfMemory, downloadFailure;
	// URL To Download
	string url;
	// Filename To Save To
	string fileName(TEMPORARY_DOWNLOAD_FILE_NAME);
	int res(0);
	remove(DEFAULT_DOWNLOADED_PATH.c_str());
	ofstream outputFile(DEFAULT_DOWNLOADED_PATH,ios::app);

	for(unsigned int i(0); i < ensList.size(); i++)
	{
		try
		{
			// Download The File
			url = DOWNLOAD_URL_1 + ensList[i] + DOWNLOAD_URL_2;
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
				throw ERROR_UNKNOW;
			}
		}
		std::cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl;
		std::cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl;
		std::cout << "progression : " << i << "/" << ensList.size() << "\nProgress : " << (double)i / (double)ensList.size() * 100.0 << "%";
	}
	remove(fileName.c_str());

	result.push_back(outOfMemory);
	result.push_back(downloadFailure);
	return result;
}

void Download::downloadFile(string const& url, string const& dest, bool shouldAppend)
{
	HRESULT hr(
		URLDownloadToFile(0, url.c_str(), dest.c_str(), 0, 0));
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