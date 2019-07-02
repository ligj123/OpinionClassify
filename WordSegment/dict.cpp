#include "dict.h"
#include "log.h"
#include "exception_ex.h"
#include "utility.h"
#include "dict_attr.h"

namespace word_segment {

CharDict* Dict::m_charDict = NULL;
unordered_map<wstring, WordDict*> Dict::m_dictHash;
unordered_map<wchar_t, void*> Dict::m_forHashMap;
unordered_map<wchar_t, void*> Dict::m_backHashMap;
map<wstring, WordDict*> Dict::m_mapNewWord;

bool Dict::LoadCharDict(string dictFileName)
{
	try
	{
        m_charDict = new CharDict[65536];
        ifstream ifs(dictFileName, ios::binary | ios::in);
        size_t len = sizeof(CharDict) * 65536;
        ifs.read((char*)m_charDict, sizeof(CharDict) * 65536);
        size_t n = (size_t)ifs.gcount();
        if (n != len)
        {
            ifs.close();
            Log::Write(L"Dict::LoadCharDict", L"Failed to load char attribute dictionary!", LogTypeError);
            return false;
        }

        ifs.close();
    }
    catch(exception& ex)
    {
        Log::Write("Dict::LoadCharDict", ex.what(), LogTypeException);
        return false;
    }
    catch(...)
    {
        Log::Write(L"Dict::LoadCharDict", L"Failed to load char attribute dictionary! Unknown error!", LogTypeException);
        return false;
	}

	return true;
}

bool Dict::LoadWordDict(string dictFileName)
{
	unsigned char uLen[2];
	unsigned int count;
	char buf[256];

    try
	{
        ifstream ifs(dictFileName, ios::binary | ios::in);

        //Read word dict
        ifs.read((char*)&count, sizeof(unsigned int));

        if (ifs.gcount() != sizeof(unsigned int))
        {
            Log::Write(L"Dict::LoadWordDict", L"Failed to read word item's count from dictionary file!", LogTypeError);
            ifs.close();
            return false;
        }

        if (count <= 0)
        {
            Log::Write(L"Dict::LoadWordDict", L"Dictionary file has error, word item count <= 0!");
            ifs.close();
            return false;
        }

        for (unsigned int i = 0; i < count; i++)
        {
            WordDict *pDictWord = new WordDict;

            ifs.read((char*)uLen, 2 * sizeof(unsigned char));

            if (ifs.gcount() != 2 * sizeof(unsigned char))
            {
                Log::Write(L"Dict::LoadWordDict", L"Failed to read word item' length, index = " + Utility::IntToWStr(i));
                ifs.close();
                delete pDictWord;
                return false;
            }

            if (uLen[0] >= WORD_MAX_LENGTH || uLen[0] == 0)
            {
                Log::Write(L"Dict::LoadWordDict", L"Word item string's length has error, index = " + Utility::IntToWStr(i));
                ifs.close();
                delete pDictWord;
                return false;
            }

            ifs.read(buf, uLen[0]);
            if (ifs.gcount() != uLen[0])
            {
                Log::Write(L"Dict::LoadWordDict", L"Failed to read word string, index = " + Utility::IntToWStr(i));
                ifs.close();
                delete pDictWord;
                return false;
            }

            unsigned short* tmp = (unsigned short*)buf;
            for (unsigned char ii = 0; ii < uLen[0] / 2; ii++)
            {
                pDictWord->word += (wchar_t)(tmp[ii]);
            }

            ifs.read((char*)&(pDictWord->timeCount), sizeof(unsigned int));
            if (ifs.gcount() != sizeof(unsigned int))
            {
                Log::Write(L"Dict::LoadWordDict", L"Failed to read word used times, index = " + Utility::IntToWStr(i) + L"  Word = " + pDictWord->word);
                ifs.close();
                delete pDictWord;
                return false;
            }

            ifs.read(buf, uLen[1]);
            if (ifs.gcount() != uLen[1])
            {
                Log::Write(L"Dict::LoadWordDict", L"Failed to read word attribute, index = " + Utility::IntToWStr(i) + L"  Word = " + pDictWord->word);
                ifs.close();
                delete pDictWord;
                return false;
            }

            for (unsigned char len = 0; len < uLen[1]; len++)
                pDictWord->vctWordAttr.push_back((emWordAttribute)buf[len]);

            pDictWord->bChn = false;
            for (wstring::iterator iter = pDictWord->word.begin(); iter != pDictWord->word.end(); iter++)
            {
                if (m_charDict[*iter].attribute & CharHan)
                {
                    pDictWord->bChn = true;
                    break;
                }
            }
            pDictWord->bAutoNew = false;

            if (!AddItem(pDictWord))
            {
                delete pDictWord;
                Log::Write(L"Dict::LoadWordDict",
                           L"Failed to add item to dictionary, index = " + Utility::IntToWStr(i) + L"  Word = " + pDictWord->word,
                           LogTypeError);
                ifs.close();
                delete pDictWord;
                return false;
            }

            if (i % 1000 == 0)
                Log::Write(L"Dict::LoadWordDict", L"Load word dict No: " + Utility::IntToWStr(i), LogTypeInforLow);
        }

        Log::Write(L"Dict::LoadWordDict", L"Load word dict No: " + Utility::IntToWStr(count), LogTypeInforLow);
        ifs.close();
	}
	catch(exception& ex)
	{
        Log::Write("Dict::LoadWordDict", ex.what(), LogTypeException);
        return false;
	}
	catch(...)
	{
        Log::Write(L"Dict::LoadWordDict", L"Failed to load word attribute dictionary, Unknown error!", LogTypeException);
        return false;
	}

	return true;
}

bool Dict::SaveWordDict(string dictFileName)
{
	unsigned char uLen[2];
	unsigned int count;
	char buf[256];
	try
	{
        ofstream ofs(dictFileName, ios::binary | ios::out);

        count = (unsigned int)m_dictHash.size();
        ofs.write((char*)&count, sizeof(unsigned int));

        int i = 0;
        for (unordered_map<wstring, WordDict*>::iterator iter = m_dictHash.begin(); iter != m_dictHash.end(); iter++)
        {
            WordDict *pDictWord = iter->second;
            uLen[0] = (unsigned char)pDictWord->word.size() * 2;
            uLen[1] = (unsigned char)pDictWord->vctWordAttr.size();

            ofs.write((char*)uLen, 2 * sizeof(char));

            for (unsigned char ii = 0; ii < uLen[0] / 2; ii++)
            {
                buf[ii * 2] = pDictWord->word[i] % 256;
                buf[ii * 2 + 1] = pDictWord->word[i] /256;
            }

            ofs.write(buf, uLen[0]);

            ofs.write((char*)&(pDictWord->timeCount), sizeof(unsigned long));

            for (unsigned char len = 0; len < uLen[1]; len++)
            buf[len] = (char)pDictWord->vctWordAttr[len];
            ofs.write(buf, uLen[1]);

            if (i % 1000 == 0)
                Log::Write(L"Dict::SaveWordDict", L"Save word dict No: " + Utility::IntToWStr(i), LogTypeInforLow);

            i++;
        }

        Log::Write(L"Dict::SaveWordDict", L"Save word dict No: " + Utility::IntToWStr(count), LogTypeInforLow);
        ofs.close();
	}
	catch(exception& ex)
	{
        Log::Write("Dict::SaveWordDict", ex.what(), LogTypeException);
        return false;
	}
	catch(...)
	{
        Log::Write(L"Dict::SaveWordDict", L"Failed to save word attribute dictionary, Unknown error!", LogTypeException);
        return false;
	}

	return true;
}

bool Dict::AddItem(WordDict* pDictWord)
{
	for (size_t i = 0; i < pDictWord->word.size(); i++)
	{
        if (!CharAttr(pDictWord->word[i], CharToWord) &&
            !CharAttr(pDictWord->word[i], CharImSegMarker) &&
            !CharAttr(pDictWord->word[i], CharSpace))
        {
            Log::Write(L"Dict::AddItem", pDictWord->word + L" has illegal char!", LogTypeError);
            return false;
        }

        pDictWord->word[i] = m_charDict[pDictWord->word[i]].destChar;
	}

	unordered_map<wstring, WordDict*>::iterator iter = m_dictHash.find(pDictWord->word);
	if (iter != m_dictHash.end())
	{
        Log::Write(L"Dict::AddItem", pDictWord->word + L" has exist in dictionary!", LogTypeWarning);
        return true;
	}

	m_dictHash.insert(pair<wstring, WordDict*>(pDictWord->word, pDictWord));

	if (!AddItemToMM((pDictWord->word + WORD_END_SIGN).c_str(), &m_forHashMap))
        return false;

	if (!AddItemToMM((Utility::ReverseString(pDictWord->word) + WORD_END_SIGN).c_str(), &m_backHashMap))
        return false;

	return true;
}

bool Dict::AddItemToMM(const wchar_t* word, unordered_map<wchar_t, void*> *hashMap)
{
	unordered_map<wchar_t, void*>::iterator iter = hashMap->find(word[0]);
	if (iter == hashMap->end())
	{
        if (word[0] == WORD_END_SIGN)
        {
            hashMap->insert(pair<wchar_t, void*>(word[0], nullptr));
            return true;
        }
        else
        {
            unordered_map<wchar_t, void*>* pHash = new unordered_map<wchar_t, void*>;
            hashMap->insert(pair<wchar_t, void*>(word[0], pHash));
            return AddItemToMM(word + 1, pHash);
        }
	}
	else
	{
        if (word[0] == WORD_END_SIGN)
        {
            return true;
        }
        else
        {
            unordered_map<wchar_t, void*>* pHash = (unordered_map<wchar_t, void*>*)iter->second;
            return AddItemToMM(word + 1, pHash);
        }
	}
}

bool Dict::AddAutoNewWord(wstring newWord, emWordAttribute attr)
{
/*
	WordDict* pDictWord = NULL;
	map<wstring, WordDict*>::iterator iter = m_mapNewWord.find(newWord);
	if (iter == m_mapNewWord.end())
	{
        pDictWord = new WordDict;
        pDictWord->bAutoNew = true;
        pDictWord->timeCount = 0;
        pDictWord->word = newWord;

        pDictWord->vctWordAttr.push_back(attr);
        pDictWord->bChn = false;

        for (wstring::iterator iter = newWord.begin(); iter != newWord.end(); iter++)
        {
            if (Dict::CharAttr(*iter, CharHan))
            {
            pDictWord->bChn = true;
            break;
            }
        }

        if (!AddItem(pDictWord))
        {
            delete pDictWord;
            Log::Write(L"Dict::AddAutoNewWord", L"Failed to add new word to dict. newWord = " + newWord);
            return false;
        }

        m_mapNewWord[newWord] = pDictWord;
	}

	Log::Write(L"Dict::AddAutoNewWord",
           newWord + L"\t" + DictAttr::WordAttributeToWStr(attr),
           LogTypeInforHigh);
    */
	return true;
}

bool Dict::DeleteWord(wstring strWord)
{
	unordered_map<wstring, WordDict*>::iterator iter = m_dictHash.find(strWord);
	if (iter == m_dictHash.end())
	{
        Log::Write(L"Dict::DeleteWord", L"Failed to find word " + strWord, LogTypeError);
        return false;
	}

	bool bSingle = false;
	if (!DeleteItem((strWord + WORD_END_SIGN).c_str(), &m_forHashMap, bSingle))
	{
        Log::Write(L"Dict::DeleteWord", L"Failed to find word " + strWord, LogTypeError);
        return false;
	}

	bSingle = false;
	if (!DeleteItem((Utility::ReverseString(strWord) + WORD_END_SIGN).c_str(), &m_backHashMap, bSingle))
	{
        Log::Write(L"Dict::DeleteWord", L"Failed to find word " + strWord, LogTypeError);
        return false;
	}

	if (iter->second->bAutoNew)
    {
        auto itNew = m_mapNewWord.find(strWord);
        if (itNew != m_mapNewWord.end())
            m_mapNewWord.erase(itNew);
    }

    delete iter->second;
    m_dictHash.erase(iter);

	return true;
}

bool Dict::DeleteItem(const wchar_t* word, unordered_map<wchar_t, void*> *hashMap, bool& bSingle)
{
	unordered_map<wchar_t, void*>::iterator iter = hashMap->find(word[0]);
	if (iter == hashMap->end())
	{
        return false;
	}

	if (word[0] == WORD_END_SIGN)
	{
        hashMap->erase(iter);
        if (hashMap->size() == 0)
            bSingle = true;
        else
            bSingle = false;

        return true;
	}

	if (!bSingle)
        return true;

	delete (unordered_map<wchar_t, void*>*)iter->second;
	hashMap->erase(iter);
	if (hashMap->size() > 0)
        bSingle = false;

	return true;
}

bool Dict::IsNewWord(wstring word)
{
	unordered_map<wstring, WordDict*>::iterator iter = m_dictHash.find(word);
	if (iter == m_dictHash.end())
    return false;

	return iter->second->bAutoNew;
}

bool Dict::RemoveFromNewWord(wstring word, bool bDelFromDict)
{
	unordered_map<wstring, WordDict*>::iterator iter = m_dictHash.find(word);
	if (iter == m_dictHash.end())
	{
        Log::Write(L"Dict::RemoveFromNewWord", L"Failed to find word " + word, LogTypeWarning);
        return false;
	}

	if (bDelFromDict)
	{
        if (!DeleteWord(word))
            return false;
	}
	else
        iter->second->bAutoNew = false;

	m_mapNewWord.erase(word);

	return true;
}

}
