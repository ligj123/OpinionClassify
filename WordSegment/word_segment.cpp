#include "word_segment.h"
#include "dict.h"
#include "text_func.h"
#include "exception_ex.h"
#include "utility.h"
#include <boost/locale.hpp>

namespace word_segment {

WordSegment::WordSegment(void)
{
}

WordSegment::~WordSegment(void)
{
}

bool WordSegment::SegmentWordMaxMatch(const wstring& strText, wstring &strResult)
{
    strResult = L"";
    vector<SegWord> vctResult;

    if (!SegmentWordMaxMatch(strText, vctResult))
        return false;

    for(vector<SegWord>::iterator iter = vctResult.begin(); iter != vctResult.end(); iter++)
    {
        strResult += iter->m_strDest;
        strResult += L"\n";
    }

    return true;
}

bool WordSegment::SegmentWordMaxMatch(const wstring& strText, vector<SegWord> &vctResult)
{
    try
    {
        vctResult.clear();

        vector<SegWord> vctExSplite;
        if (!SpliteTextByExplicitMarker(strText, vctExSplite))
        {
            return false;
        }

        vector<SegWord> vctMm;
        for (vector<SegWord>::iterator iter = vctExSplite.begin(); iter != vctExSplite.end(); iter++)
        {
            if (iter->m_Type != WordTypeUnknown)
            {
                vctMm.push_back(*iter);
                continue;
            }

            string forward = ForwordMaxMatchMethod(iter->m_strSour);
            string backward = BackwordMaxMatchMethod(iter->m_strSour);

            vector<SegWord> result;
            if (!MaxMatchMethod(iter->m_strSour, forward, backward, result))
            {
                vctMm.clear();
                return false;
            }

            vctMm.insert(vctMm.end(), result.begin(), result.end());
        }

        bool bStartOrSpace = true;
        for (vector<SegWord>::iterator iter = vctMm.begin(); iter != vctMm.end(); iter++)
        {
            if (iter->m_Type == WordTypeSpace)
            {
                bStartOrSpace = true;
                vctResult.push_back(*iter);
                continue;
            }

            if (!bStartOrSpace || (iter->m_Type != WordTypeSingleHan && iter->m_Type != WordTypeEng &&
                    iter->m_Type != WordTypeDict))
            {
                vctResult.push_back(*iter);
                continue;
            }

            bStartOrSpace = false;
            vector<SegWord>::iterator iter2 = iter;
            vector<SegWord>::iterator iter3 = vctMm.end();
            unordered_map<wchar_t, void*>* pHash = Dict::GetForHashMap();
            size_t pos = 0;
            wstring sour1, sour2;
            wstring dest1, dest2;
            bool bAllEn = true;

            int iSpace = -1;

            while (true)
            {
                unordered_map<wchar_t, void*>::iterator iHash = pHash->find(Dict::DestChar(iter2->m_strSour[pos]));
                if (!Dict::CharAttr(iter2->m_strSour[pos], CharEnglish))
                    bAllEn = false;

                if (iHash == pHash->end())
                    break;

                sour1 += iter2->m_strSour[pos];
                dest1 += Dict::DestChar(iter2->m_strSour[pos]);
                pos++;

                if (iHash->second == nullptr)
                    break;
                pHash = (unordered_map<wchar_t, void*>*)iHash->second;
                if (pos == iter2->m_strSour.size())
                {
                    if (pHash->find(WORD_END_SIGN) != pHash->end())
                    {
                        sour2 = sour1;
                        dest2 = dest1;
                        iter3 = iter2;

                        if (pHash->size() == 1)
                            break;
                    }

                    iter2++;
                    if (iter2 == vctMm.end())
                        break;

                    pos = 0;

                    if (iter2->m_Type == WordTypeSpace)
                    {
                        if (bAllEn)
                        {
                            iHash = pHash->find(L' ');
                            if (iHash != pHash->end())
                            {
                                dest1 += L' ';
                                pHash = (unordered_map<wchar_t, void*>*)iHash->second;
                            }
                        }
                        else
                        {
                            if (iSpace == -1)
                            {
                                if (dest1.size() >= 2)
                                    break;
                                else
                                    iSpace = (int)iter2->m_strSour.size();
                            }
                            else
                            {
                                if (iSpace != (int)iter2->m_strSour.size())
                                    break;
                            }
                        }

                        sour1 += iter2->m_strSour;
                        iter2++;
                        if (iter2 == vctMm.end())
                            break;
                    }
                }
            }

            if (iter3 == vctMm.end())
            {
                const WordDict* pDict = Dict::SearchWord(iter->m_strDest);
                if (pDict != NULL)
                {
                    iter->m_Type = WordTypeDict;
                    iter->m_pWordDict = pDict;
                }

                vctResult.push_back(*iter);
            }
            else
            {
                const WordDict* pDict = Dict::SearchWord(dest2);
                if (pDict == NULL)
                    throw Exception("Failed to find word " + boost::locale::conv::from_utf(dest1, "utf-8"));

                vctResult.push_back(SegWord(sour2, dest2, WordTypeDict, ReliabilityHigh, pDict));
                iter = iter3;
            }
        }
    }
    catch(exception& ex)
    {
        vctResult.clear();
        Log::Write("WordSegment::SegmentWordMaxMatch", ex.what(), LogTypeException);
        return false;
    }
    catch(...)
    {
        vctResult.clear();
        Log::Write(L"WordSegment::SegmentWordMaxMatch", L"Unknown error!", LogTypeException);
        return false;
    }

    return true;
}

bool WordSegment::SpliteTextByExplicitMarker(const wstring& strText, vector<SegWord> &vctResult)
{
    try
    {
        for (size_t i = 0; i < strText.size(); i++)
        {
            size_t end = 0;
            SegWord segWord;
            wstring str;

            if (strText[i] == L'\r' || strText[i] == L'\n')
            {
                str = strText[i];

                if (strText[i + 1] == L'\n')
                {
                    i++;
                    str += strText[i];
                }

                segWord.SetValue(str, L"\n", WordTypeLineBreak, ReliabilityAffirm);
                vctResult.push_back(segWord);
            }
            else if (Dict::CharAttr(strText[i], CharSpace))
            {
                size_t pos = TextFunc::FirstNoSpaceChar(strText, i);
                segWord.SetValue(strText.substr(i, pos - i), L"", WordTypeSpace, ReliabilityAffirm);
                vctResult.push_back(segWord);
                i = pos - 1;
            }
            else if (Dict::CharAttr(strText[i], CharEnNumber) && InfoIden::IDCardIden(strText, i, end, segWord))
            {
                vctResult.push_back(segWord);
                i = end;
            }
            else if (Dict::CharAttr(strText[i], CharEnToWord) && InfoIden::EmailIden(strText, i, end, segWord))
            {
                vctResult.push_back(segWord);
                i = end;
            }
            else if (Dict::CharAttr(strText[i], CharEnToWord) && DateTimeIden::IdenDateTime(strText, i, end, segWord))
            {
                vctResult.push_back(segWord);
                i = end;
            }
            else if ((Dict::CharAttr(strText[i], CharEnNumber) || Dict::DestChar(strText[i]) == L'(' || Dict::DestChar(strText[i]) == L'+') &&
                    InfoIden::PhoneMobileIden(strText, i, end, segWord))
            {
                vctResult.push_back(segWord);
                i = end;
            }
            else if (Dict::CharAttr(strText[i], CharEnToWord) && InfoIden::HttpIden(strText, i, end, segWord))
            {
                vctResult.push_back(segWord);
                i = end;
            }
            else if (Dict::CharAttr(strText[i], CharEnNumber) && InfoIden::NumberIden(strText, i, end, segWord))
            {
                vctResult.push_back(segWord);
                i = end;
            }
            else if (Dict::CharAttr(strText[i], CharEnToWord) && InfoIden::EngWordIden(strText, i, end, segWord))
            {
                vctResult.push_back(segWord);
                i = end;
            }
            else if (Dict::CharAttr(strText[i], CharToWord) && InfoIden::WordIden(strText, i, end, segWord))
            {
                vctResult.push_back(segWord);
                i = end;
            }
            else
            {
                segWord.SetValue(strText.substr(i, 1), L"", WordTypeNoWordChar, ReliabilityHigh, NULL, Dict::SearchChar(strText[i]));
                vctResult.push_back(segWord);
            }

#ifdef _DEBUG_LOG
            segWord.WriteToLog();
#endif
        }
    }
    catch(exception& ex)
    {
        vctResult.clear();
        Log::Write("WordSegment::SpliteTextByExplicitMarker", ex.what(), LogTypeException);
        return false;
    }
    catch(...)
    {
        vctResult.clear();
        Log::Write(L"WordSegment::SpliteTextByExplicitMarker", L"Unknown error!", LogTypeException);
        return false;
    }

    return true;
}

string WordSegment::ForwordMaxMatchMethod(const wstring& strText)
{
    string marker(strText.size(), ' ');

    for (size_t i = 0; i < strText.size(); i++)
    {
        marker[i] = 'B';
        unordered_map<wchar_t, void*>* pHash = Dict::GetForHashMap();
        int lMarker = -1;
        bool bEnStart = Dict::CharAttr(strText[i], CharEnToWord);
        bool bAllEn = true;
        bool bDictFind = true;
        int iCharHanCount = 0;
        int iCharEnCount = 0;

        for(size_t j = i; j <= strText.size(); j++)
        {
            if (Dict::CharAttr(strText[j], CharHan))
                iCharHanCount++;
            if (Dict::CharAttr(strText[j], CharEnglish))
                iCharEnCount++;

            if (bEnStart && bAllEn)
            {
                if (bDictFind)
                {
                    if (!Dict::CharAttr(strText[j], CharEnToWord))
                        lMarker = (int)j - 1;
                }
                else
                {
                    if (!Dict::CharAttr(strText[j], CharEnToWord))
                    {
                        if (j - 1 == i)
                            marker[i] = 'S';
                        else
                        {
                            marker[j - 1] = 'E';
                            i = j - 1;
                        }
                        break;
                    }
                    else
                    {
                        continue;
                    }
                }
            }

            unordered_map<wchar_t, void*>::iterator iter;

            if (j >= strText.size())
                iter = pHash->end();
            else
                iter = pHash->find(Dict::DestChar(strText[j]));

            if (!Dict::CharAttr(strText[j], CharEnToWord))
                bAllEn = false;

            if (iter == pHash->end())
            {
                bDictFind = false;
                if (bAllEn && j < strText.size())
                    continue;

                if (lMarker < 0)
                {
                    if (bAllEn)
                        marker[i] = 'S';
                    else
                        marker[i] = 'U';
                }
                else
                {
                    if ((size_t)lMarker == i)
                        marker[i] = 'S';
                    else
                        marker[lMarker] = 'E';

                    i = lMarker;
                }
                break;
            }
            else
            {
                pHash = (unordered_map<wchar_t, void*>*)iter->second;

                if (pHash == NULL)
                    throw Exception("Dictionary has error!");

                if (pHash->find(WORD_END_SIGN) != pHash->end())
                {
                    if (pHash->size() == 1)
                    {
                        if (j == i)
                            marker[j] = 'S';
                        else
                            marker[j] = 'E';

                        i = j;
                        break;
                    }
                    else
                    {
                        lMarker = (int)j;
                    }
                }
            }
        }
    }

    return marker;
}

string WordSegment::BackwordMaxMatchMethod(const wstring& strText)
{
    string marker(strText.size(), ' ');

    for (int i = (int)strText.size() - 1; i >= 0; i--)
    {
        marker[i] = 'E';
        unordered_map<wchar_t, void*>* pHash = Dict::GetBackHashMap();
        int lMarker = -1;
        bool bEnStart = Dict::CharAttr(strText[i], CharEnToWord);
        bool bAllEn = true;
        bool bDictFind = true;
        int iCharHanCount = 0;
        int iCharEnCount = 0;

        for(int j = i; j >= -1; j--)
        {
            wchar_t c = L'\0';
            if (j >= 0)
                c = strText[j];

            if (Dict::CharAttr(c, CharHan))
                iCharHanCount++;
            if (Dict::CharAttr(c, CharEnglish))
                iCharEnCount++;

            if (bEnStart && bAllEn)
            {
                if (bDictFind)
                {
                    if (!Dict::CharAttr(c, CharEnToWord))
                        lMarker = j + 1;
                }
                else
                {
                    if (!Dict::CharAttr(c, CharEnToWord))
                    {
                        if (j + 1 == i)
                            marker[i] = 'S';
                        else
                        {
                            marker[j + 1] = 'B';
                            i = j + 1;
                        }
                        break;
                    }
                    else
                    {
                        continue;
                    }
                }
            }

            unordered_map<wchar_t, void*>::iterator iter;

            if (j < 0)
                iter = pHash->end();
            else
                iter = pHash->find(Dict::DestChar(c));

            if (!Dict::CharAttr(c, CharEnToWord))
                bAllEn = false;

            if (iter == pHash->end())
            {
                bDictFind = false;
                if (bAllEn && j >= 0)
                    continue;

                if (lMarker < 0)
                {
                    if (bAllEn)
                        marker[i] = 'S';
                    else
                        marker[i] = 'U';
                }
                else
                {
                    if (lMarker == i)
                        marker[i] = 'S';
                    else
                        marker[lMarker] = 'B';

                    i = lMarker;
                }
                break;
            }
            else
            {
                pHash = (unordered_map<wchar_t, void*>*)iter->second;

                if (pHash == NULL)
                    throw Exception("Dictionary has error!");

                if (pHash->find(WORD_END_SIGN) != pHash->end())
                {
                    if (pHash->size() == 1)
                    {
                        if (j == i)
                            marker[j] = 'S';
                        else
                            marker[j] = 'B';

                        i = j;
                        break;
                    }
                    else
                    {
                        lMarker = j;
                    }
                }
            }
        }
    }

    return marker;
}

bool WordSegment::MaxMatchMethod(const wstring& strText, const string& forMarker, const string& backMarker, vector<SegWord> &vctResult)
{
    if (strText.size() != forMarker.size() || strText.size() != backMarker.size())
    {
        Log::Write(L"WordSegment::MaxMatchMethod", L"Forwark marker and backword marker should have same length with the input text!", LogTypeError);
        Log::Write(L"WordSegment::MaxMatchMethod",L"Source: " + strText, LogTypeError);
        Log::Write("WordSegment::MaxMatchMethod","Forword: " + forMarker, LogTypeError);
        Log::Write("WordSegment::MaxMatchMethod","Backword: " + backMarker, LogTypeError);

        return false;
    }

    for(size_t i = 0; i < strText.size(); i++)
    {
        size_t start =  i;

        while (i < strText.size() && forMarker[i] == backMarker[i])
        {
            if (forMarker[i] == 'S' || forMarker[i] == 'U')
            {
                wstring sour = strText.substr(i, 1);
                wstring dest = Dict::GetDestString(sour);
                const WordDict* pDict = Dict::SearchWord(dest);

                if (pDict != NULL)
                {
                    vctResult.push_back(SegWord(sour, dest, WordTypeDict, ReliabilityHigh, pDict, Dict::SearchChar(dest[0])));
                }
                else
                {
                    if (Dict::CharAttr(dest[0], CharHan))
                        vctResult.push_back(SegWord(sour, dest, WordTypeSingleHan, ReliabilityHigh, NULL, Dict::SearchChar(dest[0])));
                    else if (Dict::CharAttr(dest[0], CharEnToWord))
                        vctResult.push_back(SegWord(sour, dest, WordTypeEng, ReliabilityHigh, NULL, Dict::SearchChar(dest[0])));
                    else
                        vctResult.push_back(SegWord(sour, dest, WordTypeNoWordChar, ReliabilityHigh, NULL, Dict::SearchChar(dest[0])));
                }

                start = i + 1;
            }
            else if (forMarker[i] == 'B')
            {
                start = i;
            }
            else if (forMarker[i] == 'E')
            {
                wstring sour = strText.substr(start, i - start + 1);
                wstring dest = Dict::GetDestString(sour);
                const WordDict* pDict = Dict::SearchWord(dest);

                if (pDict == NULL)
                {
                    vector<SegWord> vct;
                    if (!SpliteTextByExplicitMarker(sour, vct))
                        return false;

                    vctResult.insert(vctResult.end(), vct.begin(), vct.end());
                }
                else
                    vctResult.push_back(SegWord(sour, dest, WordTypeDict, ReliabilityHigh, pDict, NULL));
                start = i + 1;
            }

            i++;

            if (i >= strText.size())
                break;
        }

        while(i + 1 < strText.size())
        {
            if ((forMarker[i] == backMarker[i]) && forMarker[i] == 'E')
            {
                break;
            }

            if (forMarker[i + 1] == backMarker[i + 1] && (forMarker[i + 1] == 'S' || forMarker[i + 1] == 'B'))
            {
                break;
            }

            i++;
        }

        if (i >= strText.size())
            break;

        vector<SegWord> vctWords;
        if (!SpliteAmbiguousWord(strText.substr(start, i - start + 1),
                forMarker.substr(start, i - start + 1), backMarker.substr(start, i - start + 1), vctWords))
            return false;

        vctResult.insert(vctResult.end(), vctWords.begin(), vctWords.end());
    }
#ifdef _DEBUG_LOG
    for (vector<SegWord>::iterator iter = vctResult.begin(); iter != vctResult.end(); iter++)
    {
        iter->WriteToLog();
    }
#endif
    return true;
}

bool WordSegment::SpliteAmbiguousWord(const wstring& strText, const string& forMarker, const string& backMarker, vector<SegWord> &vctResult)
{
    int iForU = 0;
    int iForS = 0;
    int iForB = 0;
    int iBackU = 0;
    int iBackS = 0;
    int iBackB = 0;

    for(int i = 0; i < (int)forMarker.size(); i++)
    {
        if (forMarker[i] == 'U')
            iForU++;
        if (forMarker[i] == 'S')
            iForS++;
        if (forMarker[i] == 'B')
            iForB++;

        if (backMarker[i] == 'U')
            iBackU++;
        if (backMarker[i] == 'S')
            iBackS++;
        if (backMarker[i] == 'B')
            iBackB++;
    }

    string marker = "";

    if (iForU < iBackU)
        marker = forMarker;
    else if (iBackU < iForU)
        marker = backMarker;
    else
    {
        if (iForS < iBackS)
            marker = forMarker;
        else
            marker = backMarker;
    }

    if (marker.size() > 0)
    {
        int start = 0;

        for (int i = 0; i < (int)marker.size(); i++)
        {
            if (marker[i] == 'U' || marker[i] == 'S')
            {
                wstring sour = strText.substr(start, 1);
                wstring dest = Dict::GetDestString(sour);
                const WordDict* pDict = Dict::SearchWord(dest);

                if (pDict != NULL)
                    vctResult.push_back(SegWord(sour, dest, WordTypeDict, ReliabilityMid, pDict, Dict::SearchChar(dest[0])));
                else if (Dict::CharAttr(dest[0], CharHanToWord))
                    vctResult.push_back(SegWord(sour, dest, WordTypeSingleHan, ReliabilityMid, NULL, Dict::SearchChar(dest[0])));
                else if (Dict::CharAttr(dest[0], CharEnToWord))
                    vctResult.push_back(SegWord(sour, dest, WordTypeEng, ReliabilityMid, NULL, Dict::SearchChar(dest[0])));
                else
                    vctResult.push_back(SegWord(sour, dest, WordTypeNoWordChar, ReliabilityMid, NULL, Dict::SearchChar(dest[0])));

                start++;
            }
            else if (marker[i] == 'B')
            {
                start = i;
            }
            else if (marker[i] == 'E')
            {
                wstring sour = strText.substr(start, i - start + 1);
                wstring dest = Dict::GetDestString(sour);
                const WordDict* pDict = Dict::SearchWord(dest);

                if (pDict == NULL)
                {
                    vector<SegWord> vct;
                    if (!SpliteTextByExplicitMarker(sour, vct))
                        return false;

                    vctResult.insert(vctResult.end(), vct.begin(), vct.end());
                }
                else
                    vctResult.push_back(SegWord(sour, dest, WordTypeDict, ReliabilityMid, pDict));
            }
        }

        return true;
    }

    return true;
}

bool WordSegment::SegmentWordMinMatch(const wstring& strText, wstring& strResult)
{
    strResult = L"";
    vector<SegWord> vctResult;

    if (!SegmentWordMinMatch(strText, vctResult))
        return false;

    for(vector<SegWord>::iterator iter = vctResult.begin(); iter != vctResult.end(); iter++)
    {
        strResult += iter->m_strDest;
        strResult += L"\n";
    }

    return true;
}

bool WordSegment::SegmentWordMinMatch(const wstring& strText, vector<SegWord>& vctResult)
{
    try
    {
        vctResult.clear();

        vector<SegWord> vctExSplite;
        if (!SpliteTextByExplicitMarker(strText, vctExSplite))
        {
            return false;
        }

        vector<SegWord> vctMm;
        for (vector<SegWord>::iterator iter = vctExSplite.begin(); iter != vctExSplite.end(); iter++)
        {
            if (iter->m_Type != WordTypeUnknown)
            {
                vctMm.push_back(*iter);
                continue;
            }

            wstring ss;
            for (wstring::iterator itws = iter->m_strSour.begin(); itws != iter->m_strSour.end(); itws++)
            {
                wchar_t c = *itws;
                if (Dict::CharAttr(c, CharEnToWord))
                {
                    ss += c;
                    continue;
                }

                if (ss.size() > 0)
                {
                    vctMm.push_back(SegWord(ss, L"", WordTypeEng, ReliabilityHigh));
                    ss.clear();
                }

                if (Dict::CharAttr(c, CharHanToWord))
                {
                    wstring str = wstring(1, c);
                    vctMm.push_back(SegWord(str, L"", WordTypeSingleHan, ReliabilityHigh, Dict::SearchWord(str), Dict::SearchChar(*itws)));
                }
                else if (Dict::CharAttr(c, CharSpace))
                {
                    vctMm.push_back(SegWord(wstring(1, c), L"", WordTypeSpace, ReliabilityHigh));
                }
                else
                {
                    vctMm.push_back(SegWord(wstring(1, c), L"", WordTypeNoWordChar, ReliabilityHigh));
                }
            }
        }

        vctResult = vctMm;
    }
    catch(exception& ex)
    {
        vctResult.clear();
        Log::Write("WordSegment::SegmentWordMinMatch", ex.what(), LogTypeException);
        return false;
    }
    catch(...)
    {
        vctResult.clear();
        Log::Write(L"WordSegment::SegmentWordMinMatch", L"Unknown error!", LogTypeException);
        return false;
    }

    return true;
}

}
