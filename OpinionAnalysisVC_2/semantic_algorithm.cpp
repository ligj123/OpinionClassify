#include "semantic_algorithm.h"

namespace opinion_analysis {
SemanticAlgorithm::SemanticAlgorithm()
{
    m_pDictAnaly = nullptr;
}

SemanticAlgorithm::SemanticAlgorithm(DictAnaly* pDictAnaly)
{
    m_pDictAnaly = pDictAnaly;
}

SemanticAlgorithm::~SemanticAlgorithm()
{
}

bool SemanticAlgorithm::JudgeTextCategory(const StatText& statText, Category& type)
{
    try
    {
    }
    catch (exception& ex)
    {
        Log::Write("SemanticAlgorithm::JudgeTextCategory", ex.what(), LogTypeException);
        return false;
    }
    catch (...)
    {
        Log::Write(L"SemanticAlgorithm::JudgeTextCategory", L"Failed to judge text category, Unknown error!", LogTypeException);
        return false;
    }

    return true;
}

bool SemanticAlgorithm::JudgeTextOpinion(const StatText& statText, Category type, emOpinion& opinion)
{
    bool hasTopicWord = false;
    double dblPostiveManual = 0;
    double dblNegativeManual = 0;
    double dblPostiveStat = 0;
    double dblNegativeStat = 0;
    bool bPrevDeny = false;

    try
    {
        const vector<SegWord>* pVctWord = statText.getVctWord();
        for (auto iter = pVctWord->begin(); iter != pVctWord->end(); iter++)
        {
            if (!hasTopicWord)
                hasTopicWord = IsTopicWord(&(*iter), statText.getVctSubject());

            if (!bPrevDeny && IsDenyWord(&(*iter)))
            {
                bPrevDeny = true;
                continue;
            }
            
            double dblPositive;
            double dblNegative;
            
            CalcWordOpinionManual(&(*iter), dblPositive, dblNegative);
            if (bPrevDeny)
            {
                dblPostiveManual += dblNegative;
                dblNegativeManual += dblPositive;
            }
            else
            {
                dblPostiveManual += dblPositive;
                dblNegativeManual += dblNegative;
            }

            bPrevDeny = false;
            
            CalcWordOpinionStat(&(*iter), dblPositive, dblNegative);
            dblPostiveStat += dblPositive;
            dblNegativeStat += dblNegative;
        }

        if (!hasTopicWord)
        {
            opinion = opnNeutral;
        }
        else
        {
            if (dblPostiveManual - dblNegativeManual > 7)
                opinion = opnPositive;
            else if (dblPostiveManual - dblNegativeManual < -7)
                opinion = opnNegative;
            //else if (dblPostiveStat - dblNegativeStat > 3)
            //    opinion = opnPositive;
            //else if (dblPostiveStat - dblNegativeStat < -3)
            //    opinion = opnNegative;
            else
                opinion = opnNeutral;
        }
    }
    catch (exception& ex)
    {
        Log::Write("SemanticAlgorithm::JudgeTextOpinion", ex.what(), LogTypeException);
        return false;
    }
    catch (...)
    {
        Log::Write(L"SemanticAlgorithm::JudgeTextOpinion", L"Failed to judge text opinion, Unknown error!", LogTypeException);
        return false;
    }

    return true;
}

bool SemanticAlgorithm::IsDenyWord(const SegWord* pSegWord)
{
    auto pHashDenyWord = m_pDictAnaly->getHashDenyWords();
    if (pHashDenyWord->find(pSegWord->m_strDest) != pHashDenyWord->end())
        return true;
    return false;
}

bool SemanticAlgorithm::IsTopicWord(const SegWord* pSegWord, const vector<SegWord>* pVctSubject)
{
    for (auto itSubject = pVctSubject->begin(); itSubject != pVctSubject->end(); itSubject++)
    {
        if (itSubject->m_strDest.size() < 2)
            continue;

        if (pSegWord->m_strDest == itSubject->m_strDest)
            return true;
    }

    const unordered_map<wstring, int>* pMapKey = m_pDictAnaly->getMapKeywords();
    if (pMapKey->find(pSegWord->m_strDest) != pMapKey->end())
        return true;

    return false;
}

void SemanticAlgorithm::CalcWordOpinionManual(const SegWord* pSegWord, double& dblPositive, double& dblNegative)
{
    dblPositive = 0;
    dblNegative = 0;

    auto iter = m_pDictAnaly->getMapEmotional()->find(pSegWord->m_strDest);
    if (iter == m_pDictAnaly->getMapEmotional()->end())
        return;

    dblPositive = iter->second->iWordsVerifiedPositive;
    dblNegative = iter->second->iWordsVerifiedNegative;
}

void SemanticAlgorithm::CalcWordOpinionStat(const SegWord* pSegWord, double& dblPositive, double& dblNegative)
{
    dblPositive = 0;
    dblNegative = 0;

    auto iter = m_pDictAnaly->getMapEmotional()->find(pSegWord->m_strDest);
    if (iter == m_pDictAnaly->getMapEmotional()->end())
        return;

    ItemStat* pItem = iter->second;
    if (pItem->fWordsUnverifiedNegative - pItem->fWordsUnverifiedNeutral > 3)
        dblPositive = iter->second->fWordsUnverifiedNegative - pItem->fWordsUnverifiedNeutral;
    if (pItem->fWordsUnverifiedPositive - pItem->fWordsUnverifiedNeutral > 3)
        dblNegative = pItem->fWordsUnverifiedPositive - pItem->fWordsUnverifiedNeutral;
}

}