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

struct Stat
{
    Stat()
    {
        iJudgeResult = 0;
        iJudgeRight = 0;
        iActualResult = 0;
    }

    int iJudgeResult;   //判断为该类结果的数量
    int iJudgeRight;    //判断为该类文本结果正确的数量
    int iActualResult;   //该类的测试文本数量
};

int main()
{
    string strPath = "D:/WordSegment/";

    if (!IWordSeg::InitWordSegment(strPath + "log/", strPath, true))
    {
        cout << "Failed to initialize WordSegment!" << endl;
        getchar();
        return -1;
    }

    string strAnalyDict = "D:/舆情分析/DictAnaly.dct";
    if (boost::filesystem::exists(strAnalyDict))
    {
        if (!OpinionAnalysis::InitDictAnaly(strAnalyDict))
        {
            cout << "Failed to load the dictionary analysis!" << endl;
            getchar();
            return -1;
        }
    }
    else
    {
        cout << "Failed to find the dictionary analysis!\nIf you continue to run, please press the key 'Y'." << endl;
        //int c = getchar();
        //if (c != 'y' && c != 'Y')
        //    return -1;

        OpinionAnalysis::setDictAnaly(new DictAnaly);
    }

    int iJudgeRight = 0;
    int iRealRight = 0;
    int iNegative = 0;
    vector<string> vctError;

    string strTextPath = "D:\\舆情分析\\自动分类测试数据\\分类用文本";
    directory_iterator end_iter;
    int index = 0;

    map<Category, Stat> mapStat;
    for (int i = 0; i < 10; i++)
    {
        mapStat.insert(pair<Category, Stat>(arCategory[i], Stat()));
    }

    Stat statTotal;
    const char textType[10][8] = { "环境", "计算机", "交通", "教育", "经济", "军事", "体育", "医药", "艺术", "政治" };

    for (directory_iterator iter(strTextPath); iter != end_iter; iter++)
    {
        if (!is_directory(*iter))
            continue;

        for (directory_iterator itChild(iter->path()); itChild != end_iter; itChild++)
        {
            index++;
            string ext = itChild->path().extension().string();
            transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
            if (ext != ".txt")
                continue;

            string strFile = itChild->path().string();
            ifstream ifs(strFile);
            stringstream ss;
            ss << ifs.rdbuf();
            ifs.close();


            Category type = tcUnknown;
            
            for (int i = 0; i < 10; i++)
            {
                if (strFile.find(textType[i]) != string::npos)
                {
                    type = arCategory[i];
                    break;
                }
            }

            wstring str = conv::to_utf<wchar_t>(ss.str(), string("gb2312"));

            //if (!OpinionAnalysis::StatWords(str, type, true, false))
            //{
            //    cout << "Failed to count the text file!\n" << strFile << endl;
            //    getchar();
            //    return -1;
            //}

            string sType;
            Category backType = type;
            if (!OpinionAnalysis::JudgeTextCategory(str, type))
            {
                cout << "Failed to judge text category!\n" << strFile << endl;
                getchar();
                return -1;
            }
            

            for (int i = 0; i < 10; i++)
            {
                if (type == arCategory[i])
                {
                    sType = textType[i];
                    break;
                }
            }

            auto iter1 = mapStat.find(type);
            iter1->second.iJudgeResult++;
            if (type == backType)
                iter1->second.iJudgeRight++;

            auto iter2 = mapStat.find(backType);
            iter2->second.iActualResult++;

            statTotal.iActualResult++;
            if (type == backType)
                statTotal.iJudgeRight++;

            cout << "index: " << index << "\t" << sType << "\t" << strFile << endl;
        }


        //vector<string> vct = Utility::SplitString(iter->path().filename().string(), "_");
        //bool bNegative = (vct[1][0] == 'N');

        //for (int i = 0; i < sizeof(arCategory) / sizeof( unsigned int); i++)
        //{
        //    if (vct[0] == cArCategory[i])
        //    {
        //        type =  arCategory[i];
        //        break;
        //    }
        //}

        //if (tcUnknown == type)
        //{
        //    cout << "Error file name! " << strFile << endl;
        //    getchar();
        //    return -1;
        //}


        //if (bNegative)
        //    iNegative++;

        //bool bResult;
        //if (!OpinionAnalysis::JudgeTextOpinion(str, type, bResult))
        //{
        //    cout << "Failed to judge text opinion!\n" << strFile << endl;
        //    getchar();
        //    return -1;
        //}

        //if (bResult)
        //{
        //    iJudgeRight++;
        //    if (bNegative)
        //        iRealRight++;
        //}

        //if (bNegative != bResult)
        //    vctError.push_back(iter->path().filename().string());


        //if (!OpinionAnalysis::StatWords(str, type, true, bNegative))
        //{
        //    cout << "Failed to count the text file!\n" << strFile << endl;
        //    getchar();
        //    return -1;
        //}
        //const char* cType = nullptr;
        //for (int i = 0; i < sizeof(arCategory) / sizeof (unsigned int); i++)
        //{
        //    if (type == arCategory[i])
        //    {
        //        cType = cArCategory[i];
        //    }
        //}

        //cout << "index: "<< index << "\t" << iter->path().filename().string() << endl;
    }
    
    //if (!OpinionAnalysis::SaveDictAnaly(strAnalyDict))
    //{
    //    cout << "Failed to save dict analy file!" << endl;
    //    return -1;
    //}
    
    for (auto iter = mapStat.begin(); iter != mapStat.end(); iter++)
    {
        string sType;
        for (int i = 0; i < 10; i++)
        {
            if (iter->first == arCategory[i])
            {
                sType = textType[i];
                break;
            }
        }

        cout << sType << "\t正确率：" << (float)iter->second.iJudgeRight / iter->second.iJudgeResult
            << "\t召回率：" << (float)iter->second.iJudgeRight / iter->second.iActualResult << endl;
    }

    cout << "总正确率：" << (float)statTotal.iJudgeRight / statTotal.iActualResult << endl;
    cout << "Passed to save dict analy file!" << endl;
    getchar();
    
    return 0;
}
