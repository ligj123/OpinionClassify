#ifndef DICT_ANALY_H
#define DICT_ANALY_H
#include "opinion_header.h"
#include <string>
#include <map>
#include <unordered_map>
#include <boost/thread/shared_mutex.hpp>
#include <vector>
#include <unordered_set>

#include <word_segment.h>
#include <seg_word.h>

#include "category.h"
#include "stat_text.h"

namespace opinion_analysis{
using namespace std;
using namespace word_segment;

enum emOpinion
{
    opnPositive = 0,
    opnNeutral = 1,
    opnNegative = 2,
};
///This struct will record the statistical information of the words' occurrence in the texts,
///if the text has been verified manual
///or if the text is positive or negative.
struct ItemStat
{
    ItemStat()
    {
        iWordsVerifiedPositive = 0;
        iWordsVerifiedNeutral = 0;
        iWordsVerifiedNegative = 0;
        fWordsUnverifiedPositive = 0;
        fWordsUnverifiedNeutral = 0;
        fWordsUnverifiedNegative = 0;
    }

    //Below 4 variables count how many times the word appearded in the texts. If appeared more than once in one text, it will repeat to count.
    int iWordsVerifiedPositive;//the text has been verified manually and is positive.
    int iWordsVerifiedNeutral;//Verified and neutral;
    int iWordsVerifiedNegative;//Verified and negative
    float fWordsUnverifiedPositive;//No verified and positive
    float fWordsUnverifiedNeutral;// No verified and neutral
    float fWordsUnverifiedNegative;//No verified and negative
};

struct ItemDict
{
    wstring word;   //the word itself.
    //the first parameter is which category the word belong.
    //The second parameter is the statistical information of occurrence of the word  in this category text.
    //If the word appears more than once, the record will add the actual times.
    map<Category, ItemStat*> mapStat;
};

class DictAnaly
{
public:
    DictAnaly(void);
    ~DictAnaly(void);
    bool LoadDict(const string& strPath);  //Load the statistical information from a binary file.
    bool SaveDict(const string& strPath);  //Save the statistical information to a binary file.
    bool LoadKeywords(const string& strPath);//Load key words and their intensity from file, for example: 本栏目，北京电视台
    bool LoadDenyWord(const string& strPath);
    //This methid will count the words in the text and add the information to m_mapDict.
    //text: the text will be counted
    //type: this text belong to which catgory
    //bVerified: if this text' type has been verified manually.
    //opinion: this text's opinion, positive, neutral or negative.
    //return bool: if the method has been called successfully.
    bool StatWords(const wstring& text, Category type, bool bVerified, emOpinion opinion);
    bool StatWords(const StatText& statText, Category type, bool bVerified, emOpinion opinion);
    void AddOpinionStat(wstring str, int iIntensity, emOpinion opinion);

    const ItemDict* const SearchItem(wstring word);
	const map<Category, ItemStat*>* const getMapStatTotal() { return &m_mapStatCategory; }
	const ItemStat* const getStatTotal(){ return &m_statTotal; }
    const unordered_map<wstring, ItemStat*>* const getMapEmotional(){ return &m_mapEmotional; }
    const unordered_map<wstring, int>* getMapKeywords(){ return &m_mapKeywords; }
    const unordered_set<wstring>* getHashDenyWords(){ return &m_hashDenyWords; }

protected:
    void Clear();
protected:
    unordered_map<wstring, ItemDict*> m_mapDict;//词汇对应的分类及正负面舆情统计信息
	map<Category, ItemStat*> m_mapStatCategory; //根据分类进行的统计信息
	ItemStat m_statTotal;//正负面舆情总的统计信息

    unordered_map<wstring, ItemStat*> m_mapEmotional;//人工整理的词汇表信息
    unordered_map<wstring, int> m_mapKeywords;//关键词，在正负面舆情判断中需要关注的关键词，以及该关键词的强度
    unordered_set<wstring> m_hashDenyWords;//否定词，例如不、没、无、莫、非等等

    boost::shared_mutex m_mutex;
};

}
#endif //DICT_ANALY_H