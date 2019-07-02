#ifndef STAT_TEXT_H
#define STAT_TEXT_H

#include <map>
#include <string>

#include <seg_word.h>
#include <dict.h>

#include "category.h"

namespace opinion_analysis {

using namespace std;
using namespace word_segment;

struct WordStat
{
    WordStat(const WordDict* p)
    {
        iShowCount = 0;
        pWordDict = p;
    }

    void AddPosition(int pos)
    {
        iShowCount++;
        vctPosition.push_back(pos);
    }

    int iShowCount;
    vector<int> vctPosition;
    const WordDict* pWordDict;
};

class StatText
{
public:
    StatText(void);
    ~StatText(void);

    bool CalcText(const wstring& strSubject, const wstring& strText, Category type);
    bool CalcText(const vector<SegWord>& vctSubject, const vector<SegWord>& vctText, Category type);

    const map<wstring, WordStat>* getMapWordStat() const {return &m_mapWordStat;}
    const vector<SegWord>* getVctWord() const {return &m_vctWord;}
    const vector<SegWord>* getVctSubject() const { return &m_vctSubject; }
protected:
    map<wstring, WordStat> m_mapWordStat;
    vector<SegWord> m_vctWord;
    vector<SegWord> m_vctSubject;
    Category m_typeText;
};

}
#endif //STAT_TEXT_H
