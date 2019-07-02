#ifndef INFO_IDEN_H
#define INFO_IDEN_H

#include "header.h"
#include "seg_word.h"
#include <string>
#include <vector>

using namespace std;

namespace word_segment {
class InfoIden
{
public:
    static bool PhoneMobileIden(const wstring& strText, size_t iStart, size_t& iEnd, SegWord& segWord);
    static bool EmailIden(const wstring& strText, size_t iStart, size_t& iEnd, SegWord& segWord);
    static bool HttpIden(const wstring& strText, size_t iStart, size_t& iEnd, SegWord& segWord);
    static bool IDCardIden(const wstring& strText, size_t iStart, size_t& iEnd, SegWord& segWord);
    static bool NumberIden(const wstring& strText, size_t iStart, size_t& iEnd, SegWord& segWord);
    static bool EngWordIden(const wstring& strText, size_t iStart, size_t& iEnd, SegWord& segWord);
    static bool WordIden(const wstring& strText, size_t iStart, size_t& iEnd, SegWord& segWord);

protected:
    static bool PhoneMobileIden_CN(const wstring& strText, size_t iStart, size_t& iEnd, SegWord& segWord);
};

}
#endif
