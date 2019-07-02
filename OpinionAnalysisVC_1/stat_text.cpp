#include "stat_text.h"

#include <word_segment.h>

namespace opinion_analysis {

StatText::StatText(void)
{
}


StatText::~StatText(void)
{
}

bool StatText::CalcText(const wstring& strText, Category type)
{
    WordSegment wordSeg;

    vector<SegWord> vct;
    if (!wordSeg.SegmentWordMaxMatch(strText, vct))
        return false;

    return CalcText(vct, type);
}

bool StatText::CalcText(const vector<SegWord>& vct, Category type)
{
    try
    {
        m_typeText = type;
        m_vctWord = vct;

        int ii = -1;
        for (auto iter = m_vctWord.begin(); iter != m_vctWord.end(); iter++)
        {
            ii++;
            if (iter->m_Type != WordTypeDict || iter->m_pWordDict == nullptr)
                continue;

            bool bReserve = false;
            for (auto itAttr = iter->m_pWordDict->vctWordAttr.begin(); itAttr != iter->m_pWordDict->vctWordAttr.end(); itAttr++)
            {
                if ((*itAttr &0xF0) == 0x10 || (*itAttr &0xF0) == 0x20 || *itAttr == WordDistinguish
                    || *itAttr == WordAdjective || *itAttr == WordDescriptive || *itAttr == WordAdverb)
                {
                    bReserve = true;
                    break;
                }
            }

            if (!bReserve)
                continue;

            auto itStat = m_mapWordStat.find(iter->m_strDest);
            if (itStat == m_mapWordStat.end())
            {
                m_mapWordStat.insert(pair<wstring, WordStat>(iter->m_strDest, WordStat(iter->m_pWordDict)));
                itStat = m_mapWordStat.find(iter->m_strDest);
            }

            itStat->second.iShowCount++;
            itStat->second.vctPosition.push_back(ii);
        }
    }
    catch(exception& ex)
	{
        Log::Write("StatText::CalcText", ex.what(), LogTypeException);
        return false;
	}
	catch(...)
	{
        Log::Write(L"StatText::CalcText", L"Failed to save words' statistical information, Unknown error!", LogTypeException);
        return false;
	}

    return true;
}

}
