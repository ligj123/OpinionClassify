#ifndef OPINION_ANALYSIS_H
#define OPINION_ANALYSIS_H
#include "category.h"
#include "dict_analy.h"
namespace opinion_analysis {

enum AlgorithmSelect
{
    algBayesian = 1,
};

class OpinionAnalysis
{
public:
    static bool InitDictAnaly(const string& strPath);
    static bool StatWords(const wstring& text, Category type);
    static bool StatWords(const StatText& statText, Category type);
	static bool JudgeTextCategory(const wstring& text, Category& type, AlgorithmSelect alg = algBayesian);
    static void setDictAnaly(DictAnaly* pDictAnaly){m_pDictAnaly = pDictAnaly;}
    static bool SaveDictAnaly(const string& strPath);

private:
    static DictAnaly* m_pDictAnaly;
};

}
#endif // OPINION_ANALYSIS_H
