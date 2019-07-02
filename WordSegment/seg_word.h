#ifndef SEG_WORD_H
#define SEG_WORD_H

#include "header.h"
#include <string>
#include <vector>
#include "dict_attribute.h"
#include "log.h"
#include "date_time.h"
#include "dict_attr.h"
#include "dict.h"

using namespace std;

namespace word_segment {

class SegWord
{
public:
	SegWord(void);
	SegWord(wstring sour, wstring dest, emWordType type, emReliability relia,
          const WordDict* pWordDict = NULL, const CharDict* pCharDict = NULL);
	~SegWord(void);

public:
	void SetValue(wstring sour, wstring dest, emWordType type, emReliability relia,
               const WordDict* pWordDict = NULL, const CharDict* pCharDict = NULL);
	void SetDateValue(wstring sour, wstring dest, emWordType type, emReliability relia);
	void Clear();
	bool IsNull(){ return m_strSour.size() == 0; }

	bool IsNeedWord(emWordAttribute attr);
	void WriteToLog();

public:
	wstring m_strSour;	//源字符串
	wstring m_strDest;	//目标字符串
	emWordType m_Type;	//
	emReliability m_Relia;
	const WordDict* m_pWordDict;
	const CharDict* m_pCharDict;
	DateTime m_datetime;	//仅当m_Type是日期时有效
};

}

#endif
