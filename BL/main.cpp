#include "load.h"
#include "download.h"
#include "targetFinder.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
	TargetFinder tf;
	try
	{
		tf.getMiRNAs("Download\\mir7_d.txt");// Tests\\\originaux3\\mir7_d.txt");
	}
	catch (string const& s)
	{
		cerr << "Erreur lors de la recuperation des miRNA" << endl << s << endl;
	}
	for (unsigned int i = 0; i < tf.m_pMiRNA.size(); i++)
	{
		cout << i << "Name : " << tf.m_pMiRNA[i].name << endl << "Sequence : " << tf.m_pMiRNA[i].miRNA << endl << endl;
	}
	try
	{
		tf.loadSequenceFile(DEFAULT_DOWNLOADED_PATH);
	}
	catch (string const& s)
	{
		cerr << "Erreur lors de la recuperation des sequences" << endl << s << endl;
	}
	system("PAUSE");
	return 0;

	vector<string> res;
    try
	{
		res=Load::loadAFile("C:\\Users\\Public\\Documents\\zaki-pont\\miRNA\\mmc2.csv",false);
   
		for (unsigned int i = 0; i < res.size(); i++)
		{
			//cout << res[i] << endl;
		}
		cout << "Success." << endl;
    }
    catch(string const& str)
    {
        cerr << str << endl;
    }
	try
	{
		Download::downloadEnsembles(res);
		cout << endl << "TOTAL SUCCESS." << endl;
	}
	catch (string const& e)
	{
		cerr << e;
	}
	
	system("PAUSE");
    return 0;
}
