#include "info_iden.h"
#include "dict.h"
#include "text_func.h"
#include "utility.h"

namespace word_segment {
bool InfoIden::PhoneMobileIden(const wstring& strText, size_t iStart, size_t& iEnd, SegWord& segWord)
{
    return PhoneMobileIden_CN(strText, iStart, iEnd, segWord);
}

bool InfoIden::PhoneMobileIden_CN(const wstring& strText, size_t iStart, size_t& iEnd, SegWord& segWord)
{
    wstring number;
    size_t pos = iStart;
    bool bCnCode = false;

    bool bLeftBracket = Dict::CharEqual(strText[pos], '(');
    if (bLeftBracket)
        pos++;

    bool bLeftPlus = Dict::CharEqual(strText[pos], L'+');
    if (bLeftPlus)
        pos++;

    bool bRightBracket = false;
    if (!bLeftBracket && !bLeftPlus && !Dict::CharAttr(strText[iStart], CharEnNumber))
        return false;

    number = Utility::SBCtoDBC(TextFunc::PickupEnNumber(strText, pos));
    pos += number.size();

    if (number.substr(0, 2) == L"86")
    {
        number = number.substr(2);
        bCnCode = true;
    }
    else if (number.substr(0, 3) == L"086")
    {
        number = number.substr(3);
        bCnCode = true;
    }

    while (number.size() < 10)
    {
        if (Dict::CharEqual(strText[pos], L' '))
        {
            size_t pos2 = TextFunc::FirstNoSpaceChar(strText, pos);
            if (pos2 - pos >= 2)
            break;
            pos = pos2;
            continue;
        }

        if (bLeftBracket && !bRightBracket && Dict::CharEqual(strText[pos], L')'))
        {
            bRightBracket = true;
            pos++;
            continue;
        }

        if (Dict::CharAttr(strText[pos], CharConnector))
        {
            pos++;
            continue;
        }

        if (Dict::CharAttr(strText[pos], CharEnNumber))
        {
            wstring tmp = TextFunc::PickupEnNumber(strText, pos);
            number += tmp;
            pos += tmp.size();
            continue;
        }

        break;
    }

    if (number.size() == 11 && number[0] == L'1' && (number[1] == L'3' || number[1] == L'4' || number[1] == L'5' || number[1] == L'8'))
    {
        iEnd = pos - 1;
        segWord = SegWord(strText.substr(iStart, pos - iStart),number, WordTypeMobile, ReliabilityAffirm, NULL, NULL);
        return true;
    }

    if (bCnCode && number.size() == 10 && number[0] != L'0')
        number = L'0' + number;

    if (number[0] == L'0' && number.size() > 10)
    {
        iEnd = pos - 1;
        segWord = SegWord(strText.substr(iStart, pos - iStart),number, WordTypePhone, ReliabilityHigh, NULL, NULL);
        return true;
    }

    return false;
}

bool InfoIden::EmailIden(const wstring& strText, size_t iStart, size_t& iEnd, SegWord& segWord)
{
    wstring str;
    size_t m = iStart;
    int iDotCount = 0;
    int iAt = 0;

    for (; m < strText.size(); m++)
    {
        wchar_t c = Dict::DestChar(strText[m]);
        if (c == L'.')
        {
            if (iAt > 0)
                iDotCount++;

            str += c;
            continue;
        }

        if (c == L'@')
        {
            iAt++;
            str += c;
            continue;
        }

        if (Dict::CharAttr(c, CharEnToWord))
        {
            str += c;
            continue;
        }

        if (c == '-' || c == '+')
        {
            str += c;
            continue;
        }

        break;
    }

    if (iAt == 1 && iDotCount >= 1)
    {
        iEnd = m - 1;
        segWord = SegWord(strText.substr(iStart, m - iStart), str, WordTypeEmail, ReliabilityHigh, NULL, NULL);

        return true;
    }

    return false;
}

bool InfoIden::HttpIden(const wstring& strText, size_t iStart, size_t& iEnd, SegWord& segWord)
{
    wstring str = TextFunc::PickupEngWord(strText, iStart);
    size_t m = iStart + str.size();
    bool bHttp = false;
    int iDotCount = 0;

    if ((Utility::ToUpper(str).compare(L"HTTP") == 0 ||
                Utility::ToUpper(str).compare(L"HTTPS") == 0 ||
                Utility::ToUpper(str).compare(L"FTP") == 0) &&
                Dict::CharEqual(strText[m], L':'))
    {
        str += Dict::DestChar(strText[m]);
        bHttp = true;
        m++;
    }

    if (!bHttp)
        return false;

    for (; m < strText.size(); m++)
    {
        wchar_t c = Dict::DestChar(strText[m]);
        if (c == L'.')
        {
            iDotCount++;
            continue;
        }

        if (Dict::CharAttr(c, CharEnToWord))
            continue;

        if (c == '/' || c == '?' || c == '&' || c == '=')
            continue;
        break;
    }

    iEnd = m - 1;
    segWord = SegWord(strText.substr(iStart, m - iStart), str, WordTypeHttp, ReliabilityHigh, NULL, NULL);

    return true;
}

bool InfoIden::IDCardIden(const wstring& strText, size_t iStart, size_t& iEnd, SegWord& segWord)
{
    wstring str = TextFunc::PickupEnNumber(strText, iStart);

    if (str.size() == 18 ||
            (str.size() == 17 && Dict::CharEqual(strText[iStart + 17], L'x') && Dict::CharAttr(strText[iStart + 18], CharEnToWord)))
    {
        if (str.size() == 17)
            str += L'X';

        iEnd = iStart + 17;
        segWord = SegWord(strText.substr(iStart, 18), str, WordTypeIDCard, ReliabilityHigh, NULL, NULL);
        return true;
    }

    return false;
}

bool InfoIden::NumberIden(const wstring& strText, size_t iStart, size_t& iEnd, SegWord& segWord)
{
    if (!Dict::CharAttr(strText[iStart], CharEnNumber))
        return false;

    wstring number1 = TextFunc::PickupEnNumber(strText, iStart, true);
    size_t pos = iStart + number1.size();

    if (Dict::CharAttr(strText[pos], CharEnToWord))
        return false;

    segWord.SetValue(number1, L"", WordTypeNumber, ReliabilityHigh);
    iEnd = iStart + number1.size() - 1;

    return true;
}

bool InfoIden::EngWordIden(const wstring& strText, size_t iStart, size_t& iEnd, SegWord& segWord)
{
    size_t i = 0;
    wstring str;
    bool bDigital = false;;

    for (i = iStart; i < strText.size(); i++)
    {
        if (!Dict::CharAttr(strText[i], CharEnToWord))
        {
            if (Dict::CharAttr(strText[i], CharToWord))
            return false;

            break;
        }

        if (!bDigital && Dict::CharAttr(strText[i], CharNumber))
        {
            if (str.size() >= 3 && Dict::SearchWord(str) != NULL)
            break;
        }

        str += strText[i];
    }

    if (iStart == i)
        return false;

    iEnd = i - 1;
    segWord.SetValue(strText.substr(iStart, i - iStart), L"", WordTypeEng, ReliabilityHigh);

    return true;
}

bool InfoIden::WordIden(const wstring& strText, size_t iStart, size_t& iEnd, SegWord& segWord)
{
    size_t i = 0;
    for (i = iStart; i < strText.size(); i++)
    {
        if (!Dict::CharAttr(strText[i], CharToWord))
            break;
    }

    if (iStart == i)
    return false;

    iEnd = i - 1;
    segWord.SetValue(strText.substr(iStart, i - iStart), L"", WordTypeUnknown, ReliabilityHigh);

    return true;
}

}
