#ifndef DATE_IDEN_H
#define DATE_IDEN_H

#include "header.h"
#include "seg_word.h"
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

namespace word_segment {

class DateTimeIden
{
public:
	static bool IdenDateTime(const wstring& strText, size_t iStart, size_t& iEnd, SegWord& segWord);

protected:
	static emReliability IsEngDateTime(const wstring& strText, size_t iStart, size_t& iEnd, wstring& result);
	static emReliability IsDateTime(const wstring& strText, size_t iStart, size_t& iEnd, wstring& result);

	static void FillMapMonth();

protected:
	static unordered_map<wstring, int> m_mapMonth;	//英文月份对照表
};

}
#endif
