#ifndef DICT_H
#define DICT_H

#include "header.h"
#include "dict_attribute.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include "log.h"
#include "utility.h"

using namespace std;

namespace word_segment {

struct CharDict
{
    wchar_t destChar;    //转换后对应的字符
    emCharAttribute attribute;//该字符的属性，emCharAttribute类型
};

struct WordDict
{
    wstring word;    //词串本身
    bool bChn;    //True 中文词或词组，False 英文词或词组
    bool bAutoNew;    //该词是否被自动判别的，如果是为True,否则为False
    unsigned int timeCount;    //该词在文章中出现的总次数
    vector<emWordAttribute> vctWordAttr;    //词性标注集，即是动词还是名词，其值为emWordAttribute中的枚举值或其组合

    void IncreaseTimeCount() { timeCount++; }

    bool IsNeedWord(emWordAttribute attr) const
    {
        for (vector<emWordAttribute>::const_iterator iter = vctWordAttr.begin(); iter != vctWordAttr.end(); iter++)
        {
            if (*iter == attr)
            return true;
        }

        return false;
    }
};


class Dict
{
public:
    //两个字符是否相同
    static bool CharEqual(wchar_t c1, wchar_t c2)
    {
        return m_charDict[c1].destChar == m_charDict[c2].destChar;
    }
    //判断字符属性
    static bool CharAttr(wchar_t c, emCharAttribute cAttr, bool bNoZero = false)
    {
        if (bNoZero)
            return (m_charDict[c].attribute & cAttr) != 0;
        else
            return (m_charDict[c].attribute & cAttr) == cAttr;
    }
    //获取该字符的目标字符
    static wchar_t DestChar(wchar_t c)
    {
        return m_charDict[c].destChar;
    }
    //判断是否已经装载词典
    static bool IsLoadedDict()
    {
        return m_dictHash.size() > 0;
    }
    //查找该字符的字典属性
    static const CharDict* SearchChar(wchar_t wch)
    {
        if (NULL == m_charDict)
            return NULL;

        return &m_charDict[wch];
    }
    //查找该词的词典属性
    static const WordDict* SearchWord(wstring str)
    {
        str = Utility::SBCtoDBC(Utility::ToUpper(str));
        unordered_map<wstring, WordDict*>::iterator iter = m_dictHash.find(str);
        if (iter == m_dictHash.end())
            return NULL;
        else
            return iter->second;
    }

    //获得以字符串的目标字符串
    static wstring GetDestString(wstring sour)
    {
        wstring dest;
        for (size_t i = 0 ; i < sour.size(); i++)
            dest += m_charDict[sour[i]].destChar;

        return dest;
    }

    static unordered_map<wstring, WordDict*>* GetDictHash(){return &m_dictHash;}
    static unordered_map<wchar_t, void*>* GetForHashMap(){ return &m_forHashMap; }
    static unordered_map<wchar_t, void*>* GetBackHashMap(){ return &m_backHashMap; }

    static bool LoadCharDict(string dictFileName);    //装载字符属性表
    static bool LoadWordDict(string dictFileName);    //装载词属性表
    static bool SaveWordDict(string dictFileName);    //保存词属性表
    static bool DeleteWord(wstring strWord);    //删除一个词
    static bool AddAutoNewWord(wstring newWord, emWordAttribute attr);    //添加新的自动识别的词
    static bool IsNewWord(wstring word);        //该词是否是新识别没有人工确认的词汇词组
    static bool RemoveFromNewWord(wstring word, bool bDelFromDict);//删除一个词从新词队列，如果bDelFromDict为True，将会从词典中彻底删除

protected:
    static bool AddItem(WordDict* dictWord);
    static bool AddItemToMM(const wchar_t* word, unordered_map<wchar_t, void*> *hashMap);
    static bool DeleteItem(const wchar_t* word, unordered_map<wchar_t, void*> *hashMap, bool& bSingle);

protected:
    static CharDict* m_charDict;            //字符属性表
    static unordered_map<wstring, WordDict*> m_dictHash;    //用于查询的分词词典
    static unordered_map<wchar_t, void*> m_forHashMap;    //正向最大分词词典
    static unordered_map<wchar_t, void*> m_backHashMap;    //逆向最大分词词典
    static map<wstring, WordDict*> m_mapNewWord;    //用于存放新识别没有人工确认的词汇词组
};

}

#endif
