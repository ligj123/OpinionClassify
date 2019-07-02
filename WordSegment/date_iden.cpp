#include "date_iden.h"
#include "dict.h"
#include "text_func.h"
#include "exception_ex.h"

namespace word_segment {
unordered_map<wstring, int> DateTimeIden::m_mapMonth;

void DateTimeIden::FillMapMonth()
{
    if (m_mapMonth.size() == 0)
    {
        m_mapMonth.insert(pair<wstring, int>(L"january", 1));
        m_mapMonth.insert(pair<wstring, int>(L"jan", 1));
        m_mapMonth.insert(pair<wstring, int>(L"february", 2));
        m_mapMonth.insert(pair<wstring, int>(L"feb", 2));
        m_mapMonth.insert(pair<wstring, int>(L"march", 3));
        m_mapMonth.insert(pair<wstring, int>(L"mar", 3));
        m_mapMonth.insert(pair<wstring, int>(L"april", 4));
        m_mapMonth.insert(pair<wstring, int>(L"apr", 4));
        m_mapMonth.insert(pair<wstring, int>(L"may", 5));
        m_mapMonth.insert(pair<wstring, int>(L"june", 6));
        m_mapMonth.insert(pair<wstring, int>(L"jun", 6));
        m_mapMonth.insert(pair<wstring, int>(L"july", 7));
        m_mapMonth.insert(pair<wstring, int>(L"jul", 7));
        m_mapMonth.insert(pair<wstring, int>(L"august", 8));
        m_mapMonth.insert(pair<wstring, int>(L"aug", 8));
        m_mapMonth.insert(pair<wstring, int>(L"september", 9));
        m_mapMonth.insert(pair<wstring, int>(L"sep", 9));
        m_mapMonth.insert(pair<wstring, int>(L"sept", 9));
        m_mapMonth.insert(pair<wstring, int>(L"sep.", 9));
        m_mapMonth.insert(pair<wstring, int>(L"october", 10));
        m_mapMonth.insert(pair<wstring, int>(L"oct", 10));
        m_mapMonth.insert(pair<wstring, int>(L"november", 11));
        m_mapMonth.insert(pair<wstring, int>(L"nov", 11));
        m_mapMonth.insert(pair<wstring, int>(L"december", 12));
        m_mapMonth.insert(pair<wstring, int>(L"dec", 12));
    }
}

bool DateTimeIden::IdenDateTime(const wstring& strText, size_t iStart, size_t& iEnd, SegWord& segWord)
{
    if (m_mapMonth.size() == 0)
        FillMapMonth();

    wstring result;
    size_t pos = iStart;
    size_t end = 0;
    emReliability reli = ReliabilityUnknown;

    if (Dict::CharAttr(strText[pos], CharEnNumber))
        reli = IsDateTime(strText, pos, end, result);
    else if (Dict::CharAttr(strText[pos], CharEnToWord))
        reli = IsEngDateTime(strText, pos, end, result);

    if (reli == ReliabilityUnknown)
        return false;

    iEnd = end;
    segWord.SetDateValue(strText.substr(iStart, iEnd - iStart + 1), result, WordTypeDate, reli);

    return true;
}

emReliability DateTimeIden::IsEngDateTime(const wstring& strText, size_t iStart, size_t& iEnd, wstring& result)
{
    return ReliabilityUnknown;
}

emReliability DateTimeIden::IsDateTime(const wstring& strText, size_t iStart, size_t& iEnd, wstring& result)
{
    wstring str1, str2, str3;
    int year = 0, month = 0, day = 0;
    size_t dPos3 = 0;
    size_t cPos3 = 0;
    wchar_t cSplite1 = 0, cSplite2 = 0, cSplite3 = 0;

    wstring ymd = L"、．.,-/"; 
    wstring ym = L"年";
    wstring md = L"月";
    wstring dd = L"日"; 

    bool bChnType = false;

    str1 = TextFunc::PickupEnNumber(strText, iStart);
    if (str1.size() != 1 && str1.size() != 2 && str1.size() != 4)
        return ReliabilityUnknown;

    size_t pos = TextFunc::FirstNoSpaceChar(strText, iStart + str1.size());
    cSplite1 = Dict::DestChar(strText[pos]);

    if (ym.find(cSplite1) != wstring::npos)
        bChnType = true;
    else if (ymd.find(cSplite1) == wstring::npos)
        return ReliabilityUnknown;

    pos = TextFunc::FirstNoSpaceChar(strText, pos + 1);
    str2 = TextFunc::PickupEnNumber(strText, pos);

    if (str2.size() != 1 && str2.size() != 2)
        return ReliabilityUnknown;

    pos = TextFunc::FirstNoSpaceChar(strText, pos + str2.size());
    cSplite2 = Dict::DestChar(strText[pos]);

    if (cSplite2 != cSplite1 && md.find(cSplite2) == wstring::npos)
        return ReliabilityUnknown;

    pos = TextFunc::FirstNoSpaceChar(strText, pos + 1);
    str3 = TextFunc::PickupEnNumber(strText, pos);
    if (str3.size() != 1 && str3.size() != 2 && str3.size() != 4)
        return ReliabilityUnknown;

    dPos3 = pos + str3.size() - 1;

    if (bChnType)
    {
        pos = TextFunc::FirstNoSpaceChar(strText, pos + str3.size());
        cSplite3 = Dict::DestChar(strText[pos]);
        if (dd.find(cSplite3) == wstring::npos)
            return ReliabilityUnknown;

        cPos3 = pos;
    }

    if (str3.size() == 4)
    {
        if (!Utility::WStringToInt(str3, year))
            return ReliabilityUnknown;
        if (!Utility::WStringToInt(str1, month))
            return ReliabilityUnknown;
        if (!Utility::WStringToInt(str2, day))
            return ReliabilityUnknown;

        if (month > 12)
        {
            int tmp = month;
            month = day;
            day = tmp;
        }
    }
    else
    {
        if (!Utility::WStringToInt(str1, year))
            return ReliabilityUnknown;
        if (!Utility::WStringToInt(str2, month))
            return ReliabilityUnknown;
        if (!Utility::WStringToInt(str3, day))
            return ReliabilityUnknown;
    }

    if (year < 100)
    {
        if (year < YEAR_BREAK)
            year += 2000;
        else
            year += 1900;
    }

    if (year > MAX_YEAR || year < MIN_YEAR)
        return ReliabilityUnknown;

    if (month > 12 || month < 1)
        return ReliabilityUnknown;

    if (day > 31 || day < 1)
        return ReliabilityUnknown;

    if (bChnType)
        iEnd = cPos3;
    else
        iEnd = dPos3;

    result = Utility::IntToWStr(year) + L"/" + Utility::IntToWStr(month) + L"/" + Utility::IntToWStr(day);

    wstring sHour = L"时点";
    wstring sMinute = L"分";
    wstring sSecond = L"秒";
    wstring sTime = L":";
    int iHour = -1, iMin = -1, iSec = -1;
    bChnType = false;

    pos = TextFunc::FirstNoSpaceChar(strText, pos + 1);
    str1 = TextFunc::PickupEnNumber(strText, pos);
    if (str1.size() == 1 || str1.size() == 2)
        iHour = Utility::WStrToLong(str1);

    if (iHour  < 0 || iHour > 23)
        return ReliabilityHigh;

    cPos3 = pos;
    pos = TextFunc::FirstNoSpaceChar(strText, pos + str1.size());
    wchar_t cSplite = Dict::DestChar(strText[pos]);
    if (sHour.find(cSplite) != wstring::npos)
        bChnType = true;
    else if (sTime.find(cSplite) != wstring::npos)
        return ReliabilityHigh;

    dPos3 = pos;
    pos = TextFunc::FirstNoSpaceChar(strText, pos + 1);
    str1 = TextFunc::PickupEnNumber(strText, pos);
    if (str1.size() == 1 || str1.size() == 2)
        iMin = Utility::WStrToLong(str1);

    if (iMin < 0 || iMin > 59)
    {
        if (bChnType)
        {
            iEnd = dPos3;
            result += L" " + Utility::IntToWStr(iHour);
        }

        return ReliabilityHigh;
    }

    cPos3 = pos + str1.size() - 1;
    pos = TextFunc::FirstNoSpaceChar(strText, pos + str1.size());
    cSplite = Dict::DestChar(strText[pos]);
    if (sMinute.find(cSplite) == wstring::npos && sTime.find(cSplite) == wstring::npos)
    {
        iEnd = cPos3;
        result += L" " + Utility::IntToWStr(iHour) + L":" + Utility::IntToWStr(iMin);
        return ReliabilityHigh;
    }

    dPos3 = pos;
    pos = TextFunc::FirstNoSpaceChar(strText, pos + 1);
    str1 = TextFunc::PickupEnNumber(strText, pos);
    if (str1.size() == 1 || str1.size() == 2)
        iSec = Utility::WStrToLong(str1);

    if (iSec < 0 || iSec > 59)
    {
        if (bChnType)
            iEnd = dPos3;
        else
            iEnd = cPos3;

        result += L" " + Utility::IntToWStr(iHour) + L":" + Utility::IntToWStr(iMin);
        return ReliabilityHigh;
    }

    iEnd = pos + str1.size() - 1;
    if (bChnType)
    {
        pos = TextFunc::FirstNoSpaceChar(strText, pos + str1.size());
        cSplite = Dict::DestChar(strText[pos]);
        if (sSecond.find(cSplite) != wstring::npos)
            iEnd = pos;
        else
        {
            iEnd = dPos3;
            result += L" " + Utility::IntToWStr(iHour) + L":" + Utility::IntToWStr(iMin);
            return ReliabilityHigh;
        }
    }

    result += L" " + Utility::IntToWStr(iHour) + L":" + Utility::IntToWStr(iMin) + L":" + Utility::IntToWStr(iSec);
    return ReliabilityHigh;
}

}
