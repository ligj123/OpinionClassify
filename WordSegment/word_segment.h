#ifndef WORD_SEGMENT_H
#define WORD_SEGMENT_H

#include "header.h"
#include "dict_attribute.h"
#include "seg_word.h"
#include "info_iden.h"
#include "date_iden.h"

namespace word_segment {

class WordSegment
{
public:
	WordSegment(void);
	~WordSegment(void);

public:
	bool SegmentWordMaxMatch(const wstring& strText, wstring &strResult);
	bool SegmentWordMaxMatch(const wstring& strText, vector<SegWord> &vctResult);

    bool SegmentWordMinMatch(const wstring& strText, wstring& strResult);
    bool SegmentWordMinMatch(const wstring& strText, vector<SegWord>& vctResult);
protected:
	bool SpliteTextByExplicitMarker(const wstring& strText, vector<SegWord> &vctResult);
	string ForwordMaxMatchMethod(const wstring& strText);
	string BackwordMaxMatchMethod(const wstring& strText);
	bool MaxMatchMethod(const wstring& strText, const string& forMarker, const string& backMarker, vector<SegWord> &vctResult);
	bool SpliteAmbiguousWord(const wstring& strText, const string& forMarker, const string& backMarker, vector<SegWord> &vctResult);
};

}
#endif
