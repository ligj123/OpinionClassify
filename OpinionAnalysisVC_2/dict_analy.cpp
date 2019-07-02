#include "dict_analy.h"
#include <log.h>
#include <fstream>
#include <boost/locale.hpp>

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
        for (auto itStat = iter->second->mapStat.begin(); itStat != iter->second->mapStat.end(); itStat++)
            delete itStat->second;

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

    try
    {
        boost::unique_lock<boost::shared_mutex> lock(m_mutex);
        ifs.open(strPath.c_str(), ios::binary | ios::in);
        if (!ifs.is_open())
            throw Exception("Failed to open DictAnaly file!");

        ifs.read((char*)&m_statTotal, sizeof(ItemStat));
        if (ifs.gcount() != sizeof(ItemStat))
            throw Exception("Failed to read total ItemStat!");

		ifs.read((char*)&count, sizeof(unsigned int));
		if (ifs.gcount() != sizeof(unsigned int) || count <= 0)
			throw Exception("Failed to get category' count!");

		for (unsigned int i = 0; i < count; i++)
		{
			Category type;
			ifs.read((char*)&type, sizeof(unsigned int));
			if (ifs.gcount() != sizeof(unsigned int))
				throw Exception("Failed to read Category!");

			ItemStat* item = new ItemStat;
			ifs.read((char*)item, sizeof(ItemStat));
			if (ifs.gcount() != sizeof(ItemStat))
				throw Exception("Failed to read category total ItemStat!");

			m_mapStatCategory.insert(pair<Category, ItemStat*>(type, item));
		}

        //∂¡»°m_mapDict
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

            for (int kk = 0; kk < uLen[1]; kk++)
            {
                Category type;
                ifs.read((char*)&type, sizeof(Category));
                if (ifs.gcount() != sizeof(Category))
                    throw Exception("Failed to read the word's category! i = " + Utility::IntToStr(i)
                        + "  kk" = Utility::IntToStr(kk));

                ItemStat* pStat = new ItemStat;
                ifs.read((char*)pStat, sizeof(ItemStat));
                if (ifs.gcount() != sizeof(ItemStat))
                    throw Exception("Failed to read the word's statistical information! i = " + Utility::IntToStr(i)
                        + "  kk" = Utility::IntToStr(kk));

                pItem->mapStat.insert(pair<Category, ItemStat*>(type, pStat));
            }

            m_mapDict.insert(pair<wstring, ItemDict*>(pItem->word, pItem));
        }

        //∂¡»°m_mapEmotional
        ifs.read((char*)&count, sizeof(unsigned int));
        if (ifs.gcount() != sizeof(unsigned int) || count <= 0)
            throw Exception("Failed to get the emotional words' count!");

        for (unsigned int i = 0; i < count; i++)
        {
            ifs.read((char*)uLen, sizeof(unsigned char)* 1);
            if (ifs.gcount() != sizeof(unsigned char)* 1)
                throw Exception("Failed to read the emotional word's length! i = " + Utility::IntToStr(i));

            ifs.read(buf, uLen[0]);
            if (ifs.gcount() != uLen[0])
                throw Exception("Failed to read the emotional word itself! i = " + Utility::IntToStr(i));

            unsigned short* tmp = (unsigned short*)buf;
            wstring word;
            for (unsigned char jj = 0; jj < uLen[0] / 2; jj++)
            {
                word += (wchar_t)(tmp[jj]);
            }

            ItemStat* pStat = new ItemStat;
            ifs.read((char*)pStat, sizeof(ItemStat));
            if (ifs.gcount() != sizeof(ItemStat))
                throw Exception("Failed to read the emotional word's information! i = " + Utility::IntToStr(i));

            m_mapEmotional.insert(pair<wstring, ItemStat*>(word, pStat));
        }
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

        ofs.write((char*)&m_statTotal, sizeof(ItemStat));
		count = m_mapStatCategory.size();
        ofs.write((char*)&count,  sizeof(unsigned int));
		for (auto iter = m_mapStatCategory.begin(); iter != m_mapStatCategory.end(); iter++)
		{
			ofs.write((char*)&iter->first, sizeof(unsigned int));
			ofs.write((char*)iter->second, sizeof(ItemStat));

		}

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

            for (auto itStat = (*(iter->second)).mapStat.begin(); itStat != (*(iter->second)).mapStat.end(); itStat++)
            {
                ofs.write((const char*)(&itStat->first), sizeof(Category));
                ofs.write((const char*)(itStat->second), sizeof(ItemStat));
            }
         }

        count = m_mapEmotional.size();
        ofs.write((char*)&count, sizeof(unsigned int));

        for (auto iter = m_mapEmotional.begin(); iter != m_mapEmotional.end(); iter++)
        {
            uLen[0] = iter->first.size() * 2;            
            ofs.write((char*)uLen, sizeof(unsigned char)* 1);

            for (unsigned char ii = 0; ii < uLen[0] / 2; ii++)
            {
                buf[ii * 2] = iter->first[ii] % 256;
                buf[ii * 2 + 1] = iter->first[ii] / 256;
            }

            ofs.write(buf, uLen[0]);
            ofs.write((const char*)(iter->second), sizeof(ItemStat));            
        }

    }
	catch(exception& ex)
	{
        ofs.close();
        Log::Write("DictAnaly::SaveDict", ex.what(), LogTypeException);
        return false;
	}
	catch(...)
	{
        ofs.close();
        Log::Write(L"DictAnaly::SaveDict", L"Failed to save words' statistical information, Unknown error!", LogTypeException);
        return false;
	}

    ofs.close();
    return true;
}

bool DictAnaly::LoadKeywords(const string& strPath)
{
    ifstream ifs;
    bool bRt = true;
    char buf[1024];
 
    try
    {
        boost::unique_lock<boost::shared_mutex> lock(m_mutex);
        ifs.open(strPath.c_str(), ios::binary | ios::in);
        if (!ifs.is_open())
            throw Exception("Failed to open DictAnaly file!");

        while (true)
        {
            ifs.getline(buf, 1024);
            int n = (int)ifs.gcount();
            if (n == 0)
                break;

            wstring tmp = boost::locale::conv::to_utf<wchar_t>(buf, string("utf-8"));
            vector<wstring> vct = Utility::SplitWString(tmp, L"\t", true);
            if (vct.size() != 2)
                continue;
            
            m_mapKeywords.insert(pair<wstring, int>(vct[0], (int)Utility::WStrToLong(vct[1])));
        }
    }
    catch (exception& ex)
    {
        bRt = false;
        Log::Write("DictAnaly::SaveDict", ex.what(), LogTypeException);
        return false;
    }
    catch (...)
    {
        bRt = false;
        Log::Write(L"DictAnaly::SaveDict", L"Failed to save words' statistical information, Unknown error!", LogTypeException);
        return false;
    }

    ifs.close();
    return bRt;
}

bool DictAnaly::LoadDenyWord(const string& strPath)
{
    ifstream ifs;
    bool bRt = true;
    char buf[1024];

    try
    {
        boost::unique_lock<boost::shared_mutex> lock(m_mutex);
        ifs.open(strPath.c_str(), ios::binary | ios::in);
        if (!ifs.is_open())
            throw Exception("Failed to open DictAnaly file!");

        while (true)
        {
            ifs.getline(buf, 1024);
            int n = (int)ifs.gcount();
            if (n == 0)
                break;

            wstring tmp = boost::locale::conv::to_utf<wchar_t>(buf, string("utf-8"));
            m_hashDenyWords.insert(tmp);
        }
    }
    catch (exception& ex)
    {
        bRt = false;
        Log::Write("DictAnaly::SaveDict", ex.what(), LogTypeException);
        return false;
    }
    catch (...)
    {
        bRt = false;
        Log::Write(L"DictAnaly::SaveDict", L"Failed to save words' statistical information, Unknown error!", LogTypeException);
        return false;
    }

    ifs.close();
    return bRt;
}

bool DictAnaly::StatWords(const wstring& text, Category type, bool bVerified, emOpinion opinion)
{
    StatText statText;
    if (!statText.CalcText(L"", text, type))
        return false;

    return StatWords(statText, type, bVerified, opinion);
}

bool DictAnaly::StatWords(const StatText& statText, Category type, bool bVerified, emOpinion opinion)
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
                pItem->mapStat.insert(pair<Category, ItemStat*>(type, new ItemStat));

            ItemStat* pStat = pItem->mapStat.find(type)->second;
			
			if (m_mapStatCategory.find(type) == m_mapStatCategory.end())
				m_mapStatCategory.insert(pair<Category, ItemStat*>(type, new ItemStat));

			ItemStat* pStatTotal = m_mapStatCategory.find(type)->second;

            if (bVerified)
            {
                if (opnPositive == opinion)
                {
                    pStat->iWordsVerifiedPositive += iter->second.vctPosition.size();
                    pStatTotal->iWordsVerifiedPositive += iter->second.vctPosition.size();
                    m_statTotal.iWordsVerifiedPositive += iter->second.vctPosition.size();
                }
                else if (opnNeutral == opinion)
                {
                    pStat->iWordsVerifiedNeutral += iter->second.vctPosition.size();
                    pStatTotal->iWordsVerifiedNeutral += iter->second.vctPosition.size();
                    m_statTotal.iWordsVerifiedNeutral += iter->second.vctPosition.size();
                }
                else
                {
				    pStat->iWordsVerifiedNegative += iter->second.vctPosition.size();
				    pStatTotal->iWordsVerifiedNegative += iter->second.vctPosition.size();
				    m_statTotal.iWordsVerifiedNegative += iter->second.vctPosition.size();
                }
            }
            else
            {
                if (opnPositive == opinion)
                {
                    pStat->fWordsUnverifiedPositive += iter->second.vctPosition.size();
                    pStatTotal->fWordsUnverifiedPositive += iter->second.vctPosition.size();
                    m_statTotal.fWordsUnverifiedPositive += iter->second.vctPosition.size();
                }
                else if (opnNeutral == opinion)
                {
                    pStat->fWordsUnverifiedNeutral += iter->second.vctPosition.size();
                    pStatTotal->fWordsUnverifiedNeutral += iter->second.vctPosition.size();
                    m_statTotal.fWordsUnverifiedNeutral += iter->second.vctPosition.size();
                }
                else
                {
                    pStat->fWordsUnverifiedNegative += iter->second.vctPosition.size();
                    pStatTotal->fWordsUnverifiedNegative += iter->second.vctPosition.size();
                    m_statTotal.fWordsUnverifiedNegative += iter->second.vctPosition.size();
                }
            }
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

void DictAnaly::AddOpinionStat(wstring str, int iIntensity, emOpinion opinion)
{
    if (m_mapEmotional.find(str) == m_mapEmotional.end())
        m_mapEmotional.insert(pair<wstring, ItemStat*>(str, new ItemStat));

    ItemStat* pStat = m_mapEmotional.find(str)->second;
    switch (opinion)
    {
    case opnPositive:
        pStat->iWordsVerifiedPositive += iIntensity;
        break;
    case opnNeutral:
        pStat->iWordsVerifiedNeutral += iIntensity;
        break;
    case opnNegative:
        pStat->iWordsVerifiedNegative += iIntensity;
        break;
    }
}

}
