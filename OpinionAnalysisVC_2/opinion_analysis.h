#ifndef OPINION_ANALYSIS_H
#define OPINION_ANALYSIS_H
#include "dict_analy.h"
#include "category.h"

namespace opinion_analysis {

enum AlgorithmSelect
{
    algBayesian = 1,
    algSemantic = 2,
};

class OpinionAnalysis
{
public:
    static bool InitDictAnaly(const string& strPath);
    static bool StatWords(const wstring& text, Category type, bool bVerified, emOpinion opinion);
    static bool StatWords(const StatText& statText, Category type, bool bVerified, emOpinion opinion);
    static bool JudgeTextOpinion(const wstring& strSubject, const wstring& text, Category& type, emOpinion& opinion, AlgorithmSelect alg = algBayesian);
    static bool JudgeTextCategory(const wstring& strSubject, const wstring& text, Category& type, AlgorithmSelect alg = algBayesian);
    static bool SaveDictAnaly(const string& strPath);
    static void setDictAnaly(DictAnaly* pDictAnaly){m_pDictAnaly = pDictAnaly;}
    static DictAnaly* getDictAnaly(){ return m_pDictAnaly; }

private:
    static DictAnaly* m_pDictAnaly;
};

}
#endif // OPINION_ANALYSIS_H
