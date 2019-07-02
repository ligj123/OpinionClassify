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
    wchar_t destChar;    //ת�����Ӧ���ַ�
    emCharAttribute attribute;//���ַ������ԣ�emCharAttribute����
};

struct WordDict
{
    wstring word;    //�ʴ�����
    bool bChn;    //True ���Ĵʻ���飬False Ӣ�Ĵʻ����
    bool bAutoNew;    //�ô��Ƿ��Զ��б�ģ������ΪTrue,����ΪFalse
    unsigned int timeCount;    //�ô��������г��ֵ��ܴ���
    vector<emWordAttribute> vctWordAttr;    //���Ա�ע�������Ƕ��ʻ������ʣ���ֵΪemWordAttribute�е�ö��ֵ�������

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
    //�����ַ��Ƿ���ͬ
    static bool CharEqual(wchar_t c1, wchar_t c2)
    {
        return m_charDict[c1].destChar == m_charDict[c2].destChar;
    }
    //�ж��ַ�����
    static bool CharAttr(wchar_t c, emCharAttribute cAttr, bool bNoZero = false)
    {
        if (bNoZero)
            return (m_charDict[c].attribute & cAttr) != 0;
        else
            return (m_charDict[c].attribute & cAttr) == cAttr;
    }
    //��ȡ���ַ���Ŀ���ַ�
    static wchar_t DestChar(wchar_t c)
    {
        return m_charDict[c].destChar;
    }
    //�ж��Ƿ��Ѿ�װ�شʵ�
    static bool IsLoadedDict()
    {
        return m_dictHash.size() > 0;
    }
    //���Ҹ��ַ����ֵ�����
    static const CharDict* SearchChar(wchar_t wch)
    {
        if (NULL == m_charDict)
            return NULL;

        return &m_charDict[wch];
    }
    //���ҸôʵĴʵ�����
    static const WordDict* SearchWord(wstring str)
    {
        str = Utility::SBCtoDBC(Utility::ToUpper(str));
        unordered_map<wstring, WordDict*>::iterator iter = m_dictHash.find(str);
        if (iter == m_dictHash.end())
            return NULL;
        else
            return iter->second;
    }

    //������ַ�����Ŀ���ַ���
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

    static bool LoadCharDict(string dictFileName);    //װ���ַ����Ա�
    static bool LoadWordDict(string dictFileName);    //װ�ش����Ա�
    static bool SaveWordDict(string dictFileName);    //��������Ա�
    static bool DeleteWord(wstring strWord);    //ɾ��һ����
    static bool AddAutoNewWord(wstring newWord, emWordAttribute attr);    //����µ��Զ�ʶ��Ĵ�
    static bool IsNewWord(wstring word);        //�ô��Ƿ�����ʶ��û���˹�ȷ�ϵĴʻ����
    static bool RemoveFromNewWord(wstring word, bool bDelFromDict);//ɾ��һ���ʴ��´ʶ��У����bDelFromDictΪTrue������Ӵʵ��г���ɾ��

protected:
    static bool AddItem(WordDict* dictWord);
    static bool AddItemToMM(const wchar_t* word, unordered_map<wchar_t, void*> *hashMap);
    static bool DeleteItem(const wchar_t* word, unordered_map<wchar_t, void*> *hashMap, bool& bSingle);

protected:
    static CharDict* m_charDict;            //�ַ����Ա�
    static unordered_map<wstring, WordDict*> m_dictHash;    //���ڲ�ѯ�ķִʴʵ�
    static unordered_map<wchar_t, void*> m_forHashMap;    //�������ִʴʵ�
    static unordered_map<wchar_t, void*> m_backHashMap;    //�������ִʴʵ�
    static map<wstring, WordDict*> m_mapNewWord;    //���ڴ����ʶ��û���˹�ȷ�ϵĴʻ����
};

}

#endif
