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
		double var = m_pDictAnaly->GetVarAverg();
        assert(nullptr != m_pDictAnaly);
        const map<wstring, WordStat>* pMap = statText.getMapWordStat();
        if (pMap->size() == 0)
            return false;

		while (var > 0.01)
		{
			map<Category, double> mapChance;

			for (auto iter = pMap->begin(); iter != pMap->end(); iter++)
			{
				const ItemDict* const pItem = m_pDictAnaly->SearchItem(iter->first);
				if (nullptr == pItem || pItem->dblVar < var)
					continue;

				const map<Category, double> map = CalcCategoryWeights(pItem);///

				for (auto itChance = map.begin(); itChance != map.end(); itChance++)
				{
					mapChance[itChance->first] += itChance->second / iter->second.iShowCount;
				}
			}

			if (mapChance.size() == 0) {
				var *= 0.3;
				continue;
			}
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

			break;
		}
    }
	catch(exception& ex)
	{
        Log::Write("BayesianAlgorithm::JudgeTextCategory", ex.what(), LogTypeException);
        return false;
	}
	catch(...)
	{
        Log::Write(L"BayesianAlgorithm::JudgeTextCategory", L"Failed to count words' statistical information, Unknown error!", LogTypeException);
        return false;
	}

    return true;
}

const map<Category, double> BayesianAlgorithm::CalcCategoryWeights(const ItemDict* pItem)
{
	double dblCount = 0;

    for (auto iter = pItem->mapStat.cbegin(); iter != pItem->mapStat.cend(); iter++)
    {
		dblCount += iter->second;
    }

    map<Category, double> map;
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
            double dblType = iter->second + 1;
            map.insert(pair<Category, double>(arCategory[i], log(dblType /  dblCount)));
        }
    }

    return map;
}

}
