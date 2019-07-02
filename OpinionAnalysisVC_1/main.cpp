#include <iostream>
#include <boost/filesystem.hpp>
#include <iword_seg.h>
#include "opinion_analysis.h"
#include <boost/locale.hpp>
#include <vector>

using namespace std;
using namespace word_segment;
using namespace opinion_analysis;
using namespace boost::filesystem;
using namespace boost::locale;

const char* cArCategory[] = { "����", "����", "�ƾ�", "����", "����", "����", "����" };//{ "����", "�����", "��ͨ", "����", "����", "����", "����", "ҽҩ", "����", "����" };

int main(int argc, char *argv[])
{
    if (!IWordSeg::InitWordSegment("./log/", "./", true))
    {
        cout << "Failed to initialize WordSegment!" << endl;
        getchar();
        return -1;
    }

	if (argc != 2 && argc != 3)
	{
		cout << "Please input right parameter!\n";
		cout << "Category.exe �����ı�·��\n" << endl;
		cout << "Category.exe ѵ���ı�·�� train" << endl;
		getchar();
		return -1;
	}
	
	string strTextPath = argv[1];
	bool bTrain = (argc == 3);
    string strAnalyDict = "DictAnaly.dct";

    if (bTrain )
    {
        OpinionAnalysis::setDictAnaly(new DictAnaly);
    }
    else
    {
        if (!boost::filesystem::exists(strAnalyDict)|| !OpinionAnalysis::InitDictAnaly(strAnalyDict))
        {
            cout << "Failed to load the dictionary analysis or it is not exist!" << endl;
            getchar();
            return -1;
        }
    }

    int iRight = 0;
    int iCount = 0;
	map<string, long> mapRe;

    directory_iterator end_iter;
    for(directory_iterator iterDir(strTextPath); iterDir != end_iter; iterDir++)
    {
        if (!is_directory(iterDir->status()))
            continue;

		string strDir = iterDir->path().string();
		int right = 0;
		int count = 0;
		Category type = tcUnknown;
		for (int i = 0; i < sizeof(arCategory) / sizeof(Category); i++)
		{
				
			if (strDir.find(cArCategory[i]) != string::npos) 
			{
			    type =  arCategory[i];
			    break;
			}
		}

		if (tcUnknown == type)
		{
			cout << "Error category! " << strDir << endl;
			getchar();
			return -1;
		}	
			
		for (directory_iterator iterFile(strDir); iterFile != end_iter; iterFile++)
		{
			count++;
			iCount++;
			if (!is_regular_file(iterFile->status()))
				continue;

			string ext = iterFile->path().extension().string();
			transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
			if (ext != ".txt")
				continue;

			string strFile = iterFile->path().string();
			ifstream ifs(strFile);
			stringstream ss;
			ss << ifs.rdbuf();
			ifs.close();



			wstring str = conv::to_utf<wchar_t>(ss.str(), string("utf-8"));
			if (bTrain)
			{
				if (!OpinionAnalysis::StatWords(str, type))
				{
				    cout << "Failed to count the text file!\n" << strFile << endl;
				    getchar();
				    return -1;
				}
				cout << "\n�ļ����ͣ�" << cArCategory[type - 1] << "\t�ļ����ƣ�" << strFile << endl;
			}
			else
			{
				Category rType;
				if (!OpinionAnalysis::JudgeTextCategory(str, rType))
				{
					cout << "Failed to judge text category!\n" << strFile << endl;
					getchar();
					continue;
				}

				cout << "\nʵ�����ͣ�" << cArCategory[type - 1] << "\t�жϽ����" << cArCategory[rType - 1] << "\t�ļ����ƣ�" << strFile << endl;
				if (type == rType)
				{
					right++;
					iRight++;
				}
			}
		}

		if (!bTrain)
		{
			cout << "\n����" << cArCategory[type - 1] << "\t����������" << count << "\t�ж���ȷ�ʣ�" << right << endl;
			mapRe.insert(make_pair(cArCategory[type - 1], count + (right << 16)));
		}
		else
		{
			cout << "\n���ͣ�" << cArCategory[type - 1] << "\t��ȡ����������" << count << endl;
			mapRe.insert(make_pair(cArCategory[type - 1], count));
		}
    }
    
	if (!bTrain)
	{
		for (auto iter = mapRe.begin(); iter != mapRe.end(); iter++)
		{
			int total = (iter->second & 0xffff);
			int right = (iter->second >> 16);
			cout << "\n����" << iter->first << "\t����������" << total << "\t�ж���ȷ����" << right << "\t��ȷ�ʣ�" << (double)right / total;
		}
		cout << "\n����������" << iCount << "\t�ж���ȷ�ʣ�" << iRight << "\t��ȷ�ʣ�" << (double)iRight /iCount << endl;
	}
	else
	{
		for (auto iter = mapRe.begin(); iter != mapRe.end(); iter++)
		{
			cout << "\n����" << iter->first << "\t����������" << iter->second;
		}
		cout << "\n��ȡ����������" << iCount << endl;
		if (!OpinionAnalysis::SaveDictAnaly(strAnalyDict))
		{
		    cout << "Failed to save dictionary analysis file!" << endl;
		    return -1;
		}
		cout << "Passed to save dict analy file!" << endl;
	}

    getchar();    
    return 0;
}
