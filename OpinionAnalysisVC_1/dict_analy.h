#ifndef DICT_ANALY_H
#define DICT_ANALY_H
#include <string>
#include <map>
#include <unordered_map>
#include <boost/thread/shared_mutex.hpp>
#include <vector>

#include <word_segment.h>
#include <seg_word.h>

#include "category.h"
#include "stat_text.h"

namespace opinion_analysis{
using namespace std;
using namespace word_segment;

struct ItemDict
{
    wstring word;   //the word itself.
    //the first parameter is which category the word belong.
    //The second parameter is the statistical information of occurrence of the word  in this category text.
    //If the word appears more than once, the record will add the actual times.
    map<Category, long> mapStat;
	double dblVar;//改词在不同类型分布的方差
};

class DictAnaly
{
public:
    DictAnaly(void);
    ~DictAnaly(void);
    bool LoadDict(const string& strPath);  //Load the statistical information from a binary file.
    bool SaveDict(const string& strPath);  //Save the statistical information to a binary file.
    //This methid will count the words in the text and add the information to m_mapDict.
    //text: the text will be counted
    //type: this text belong to which catgory
    //bVerified: if this text' type has been verified manually.
    //bNegative: if the text is negative message.
    //bEraseUnverified: if to erase the unverified statistical information, it will be effectively when bVerified=true.
    //Its default value is 0, means don't change the old data, -1 means the old data is negative and 1 means the old data is positiove.
    //return bool: if the method has been called successfully.
    bool StatWords(const wstring& text, Category type);
    bool StatWords(const StatText& statText, Category type);

    const ItemDict* const SearchItem(wstring word);
	const double GetVarAverg() { return m_dblVarAverg; }

protected:
    void Clear();
protected:
    //This variable is hashtable type, it will store all words' statistical information.
    //The first parameter is the word itself.
    //The second parameter is the word's statistical information.
    unordered_map<wstring, ItemDict*> m_mapDict;
	double m_dblVarAverg;//所有词汇方差的均值

    boost::shared_mutex m_mutex;
};

}
#endif //DICT_ANALY_H