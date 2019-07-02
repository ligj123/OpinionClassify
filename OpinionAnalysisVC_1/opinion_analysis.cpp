#include "opinion_analysis.h"
#include "ialgorithm.h"
#include "bayesian_algorithm.h"
#include <boost/scoped_ptr.hpp>

namespace opinion_analysis {
DictAnaly* OpinionAnalysis::m_pDictAnaly = nullptr;

bool OpinionAnalysis::InitDictAnaly(const string& strPath)
{
    if (nullptr != m_pDictAnaly)
        delete m_pDictAnaly;

    m_pDictAnaly = new DictAnaly();
    return m_pDictAnaly->LoadDict(strPath);
}

bool OpinionAnalysis::SaveDictAnaly(const string& strPath)
{
    if (nullptr == m_pDictAnaly)
        return false;

    return m_pDictAnaly->SaveDict(strPath);
}

bool OpinionAnalysis::StatWords(const wstring& text, Category type)
{
    if (nullptr == m_pDictAnaly)
        return false;

    return m_pDictAnaly->StatWords(text, type);
}

bool OpinionAnalysis::StatWords(const StatText& statText, Category type)
{
    if (nullptr == m_pDictAnaly)
        return false;

    return m_pDictAnaly->StatWords(statText, type);
}

bool OpinionAnalysis::JudgeTextCategory(const wstring& text, Category& type, AlgorithmSelect alg)
{
    boost::scoped_ptr<IAlgorithm> pIAlg(nullptr);
    switch (alg)
    {
    case algBayesian:
        pIAlg.reset(new BayesianAlgorithm);
        break;
    default:
        throw Exception("Unknown algorithm!");
    }

    StatText statText;
    if (!statText.CalcText(text, type))
        return false;

    pIAlg->SetDictAnaly(m_pDictAnaly);

/*    if (tcUnknown == type)
    {
        if (!pIAlg->JudgeTextCategory(statText, type))
            return false;
    }
*/
    return pIAlg->JudgeTextCategory(statText, type);
}

}
