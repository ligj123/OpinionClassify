#include "seg_word.h"

namespace word_segment {

SegWord::SegWord(void)
{
    Clear();
}

SegWord::SegWord(wstring sour, wstring dest, emWordType type, emReliability relia,
                  const WordDict* pWordDict, const CharDict* pCharDict)
{
    Clear();
    m_strSour = sour;

    if (dest.size() == 0)
        m_strDest = sour;
    else
        m_strDest = dest;

    m_Type = type;
    m_Relia = relia;

    m_pWordDict = pWordDict;
    m_pCharDict = pCharDict;
}

SegWord::~SegWord(void)
{
}

void SegWord::SetValue(wstring sour, wstring dest, emWordType type, emReliability relia,
                       const WordDict* pWordDict, const CharDict* pCharDict)
{
    Clear();
    m_strSour = sour;
    if (dest.size() == 0)
        m_strDest = sour;
    else
        m_strDest = dest;
    m_Type = type;
    m_Relia = relia;

    m_pWordDict = pWordDict;
    m_pCharDict = pCharDict;
}

void SegWord::SetDateValue(wstring sour, wstring dest, emWordType type, emReliability relia)
{
    Clear();
    m_strSour = sour;
    m_strDest = dest;
    m_Type = type;
    m_Relia = relia;
    m_datetime.FromString(dest);
}

void SegWord::Clear()
{
    m_strSour = L"";
    m_strDest = L"";
    m_Type = WordTypeNull;
    m_Relia = ReliabilityUnknown;
    m_pWordDict = NULL;
    m_pCharDict = NULL;
    m_datetime.Clear();
}

bool SegWord::IsNeedWord(emWordAttribute attr)
{
    if (m_pWordDict == NULL)
    	return false;

    return m_pWordDict->IsNeedWord(attr);
}

void SegWord::WriteToLog()
{
    wstring str;

  	str = L"Source: " + m_strSour + L"\tDestination: " + m_strDest +
    L"\tWordType: " + DictAttr::WordTypeToWStr(m_Type) + L"\tReliability: " + DictAttr::ReliabilityToWStr(m_Relia);

    Log::Write(L"CSegWord::WriteLog", str);
}

}
