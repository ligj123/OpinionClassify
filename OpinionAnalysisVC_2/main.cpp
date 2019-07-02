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


bool LoadEmotionalWord(string strAnalyDict)
{
    string strPath = "D:\\舆情中心文档\\吴琳\\情感词汇表.txt";
    ifstream ifs(strPath, ios_base::binary);
    char* buf = new char[10240];
    DictAnaly* pDictAnaly = OpinionAnalysis::getDictAnaly();

    while (true)
    {
        ifs.getline(buf, 10240);
        int n = (int)ifs.gcount();
        if (n == 0)
            break;

        wstring tmp = conv::to_utf<wchar_t>(buf, string("gb2312"));
        vector<wstring> vct = Utility::SplitWString(tmp, L"\t", true);
        if (vct.size() != 3)
            continue;

        emOpinion opinion;
        switch (vct[2][0])
        {
        case L'0':
            opinion = opnNeutral;
            break;
        case L'1':
            opinion = opnPositive;
            break;
        case L'2':
            opinion = opnNegative;
            break;
        default:
            continue;
        }

        int ii;
        if (!Utility::WStringToInt(vct[1], ii))
            continue;

        pDictAnaly->AddOpinionStat(vct[0], ii, opinion);
    }

    if (!OpinionAnalysis::SaveDictAnaly(strAnalyDict))
    {
        cout << "Failed to save dict analy file!" << endl;
        return false;
    }

    return true;
}

bool TestOpinion()
{
    string strPath = "D:\\舆情中心文档\\吴琳\\opinion.txt";
    ifstream ifs(strPath, ios_base::binary);
    char* buf = new char[10240];

    ofstream ofs("D:\\舆情中心文档\\吴琳\\opinionResult.txt", ios_base::binary);
    int index = 1;

    while (true)
    {
        ifs.getline(buf, 10240);
        int n = (size_t)ifs.gcount();
        if (n == 0)
            break;

        wstring tmp = conv::to_utf<wchar_t>(buf, string("utf-8"));
        tmp = tmp.substr(0, tmp.size() - 2);
        vector<wstring> vct = Utility::SplitWString(tmp, L"\t");
        if (vct.size() != 2)
            continue;
        
        emOpinion opinion;
        Category type = tcOpinion;
        if (!OpinionAnalysis::JudgeTextOpinion(vct[0], vct[1], type, opinion, algSemantic))
        {
            std::cout << "Failed to judge text opinion!" << tmp << endl;
            getchar();
        }

        emOpinion opinion2;
        if (!OpinionAnalysis::JudgeTextOpinion(vct[0], vct[1], type, opinion2))
        {
            std::cout << "Failed to judge text opinion!" << tmp << endl;
            getchar();
        }

        if (opinion != opinion2)
            opinion = opnNeutral;

        switch (opinion)
        {
        case opnPositive:
            tmp = L"正面\t" + tmp;
            break;
        case opnNeutral:
            tmp = L"中性\t" + tmp;
            break;
        case opnNegative:
            tmp = L"负面\t" + tmp;
            break;
        default:
            tmp = L"未判断\t" + tmp;
            break;
        }

        string ss = conv::from_utf<wchar_t>(tmp.c_str(), "utf-8");
        ss += "\n";
        ofs.write(ss.c_str(), ss.size());

        cout << index << "\t" << tmp << endl;
        index++;
    }

    ifs.close();
    ofs.close();
    return true;
}

bool LoadTrainingText(string strAnalyDict)
{
    //string strAnalyDict = "D:/舆情分析/DictAnaly.dct";
    string strTextPath = "D:\\舆情中心文档\\吴琳\\text";
    directory_iterator end_iter;
    int index = 0;


    for (directory_iterator iter(strTextPath); iter != end_iter; iter++)
    {
        string ext = iter->path().extension().string();
        transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        if (ext != ".txt")
            continue;

        string strFile = iter->path().string();
        ifstream ifs(strFile, ios_base::binary);
        ifs.seekg(0, ios_base::end);
        size_t length = (size_t)ifs.tellg();
        ifs.seekg(0, ios_base::beg);
        char* buf = new char[length + 2];
        size_t ii = (size_t)ifs.tellg();
        buf[length] = 0;
        buf[length + 1] = 0;
        char* p = buf;

        while (length > 0)
        {
            size_t n = length > 10240 ? 10240 : length;
            ifs.read(p, n);
            n = (int)ifs.gcount();
            length -= n;
            p += n;
        }
        ifs.close();


        emOpinion opinion ;
        if (strFile.find("positive") != wstring::npos)
            opinion = opnPositive;
        else if (strFile.find("negative") != wstring::npos)
            opinion = opnNegative;
        else if (strFile.find("neutral") != wstring::npos)
            opinion = opnNeutral;
        else
            continue;

        wstring str = conv::to_utf<wchar_t>(buf, string("utf-8"));
        delete[] buf;

        size_t start = 0;
        size_t end = 0;
        while (start != wstring::npos)
        {
            start = str.find(L'\t', end);
            if (start == wstring::npos)
                break;
            end = str.find(L'\n', start);
            wstring tmp = str.substr(start + 1, end - start - 1);
           
            if (!OpinionAnalysis::StatWords(tmp, tcOpinion, true, opinion))
            {
                std::cout << "Failed to count the text file!\n" << tmp << endl;
                getchar();
                return false;
            }

            std::cout << "Failed to count the text file!\n" << tmp << endl;
           
        }
    }

    if (!OpinionAnalysis::SaveDictAnaly(strAnalyDict))
    {
        cout << "Failed to save dict analy file!" << endl;
        return false;
    }

    return true;
}
int main()
{
    string strPath = "D:/WordSegment/";

    if (!IWordSeg::InitWordSegment(strPath + "log/", strPath, true))
    {
        std::cout << "Failed to initialize WordSegment!" << endl;
        getchar();
        return -1;
    }

    string strAnalyDict = "D:/舆情分析/DictAnaly.dct";
    if (boost::filesystem::exists(strAnalyDict))
    {
        if (!OpinionAnalysis::InitDictAnaly(strAnalyDict))
        {
            std::cout << "Failed to load the dictionary analysis!" << endl;
            getchar();
            return -1;
        }
    }
    else
    {
        std::cout << "Failed to find the dictionary analysis!\nIf you continue to run, please press the key 'Y'." << endl;
        //int c = getchar();
        //if (c != 'y' && c != 'Y')
        //    return -1;

        OpinionAnalysis::setDictAnaly(new DictAnaly);
    }


    //bool b = LoadTrainingText(strAnalyDict);
    //b = LoadEmotionalWord(strAnalyDict);
 
    bool b = TestOpinion();



    
    getchar();
    
    return 0;
}
