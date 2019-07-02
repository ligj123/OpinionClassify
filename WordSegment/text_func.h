#ifndef TEXT_FUNC_H
#define TEXT_FUNC_H

#include "header.h"
#include "dict_attribute.h"
#include "dict.h"
#include <string>

using namespace std;
namespace word_segment {
class TextFunc
{
public:
    static size_t FirstNoSpaceChar(const wstring& strText, size_t start)
    {
        for (size_t i = start; i < strText.size(); i++)
        {
            if (!Dict::CharAttr(strText[i], CharSpace))
                return i;

            if (strText.at(i) == L'\r' || strText.at(i) == L'\n')
                break;
        }

        return strText.size();
    }

    static size_t FirstSpaceChar(const wstring& strText, size_t start)
    {
        for (size_t i = start; i < strText.size(); i++)
        {
            if (Dict::CharAttr(strText[i], CharSpace))
                return i;

            if (strText.at(i) == L'\r' || strText.at(i) == L'\n')
                break;
        }

        return strText.size();
    }

    static wstring PickupEnNumber(const wstring& strText, size_t start, bool bIncPunc = false)
    {
        if (start < 0)
            return L"";

        wstring str;
        int iPunc = 0;

        for (size_t i = start; i < strText.size(); i++)
        {
            if (!Dict::CharAttr(strText[i], CharEnNumber))
            {
                if (!bIncPunc || (!Dict::CharEqual(strText[i], L',') && !Dict::CharEqual(strText[i], L'.')))
                    break;

                iPunc++;
            }
            else
                str += Dict::DestChar(strText[i]);
        }

        if (iPunc > 0)
            str += wstring(iPunc, L' ');

        return str;
    }

    static wstring PickupEngWord(const wstring& strText, size_t start, bool bIncNum = true)
    {
        if (start < 0)
            return L"";

        wstring str;
        for (size_t i = start; i < strText.size(); i++)
        {
            if (!Dict::CharAttr(strText[i], CharEnToWord))
                break;

            if (!bIncNum && Dict::CharAttr(strText[i], CharNumber))
                break;

            str += Dict::DestChar(strText[i]);
        }

        return str;
    }
};

}

#endif
