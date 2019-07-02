#ifndef UTILITY_H
#define UTILITY_H

#include "header.h"
#include <string>
#include <vector>
#include "exception_ex.h"
#include "wchar.h"
#include <sstream>
#include <iomanip>

using namespace std;
namespace word_segment {
class Utility
{
public:
    static const wstring SBCtoDBC(wstring sbcWStr)
    {
        for (size_t i = 0; i < sbcWStr.size(); i++)
        {
            if(sbcWStr[i] >= 0xff00 && sbcWStr[i] <= 0xff5f)
                sbcWStr[i] -= 0xFEE0;
        }

        return sbcWStr;
    }

    static const wstring DBCtoSBC(wstring dbcWStr)
    {
        for (size_t i = 0; i < dbcWStr.size(); i++)
        {
            if(dbcWStr[i] >= 0x20 && dbcWStr[i] <= 0x7f)
                dbcWStr[i] += 0xFEE0;
        }

        return dbcWStr;
    }

    static const wstring ToLower(wstring wstr)
    {
        for (size_t i = 0; i < wstr.size(); i++)
        {
            if ((wstr[i] >=  L'A' && wstr[i] <= L'Z') || (wstr[i] >=  L'Ａ' && wstr[i] <= L'Ｚ'))
                wstr[i] += 32;
        }

        return wstr;
    }

    static const wstring ToUpper(wstring wstr)
    {
        for (size_t i = 0; i < wstr.size(); i++)
        {
            if ((wstr[i] >=  L'a' && wstr[i] <= L'z') || (wstr[i] >=  L'ａ' && wstr[i] <= L'ｚ'))
                wstr[i] -= 32;
        }

        return wstr;
    }

    static vector<wstring> SplitWString(const wstring& sourWStr, const wstring& separater, bool RemoveEmptyEntries = false)
    {
        vector<wstring> result;

        size_t pos = 0;
        for (size_t i = 0; i < sourWStr.size(); i++)
        {
            if (separater.find(sourWStr[i]) != wstring::npos)
            {
                if (pos < i || (pos == i && !RemoveEmptyEntries))
                {
                    result.push_back(sourWStr.substr(pos, i - pos));
                }

                pos = i + 1;
            }
        }

        if (pos < sourWStr.size())
        {
            result.push_back(sourWStr.substr(pos, sourWStr.size() - pos));
        }

        return result;
    }

    static vector<string> SplitString(const string& sourStr, const string& separater, bool RemoveEmptyEntries = false)
    {
        vector<string> result;

        size_t pos = 0;
        for (size_t i = 0; i < sourStr.size(); i++)
        {
            if (separater.find(sourStr[i]) != string::npos)
            {
                if (pos < i || (pos == i && !RemoveEmptyEntries))
                {
                    result.push_back(sourStr.substr(pos, i - pos));
                }

                pos = i + 1;
            }
        }

        if (pos < sourStr.size())
        {
            result.push_back(sourStr.substr(pos, sourStr.size() - pos));
        }

        return result;
    }

    static wstring ReverseString(const wstring& strSour)
    {
        wstring strDest;

        for (size_t i = strSour.size() -1; i != wstring::npos; i--)
        {
            strDest += strSour[i];;
        }

        return strDest;
    }

    static string ReverseString(const string& strSour)
    {
        string strDest;

        for (size_t i = strSour.size() -1; i != string::npos; i--)
        {
            strDest += strSour[i];;
        }

        return strDest;
    }

    static wstring IntToWStr(int ii)
    {
        std::wstringstream ss;
        ss << ii;
        return ss.str();
    }

    static string IntToStr(int ii)
    {
        std::stringstream ss;
        ss << ii;
        return ss.str();
    }

    static wstring IntToHexWStr(int ii, int bitNumber = -1)
    {
         if (bitNumber > 20)
            throw Exception("IntToHexStr's bitNumber is more than 20!");

        std::wstringstream ss;
        ss << L"0x";
        if (bitNumber > 0)
            ss << std::setfill(L'0') << std::setw(bitNumber);
        ss << std::hex << ii;

        return ss.str();
    }

    static string IntToHexStr(int ii, int bitNumber = -1)
    {
        if (bitNumber > 20)
            throw Exception("IntToHexStr's bitNumber is more than 20!");

        std::stringstream ss;
        ss << "0x";
        if (bitNumber > 0)
            ss << std::setfill('0') << std::setw(bitNumber);
        ss << std::hex << ii;

        return ss.str();
    }

    static long WStrToLong(wstring str)
    {
        wchar_t* pStop;
        return wcstol(str.c_str(), &pStop, 10);
    }

    static bool WStringToInt(wstring str, int& value)
    {
        value = 0;

        if (str.size() == 0)
            return true;

        const wstring strNum = L"零○一二两三四五六七八九十百千万亿0123456789０１２３４５６７８９,，";

        size_t iStart = wstring::npos;
        size_t iEnd = str.size();

        for (size_t i = 0; i < str.size(); i++)
        {
            if (strNum.find(str[i]) != wstring::npos)
            {
                if (iStart == wstring::npos)
                    iStart = i;
            }
            else if (iStart != wstring::npos)
            {
                iEnd = i;
                break;
            }
        }

        if (iStart == wstring::npos || iEnd - iStart <= 0)
            return false;

        if (iStart != 0 || iEnd < str.size())
            str = str.substr(iStart, iEnd - iStart);

        const wstring hanNum = L"亿万千百十";
        const int hanVal[] = {100000000, 10000, 1000, 100, 10};

        for (int i = 0; i < 5; i++)
        {
            if (str.find(hanNum[i]) != wstring::npos)
            {
                size_t pos = str.find(hanNum[i]);
                if (pos == 0 && i != 4)
                    return false;

                int val1 = 0;
                int val2 = 0;

                if (!WStringToInt(str.substr(0, pos), val1))
                    return false;

                if (!WStringToInt(str.substr(pos + 1), val2))
                    return false;

                if (i == 4 && pos == 0)
                    val1 = 1;

                value = val1 * hanVal[i] + val2;
                return true;
            }
        }

        for (size_t i = 0; i < str.size(); i++)
        {
            switch (str[i])
            {
            case L'零':
            case L'○':
                if (str.size() != 1)
                    return false;
                break;
            case L'一':
                value = 1;
                if (str.size() != 1)
                    return false;
                break;
            case L'二':
            case L'两':
                if (str.size() != 1)
                    return false;
                value = 2;
                break;
            case L'三':
                if (str.size() != 1)
                    return false;
                value = 3;
                break;
            case L'四':
                if (str.size() != 1)
                    return false;
                value = 4;
                break;
            case L'五':
                if (str.size() != 1)
                    return false;
                value = 5;
                break;
            case L'六':
                if (str.size() != 1)
                    return false;
                value = 6;
                break;
            case L'七':
                if (str.size() != 1)
                    return false;
                value = 7;
                break;
            case L'八':
                if (str.size() != 1)
                    return false;
                value = 8;
                break;
            case L'九':
                if (str.size() != 1)
                    return false;
                value = 9;
                break;
            case L'十':
                if (str.size() != 1)
                    return false;
                value = 10;
                break;
            case L'0':
            case L'1':
            case L'2':
            case L'3':
            case L'4':
            case L'5':
            case L'6':
            case L'7':
            case L'8':
            case L'9':
                value = value * 10 + str[i] - L'0';
                break;
            case L'０':
            case L'１':
            case L'２':
            case L'３':
            case L'４':
            case L'５':
            case L'６':
            case L'７':
            case L'８':
            case L'９':
                value = value * 10 + str[i] - L'０';
                break;
            default:
                break;
            }
        }

        return true;
    }

    static string& Trim(string& s)
    {
        if (s.empty())
            return s;

        string::iterator c;
        // Erase whitespace before the string
        for (c = s.begin(); c != s.end(); c++)
        {
            if (*c == ' ' || *c == '\t')
                continue;
            break;
        }

        s.erase(s.begin(), c);
        if (s.empty())
            return s;

        // Erase whitespace after the string
        for (c = s.end(); c != s.begin(); c--)
        {
            if (c == s.end() || *c == ' ' || *c == '\t')
                continue;
            break;
        }

        s.erase(++c, s.end());

        return s;
    }
};

}

#endif

