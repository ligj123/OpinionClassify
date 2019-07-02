#include "dict_analy.h"
#include <log.h>
#include <fstream>
#include <iostream>

using namespace word_segment;

namespace opinion_analysis{

DictAnaly::DictAnaly(void)
{

}

DictAnaly::~DictAnaly(void)
{
    Clear();
}

void DictAnaly::Clear()
{
    for (auto iter = m_mapDict.begin(); iter != m_mapDict.end(); iter++)
    {
        delete iter->second;
    }
}

bool DictAnaly::LoadDict(const string& strPath)
{
    ifstream ifs;
    bool bRt = true;
    unsigned char uLen[2];
    char buf[256];
    unsigned int count;
	double num = (double)sizeof(Category) / sizeof(arCategory);
	m_dblVarAverg = 0;

    try
    {
        boost::unique_lock<boost::shared_mutex> lock(m_mutex);
        ifs.open(strPath.c_str(), ios::binary | ios::in);
        if (!ifs.is_open())
            throw Exception("Failed to open DictAnaly file!");

        ifs.read((char*)&count, sizeof(unsigned int));
        if (ifs.gcount() != sizeof(unsigned int) || count <= 0)
            throw Exception("Failed to get dict words' count!");

        for (unsigned int i = 0; i < count; i++)
        {
            ItemDict* pItem = new ItemDict;
            ifs.read((char*)uLen, sizeof(unsigned char) * 2);
            if (ifs.gcount() != sizeof(unsigned char) * 2)
                throw Exception("Failed to read the word's length! i = " + Utility::IntToStr(i));

            ifs.read(buf, uLen[0]);
            if (ifs.gcount() != uLen[0])
                throw Exception("Failed to read the word itself! i = " + Utility::IntToStr(i));

            unsigned short* tmp = (unsigned short*)buf;
            for (unsigned char jj = 0; jj < uLen[0] / 2; jj++)
            {
                pItem->word += (wchar_t)(tmp[jj]);
            }

			double count = 0;
            for (int kk = 0; kk < uLen[1]; kk++)
            {
                Category type;
                ifs.read((char*)&type, sizeof(Category));
                if (ifs.gcount() != sizeof(Category))
                    throw Exception("Failed to read the word's category! i = " + Utility::IntToStr(i)
                        + "  kk" = Utility::IntToStr(kk));

				long times;
                ifs.read((char*)&times, sizeof(long));
                if (ifs.gcount() != sizeof(long))
                    throw Exception("Failed to read the word's statistical information! i = " + Utility::IntToStr(i)
                        + "  kk" = Utility::IntToStr(kk));

                pItem->mapStat.insert(pair<Category, long>(type, times));
				count += times;
            }

			pItem->dblVar = 0;

			for (auto itStat = pItem->mapStat.begin(); itStat != pItem->mapStat.end(); itStat++)
			{
				pItem->dblVar += (itStat->second / count - num) * (itStat->second / count - num);
			}

			m_dblVarAverg += pItem->dblVar;
            m_mapDict.insert(pair<wstring, ItemDict*>(pItem->word, pItem));
        }

		m_dblVarAverg /= count;
    }
	catch(exception& ex)
	{
        bRt = false;
        Log::Write("DictAnaly::LoadDict", ex.what(), LogTypeException);
	}
	catch(...)
	{
        bRt = false;
        Log::Write(L"DictAnaly::LoadDict", L"Failed to load words' statistical information, Unknown error!", LogTypeException);
	}

    ifs.close();
    return bRt;
}

bool DictAnaly::SaveDict(const string& strPath)
{
	unsigned char uLen[2];
	unsigned int count;
	char buf[256];
    ofstream ofs;
	

	try
	{
        boost::unique_lock<boost::shared_mutex> lock(m_mutex);
        ofs.open(strPath.c_str(), ios::binary | ios::out | ios::trunc);
        if (!ofs.is_open())
            throw Exception("Failed to open DictAnaly file!");

        count = m_mapDict.size();
        ofs.write((char*)&count, sizeof(unsigned int));

        for (auto iter = m_mapDict.begin(); iter != m_mapDict.end(); iter++)
        {
            uLen[0] = (*(iter->second)).word.size() * 2;
            uLen[1] = (*(iter->second)).mapStat.size();
            ofs.write((char*)uLen, sizeof(unsigned char) * 2);

            for (unsigned char ii = 0; ii < uLen[0] / 2; ii++)
            {
                buf[ii * 2] = iter->first[ii] % 256;
                buf[ii * 2 + 1] = iter->first[ii] /256;
            }

            ofs.write(buf, uLen[0]);

			map<Category, long> *pMapStat = &(iter->second->mapStat);

			for (auto itStat = pMapStat->begin(); itStat != pMapStat->end(); itStat++)
            {
                ofs.write((const char*)(&(itStat->first)), sizeof(Category));
                ofs.write((const char*)(&(itStat->second)), sizeof(long));
            }
         }
    }
	catch(exception& ex)
	{
        Log::Write("DictAnaly::SaveDict", ex.what(), LogTypeException);
        return false;
	}
	catch(...)
	{
        Log::Write(L"DictAnaly::SaveDict", L"Failed to save words' statistical information, Unknown error!", LogTypeException);
        return false;
	}

    return true;
}

bool DictAnaly::StatWords(const wstring& text, Category type)
{
    StatText statText;
    if (!statText.CalcText(text, type))
        return false;

    return StatWords(statText, type);
}

 bool DictAnaly::StatWords(const StatText& statText, Category type)
 {
    try
    {
        boost::shared_lock<boost::shared_mutex> lock(m_mutex);
        const map<wstring, WordStat>* pMap = statText.getMapWordStat();

        for (auto iter = pMap->begin(); iter != statText.getMapWordStat()->end(); iter++)
        {
            if (m_mapDict.find(iter->first) == m_mapDict.end())
            {
                ItemDict* pItem = new ItemDict;
                pItem->word = iter->first;
                m_mapDict.insert(pair<wstring, ItemDict*>(iter->first, pItem));
            }

            ItemDict* pItem = m_mapDict.find(iter->first)->second;
            if (pItem->mapStat.find(type) == pItem->mapStat.end())
                pItem->mapStat.insert(pair<Category, long>(type, 0));

			pItem->mapStat[type] += 1;
            
        }
    }
	catch(exception& ex)
	{
        Log::Write("DictAnaly::StatText", ex.what(), LogTypeException);
        return false;
	}
	catch(...)
	{
        Log::Write(L"DictAnaly::StatText", L"Failed to count words' statistical information, Unknown error!", LogTypeException);
        return false;
	}

    return true;
}

const ItemDict* const DictAnaly::SearchItem(wstring word)
{
    auto iter = m_mapDict.find(word);
    if (iter == m_mapDict.end())
        return nullptr;

    return iter->second;
}

}
