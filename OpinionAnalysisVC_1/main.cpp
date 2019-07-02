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

const char* cArCategory[] = { "军事", "健康", "财经", "房产", "汽车", "教育", "体育" };//{ "环境", "计算机", "交通", "教育", "经济", "军事", "体育", "医药", "艺术", "政治" };

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
		cout << "Category.exe 分类文本路径\n" << endl;
		cout << "Category.exe 训练文本路径 train" << endl;
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
				cout << "\n文件类型：" << cArCategory[type - 1] << "\t文件名称：" << strFile << endl;
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

				cout << "\n实际类型：" << cArCategory[type - 1] << "\t判断结果：" << cArCategory[rType - 1] << "\t文件名称：" << strFile << endl;
				if (type == rType)
				{
					right++;
					iRight++;
				}
			}
		}

		if (!bTrain)
		{
			cout << "\n类型" << cArCategory[type - 1] << "\t文章总数：" << count << "\t判断正确率：" << right << endl;
			mapRe.insert(make_pair(cArCategory[type - 1], count + (right << 16)));
		}
		else
		{
			cout << "\n类型：" << cArCategory[type - 1] << "\t读取文章总数：" << count << endl;
			mapRe.insert(make_pair(cArCategory[type - 1], count));
		}
    }
    
	if (!bTrain)
	{
		for (auto iter = mapRe.begin(); iter != mapRe.end(); iter++)
		{
			int total = (iter->second & 0xffff);
			int right = (iter->second >> 16);
			cout << "\n类型" << iter->first << "\t文章总数：" << total << "\t判断正确数：" << right << "\t正确率：" << (double)right / total;
		}
		cout << "\n文章总数：" << iCount << "\t判断正确率：" << iRight << "\t正确率：" << (double)iRight /iCount << endl;
	}
	else
	{
		for (auto iter = mapRe.begin(); iter != mapRe.end(); iter++)
		{
			cout << "\n类型" << iter->first << "\t文章总数：" << iter->second;
		}
		cout << "\n读取文章总数：" << iCount << endl;
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
