#include "bayesian_algorithm.h"
#include <log.h>

namespace opinion_analysis {

BayesianAlgorithm::BayesianAlgorithm(void)
{
    m_pDictAnaly = nullptr;
}

BayesianAlgorithm::BayesianAlgorithm(DictAnaly* pDictAnaly)
{
    m_pDictAnaly = pDictAnaly;
}

BayesianAlgorithm::~BayesianAlgorithm(void)
{
}


bool BayesianAlgorithm::JudgeTextCategory(const StatText& statText, Category& type)
{
    try
    {
        assert(nullptr != m_pDictAnaly);
        const map<wstring, WordStat>* pMap = statText.getMapWordStat();
        if (pMap->size() == 0)
            return false;

        map<Category, double> mapChance;
		map<Category, double> mapNPRate;

		const map<Category, ItemStat*>* pMapStat = m_pDictAnaly->getMapStatTotal();
		double dblCount = 0;

		for (auto iter = pMapStat->begin(); iter != pMapStat->end(); iter++)
		{
			double d = (iter->second->iWordsVerifiedNegative + iter->second->fWordsUnverifiedNegative * UNVERIFIED_WEIGHT) +
                (iter->second->iWordsVerifiedNeutral + iter->second->fWordsUnverifiedNeutral * UNVERIFIED_WEIGHT) +
				(iter->second->iWordsVerifiedPositive + iter->second->fWordsUnverifiedPositive * UNVERIFIED_WEIGHT) + 1;
			mapNPRate[iter->first] = d;
			dblCount += d;
		}

		for (auto iter = mapNPRate.begin(); iter != mapNPRate.end(); iter++)
		{
			iter->second = dblCount / iter->second;
		}

        for (auto iter = pMap->begin(); iter != pMap->end(); iter++)
        {
            const ItemDict* const pItem = m_pDictAnaly->SearchItem(iter->first);
            if (nullptr == pItem)
                continue;

            const map<Category, double> map = CalcCategoryWeights(pItem, mapNPRate);

            for (auto itChance = map.begin(); itChance != map.end(); itChance++)
            {
                mapChance[itChance->first] += itChance->second * iter->second.iShowCount;
            }
        }

        if (mapChance.size() == 0)
            return true;
        type = mapChance.begin()->first;
        double dblMax = mapChance.begin()->second;
        for (auto iter = mapChance.begin(); iter != mapChance.end(); iter++)
        {
            if (dblMax < iter->second)
            {
                dblMax = iter->second;
                type = iter->first;
            }
        }
    }
	catch(exception& ex)
	{
        Log::Write("BayesianAlgorithm::JudgeTextCategory", ex.what(), LogTypeException);
        return false;
	}
	catch(...)
	{
        Log::Write(L"BayesianAlgorithm::JudgeTextCategory", L"Failed to judge text category, Unknown error!", LogTypeException);
        return false;
	}

    return true;
}

bool BayesianAlgorithm::JudgeTextOpinion(const StatText& statText, Category type, emOpinion& opinion)
{
    try
    {
        assert(nullptr != m_pDictAnaly);
        const map<wstring, WordStat>* pMap = statText.getMapWordStat();
        if (pMap->size() == 0)
            return false;

		const ItemStat* pStat = m_pDictAnaly->getStatTotal();
        double dblNegative = pStat->iWordsVerifiedNegative + pStat->fWordsUnverifiedNegative * UNVERIFIED_WEIGHT + 1;
        double dblNeutral = pStat->iWordsVerifiedNeutral + pStat->fWordsUnverifiedNeutral * UNVERIFIED_WEIGHT + 1;
        double dblPositive = pStat->iWordsVerifiedPositive + pStat->fWordsUnverifiedPositive * UNVERIFIED_WEIGHT + 1;
        double dblCount = dblNegative + dblNeutral + dblPositive;
        map<emOpinion, double> mapNRate;
        mapNRate.insert(pair<emOpinion, double>(opnNegative, dblCount / dblNegative));
        mapNRate.insert(pair<emOpinion, double>(opnNeutral, dblCount / dblNeutral));
        mapNRate.insert(pair<emOpinion, double>(opnPositive, dblCount / dblPositive));

        double dblNegativeSum = 0;
        double dblNeutralSum = 0;
        double dblPositiveSum = 0;

        for (auto iter = pMap->begin(); iter != pMap->end(); iter++)
        {
            const ItemDict* const pItem = m_pDictAnaly->SearchItem(iter->first);
            if (nullptr == pItem)
                continue;

            map<emOpinion, double> map = CalcOpinionWeights(pItem, type, mapNRate);

            dblNegativeSum += map.find(opnNegative)->second * iter->second.iShowCount;
            dblNeutralSum += map.find(opnNeutral)->second * iter->second.iShowCount;
            dblPositiveSum += map.find(opnPositive)->second * iter->second.iShowCount;
        }

        opinion = (dblNegativeSum > dblNeutralSum) ? opnNegative : opnNeutral;
        if (dblPositiveSum > dblNegativeSum && dblPositiveSum > dblNeutralSum)
            opinion = opnPositive;
  
    }
	catch(exception& ex)
	{
        Log::Write("BayesianAlgorithm::JudgeTextOpinion", ex.what(), LogTypeException);
        return false;
	}
	catch(...)
	{
        Log::Write(L"BayesianAlgorithm::JudgeTextOpinion", L"Failed to count words' statistical information, Unknown error!", LogTypeException);
        return false;
	}

    return true;
}

const map<Category, double> BayesianAlgorithm::CalcCategoryWeights(const ItemDict* pItem, const map<Category, double>& mapNPRate)
{
    double dblVerifiedCount = 0;
    double dblUnverifiedCount = 0;

    for (auto iter = pItem->mapStat.cbegin(); iter != pItem->mapStat.cend(); iter++)
    {
        dblVerifiedCount += (iter->second->iWordsVerifiedNegative + iter->second->iWordsVerifiedNeutral + iter->second->iWordsVerifiedPositive)
            * mapNPRate.find(iter->first)->second;
        dblUnverifiedCount += (iter->second->fWordsUnverifiedNegative + iter->second->fWordsUnverifiedNeutral + iter->second->fWordsUnverifiedPositive)
            * mapNPRate.find(iter->first)->second;
    }

    map<Category, double> map;
    double dblCount = dblVerifiedCount + dblUnverifiedCount * UNVERIFIED_WEIGHT;
    if (dblCount < MIN_COUNT)
        return map;

    dblCount += 1;

    for (size_t i = 0; i < sizeof(arCategory) / sizeof(Category); i++)
    {
        const auto iter = pItem->mapStat.find(arCategory[i]);
        if (iter == pItem->mapStat.end())
            map.insert(pair<Category, double>(arCategory[i], log(1 /  dblCount)));
        else
        {
            double dblType = iter->second->iWordsVerifiedNegative + iter->second->iWordsVerifiedNeutral +iter->second->iWordsVerifiedPositive +
                (iter->second->fWordsUnverifiedNegative + iter->second->fWordsUnverifiedNeutral +iter->second->fWordsUnverifiedPositive) * UNVERIFIED_WEIGHT + 1;
			map.insert(pair<Category, double>(arCategory[i], log(dblType * mapNPRate.find(arCategory[i])->second / dblCount)));
        }
    }

    return map;
}

const map<emOpinion, double> BayesianAlgorithm::CalcOpinionWeights(const ItemDict* pItem, const Category type, const map<emOpinion, double>& mapNPRate)
{
    double dblPositiveCount = 0;
    double dblNegativeCount = 0;
    double dblNeutralCount = 0;

    for (auto iter = pItem->mapStat.cbegin(); iter != pItem->mapStat.cend(); iter++)
    {
        if (type == iter->first)
        {
            dblPositiveCount += iter->second->iWordsVerifiedPositive +
                iter->second->fWordsUnverifiedPositive * UNVERIFIED_WEIGHT;
            dblNeutralCount += iter->second->iWordsVerifiedNeutral +
                iter->second->fWordsUnverifiedNeutral * UNVERIFIED_WEIGHT;
            dblNegativeCount += iter->second->iWordsVerifiedNegative +
                iter->second->fWordsUnverifiedNegative * UNVERIFIED_WEIGHT;
        }
        else
        {
            dblPositiveCount += (iter->second->iWordsVerifiedPositive +
                iter->second->fWordsUnverifiedPositive * UNVERIFIED_WEIGHT) * CATEGORY_WEIGHT;
            dblNeutralCount += (iter->second->iWordsVerifiedNeutral +
                iter->second->fWordsUnverifiedNeutral * UNVERIFIED_WEIGHT) * CATEGORY_WEIGHT;
            dblNegativeCount += (iter->second->iWordsVerifiedNegative +
                iter->second->fWordsUnverifiedNegative * UNVERIFIED_WEIGHT) * CATEGORY_WEIGHT;
        }
    }

    map<emOpinion, double> map;
    if (dblPositiveCount + dblNeutralCount + dblNegativeCount < MIN_COUNT)
    {        
        map.insert(pair<emOpinion, double>(opnPositive, 0));
        map.insert(pair<emOpinion, double>(opnNeutral, 0));
        map.insert(pair<emOpinion, double>(opnNegative, 0));
        return map;
    }

    double dblCount = dblPositiveCount + dblNegativeCount + dblNeutralCount + 1;

    map.insert(pair<emOpinion, double>(opnPositive, log((dblPositiveCount + 1) * mapNPRate.find(opnPositive)->second / dblCount)));
    map.insert(pair<emOpinion, double>(opnNeutral, log((dblNeutralCount + 1) * mapNPRate.find(opnNeutral)->second / dblCount)));
    map.insert(pair<emOpinion, double>(opnNegative, log((dblNegativeCount + 1) * mapNPRate.find(opnNegative)->second / dblCount)));
	
    return map;
}

}
