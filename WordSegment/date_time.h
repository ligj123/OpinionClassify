#ifndef DATE_TIME_H
#define DATE_TIME_H

#include "header.h"
#include <string>
#include <sstream>
#include "utility.h"

namespace word_segment {
class DateTime
{
public:
	DateTime(void)
	{
        Clear();
	}

    DateTime(int year, int month = -1, int day = -1, int hour = -1, int minute = -1, int second = -1)
	{
        m_iYear = year;
        m_iMonth = month;
        m_iDay = day;

        m_iHour = hour;
        m_iMinute = minute;
        m_iSecond = second;
	}

    DateTime(const DateTime& dt)
    {
        *this = dt;
    }

	~DateTime(void)
	{
	}

	void Clear()
	{
        m_iYear = 0;
        m_iMonth = -1;
        m_iDay = -1;

        m_iHour = -1;
        m_iMinute = 0;
        m_iSecond = 0;
	}
	bool IsNull()
	{
        return m_iYear == 0;
	}

	int GetYear()
	{
        return m_iYear;
	}

    bool SetDate(int year, int mon = -1, int day = -1, int hour = -1, int minute = 0, int second = 0)
	{
        if (year > MAX_YEAR || year < MIN_YEAR)
            return false;
        if (mon > 12 || mon < -1)
            return false;
        if (day > 31 || day < -1)
            return false;
        if (hour > 23 || hour < -1)
            return false;
        if (minute > 59 || minute < 0)
            return false;
        if (second > 59 || second < 0)
            return false;

        m_iYear = year;
        m_iMonth = mon;
        m_iDay = day;

        m_iHour = hour;
        m_iMinute = minute;
        m_iSecond = second;

        return true;
	}

	bool FromString(wstring str)
	{
        Clear();

        vector<wstring> vct = Utility::SplitWString(str, L"/ :");

        if (vct.size() == 0 || vct.size() > 6)
            return false;

        for (int i = 0; i < (int)vct.size(); i++)
        {
            short val = (short)Utility::WStrToLong(vct[i]);
            switch(i)
            {
            case 0:
                m_iYear = val;
                break;
            case 1:
                m_iMonth = val;
                break;
            case 2:
                m_iDay = val;
                break;
            case 3:
                m_iHour = val;
                break;
            case 4:
                m_iMinute = val;
                break;
            case 5:
                m_iSecond = val;
                break;
            }
        }

        return true;
	}

	wstring ToString()
	{
        if (IsNull())
            return L"";

        wstringstream ss;
        ss << m_iYear;
        if (m_iMonth < 0)
            return ss.str();

        ss << "/" << m_iMonth;
        if (m_iDay < 0)
            return ss.str();

        ss << "/" << m_iDay;
        if (m_iHour < 0)
            return ss.str();

        ss << " " << m_iHour << ":" << m_iMinute << ":" << m_iSecond;

        return ss.str();
    }

	const DateTime& operator= (const DateTime& dt)
	{
        if (this == &dt)
            return *this;

        m_iYear = dt.m_iYear;
        m_iMonth = dt.m_iMonth;
        m_iDay = dt.m_iDay;

        m_iHour = dt.m_iHour;
        m_iMinute = dt.m_iMinute;
        m_iSecond = dt.m_iSecond;

        return *this;
	}

protected:
	short m_iYear;
	short m_iMonth;
	short m_iDay;
    short m_iHour;
    short m_iMinute;
    short m_iSecond;
};
}
#endif
